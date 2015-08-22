///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2015 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "parser.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

static void unexpectedtokenerror(
    Parser*     parser
);

Parser* parser_new(
    const char* source,
    StringPool* stringpool
)
{
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    assert(parser);

    parser->lexer = lexer_new(source);
    parser->stringpool = stringpool;

    // Move to the first token
    lexer_next(parser->lexer);

    return parser;
}

void parser_free(
    Parser* parser
)
{
    assert(parser);

    // Free the lexer
    if (parser->lexer)
    {
        free(parser->lexer);
    }

    free(parser);
}

void parser_seterror(
    Parser*     parser,
    const char* fmt,
    ...
)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(parser->error, fmt, args);
    va_end(args);

    LexerState state = lexer_savestate(parser->lexer);
    sprintf(parser->fullerror, "%s on line %i", parser->error, state.line);
}

const char* parser_geterror(
    Parser* parser
)
{
    return parser->fullerror;
}

Node* parser_exprs(
    Parser* parser,
    bool    newlines
)
{
    // Parse an expression
    Node* expr = parser_expr(parser);
    if (!expr)
    {
        return NULL;
    }

    // If there is a trailing comma or a new line, then recursively parse
    // another expression sequence
    Node* exprs = NULL;
    if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ',') ||
            (newlines && lexer_skippednewline(parser->lexer) &&
             !lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ']') &&
             !lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '}')))
    {
        if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ','))
        {
            lexer_next(parser->lexer);
        }

        exprs = parser_exprs(parser, newlines);
        if (!exprs)
        {
            node_free(expr);
            return NULL;
        }
    }

    // Create a sequence node for this expression and the potential expressions
    // after it
    Node* sequence = node_new(NODE_SEQUENCE);
    sequence->data.sequence.expr = expr;
    sequence->data.sequence.next = exprs;
    return sequence;
}

Node* parser_expr(
    Parser* parser
)
{
    return parser_binexpr(parser, 0, parser_primaryexpr(parser));
}

Node* parser_primaryexpr(
    Parser* parser
)
{
    // Unary operator
    if (lexer_istokentype(parser->lexer, TOK_OPERATOR))
    {
        Operator op = (Operator)lexer_gettokenid(parser->lexer);
        if (op == OP_SUB)
        {
            op = OP_NEG;
        }

        if (op != OP_NOT && op != OP_NEG)
        {
            parser_seterror(parser, "Operator '%s' is not unary", OP_STR[op]);
            return NULL;
        }

        lexer_next(parser->lexer);

        if (lexer_skippedwhitespace(parser->lexer))
        {
            parser_seterror(parser, "Unary operator '%s' cannot have trailing whitespace", OP_STR[op]);
            return NULL;
        }

        Node* expr = parser_primaryexpr(parser);
        if (!expr)
        {
            return NULL;
        }

        // Create the unary node around the expression
        Node* unary = node_new(NODE_UNARY);
        unary->data.unary.op = op;
        unary->data.unary.expr = expr;
        return unary;
    }
    else
    {
        return parser_secondaryexpr(parser);
    }
}

Node* parser_secondaryexpr(
    Parser* parser
)
{
    TokenType type = lexer_gettokentype(parser->lexer);
    unsigned id = lexer_gettokenid(parser->lexer);

    Node* node = NULL;
    switch (type)
    {
    case TOK_SYMBOL:

        // Paren expression
        if (id == '(')
        {
            node = parser_parenexpr(parser);
            break;
        }

        // Map literal
        else if (id == '{')
        {
            node = parser_map(parser);
            break;
        }

        // Function literal
        else if (id == '[')
        {
            node = parser_function(parser);
            break;
        }

        else
        {
            unexpectedtokenerror(parser);
            break;
        }

    // Number literal
    case TOK_NUMBER:
        node = node_new(NODE_NUMBER);
        node->data.number.value = lexer_gettokenasnumber(parser->lexer);
        lexer_next(parser->lexer);
        break;

    // String literal or identifier
    case TOK_STRING:
    case TOK_IDENT:
        node = parser_stringorident(parser);
        break;

    default:
        unexpectedtokenerror(parser);
    }

    if (node && !lexer_skippedwhitespace(parser->lexer))
    {
        // Parse and trailing indexing operations
        for (;;)
        {
            // Check for bracket index
            if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '['))
            {
                lexer_next(parser->lexer);

                // Parse the expression in the brackets
                Node* key = parser_expr(parser);
                if (!key)
                {
                    node_free(node);
                    return NULL;
                }

                // Expect a closing bracket
                if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ']'))
                {
                    parser_seterror(parser, "Expected closing ']'");
                    node_free(key);
                    node_free(node);
                    return NULL;
                }
                lexer_next(parser->lexer);

                // Create the index node
                Node* index = node_new(NODE_INDEX);
                index->data.index.bracket = true;
                index->data.index.expr = node;
                index->data.index.key = key;

                node = index;
            }

            // Check for dot index
            else if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '.'))
            {
                lexer_next(parser->lexer);

                // Expect an identifier
                if (!lexer_istokentype(parser->lexer, TOK_IDENT))
                {
                    parser_seterror(parser, "Right side of '.' operation must be an identifier");
                    node_free(node);
                    return NULL;
                }

                // Parse the identifier
                Node* key = parser_stringorident(parser);
                if (!key)
                {
                    node_free(node);
                    return NULL;
                }

                // Use identifier as a string
                key->type = NODE_STRING;

                // Create the index node
                Node* index = node_new(NODE_INDEX);
                index->data.index.bracket = false;
                index->data.index.expr = node;
                index->data.index.key = key;

                node = index;
            }

            // Check for invocation
            else if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ':'))
            {
                lexer_next(parser->lexer);

                Node* expr = node;
                Node* args = node_new(NODE_SEQUENCE);

                // Create the invocation node
                node = node_new(NODE_INVOCATION);
                node->data.invocation.expr = expr;
                node->data.invocation.args = args;

                // Parse the argments
                if (lexer_skippedwhitespace(parser->lexer))
                {
                    while (!lexer_skippednewline(parser->lexer))
                    {
                        // Save the parser state
                        LexerState state = lexer_savestate(parser->lexer);

                        // Try to parse an argument
                        Node* arg = parser_primaryexpr(parser);
                        if (arg)
                        {
                            // Append the argument to the end of the invocation node's
                            // argument sequence
                            args->data.sequence.expr = arg;

                            // Create the next node
                            Node* next = node_new(NODE_SEQUENCE);
                            args->data.sequence.next = next;

                            // Move to the next node
                            args = next;
                        }
                        else
                        {
                            // Failed to parse an argument which indicates the end of
                            // argument list

                            // Restore the parser state
                            lexer_restorestate(parser->lexer, state);

                            break;
                        }
                    }
                }
            }
            else
            {
                break; // No more dot or bracket indices
            }
        }
    }

    return node;
}

Node* parser_parenexpr(
    Parser* parser
)
{
    if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '('))
    {
        parser_seterror(parser, "Expected opening '('");
        return NULL;
    }

    lexer_next(parser->lexer);

    Node* expr = parser_expr(parser);
    if (!expr)
    {
        return NULL;
    }

    if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ')'))
    {
        parser_seterror(parser, "Expected closing ')'");
        node_free(expr);
        return NULL;
    }

    lexer_next(parser->lexer);

    return expr;
}

Node* parser_binexpr(
    Parser* parser,
    int     prec,
    Node*   leftexpr
)
{
    if (!leftexpr)
    {
        return NULL;
    }

    for (;;)
    {
        Operator op = (Operator)lexer_gettokenid(parser->lexer);

        // Check that the current token is an operator
        if (!lexer_istokentype(parser->lexer, TOK_OPERATOR))
        {
            return leftexpr; // The current token is not an operator
        }

        // Get operator precedence and move to next token
        int opprec = OP_PREC[lexer_gettokenid(parser->lexer)];

        // This operator has less precedence
        if (opprec < prec)
        {
            return leftexpr;
        }

        // Move past the operator
        lexer_next(parser->lexer);

        // Parse the right-hand expression
        Node* rightexpr = parser_primaryexpr(parser);
        if (!rightexpr)
        {
            node_free(leftexpr);
            return NULL;
        }

        // Check if the token past the right-hand expression is an operator
        if (lexer_istokentype(parser->lexer, TOK_OPERATOR))
        {
            int nextopprec = OP_PREC[lexer_gettokenid(parser->lexer)];

            // If the current operator is of less precedence then parse the
            // next binary expression first
            if (opprec < nextopprec)
            {
                rightexpr = parser_binexpr(parser, opprec + 1, rightexpr);
                if (!rightexpr)
                {
                    node_free(leftexpr);
                    return NULL;
                }
            }
        }

        // Verify the binary operation is valid
        if (op == OP_LET && leftexpr->type != NODE_IDENT && !(leftexpr->type == NODE_INDEX && leftexpr->data.index.bracket == false))
        {
            parser_seterror(parser, "The left side of a ':=' expression must be an identifier");
            node_free(leftexpr);
            node_free(rightexpr);
            return NULL;
        }

        // Create the binary operation node
        Node* binary = node_new(NODE_BINARY);
        binary->data.binary.op = op;
        binary->data.binary.leftexpr = leftexpr;
        binary->data.binary.rightexpr = rightexpr;

        leftexpr = binary;
    }
}

Node* parser_map(
    Parser* parser
)
{
    if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '{'))
    {
        parser_seterror(parser, "Expected opening '{'");
        return NULL;
    }

    lexer_next(parser->lexer);

    Node* maproot = node_new(NODE_MAP);
    if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '}'))
    {
        lexer_next(parser->lexer);

        // An empty map
        return maproot;
    }

    // Parse the expressions in the map
    Node* exprs = parser_exprs(parser, false);
    if (!exprs)
    {
        return NULL;
    }
    else if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '}'))
    {
        parser_seterror(parser, "Expected closing '}'");
        node_free(maproot);
        node_free(exprs);
        return NULL;
    }

    lexer_next(parser->lexer);

    size_t i = 0;
    Node* map = maproot;
    Node* expr = exprs;
    while (expr)
    {
        Node* item = expr->data.sequence.expr;
        expr->data.sequence.expr = NULL;

        // If the association is a let operation then treat the left side as
        // a string
        if (item->type == NODE_BINARY && item->data.binary.op == OP_LET)
        {
            item->data.binary.op = OP_ASSOC;
            item->data.binary.leftexpr->type = NODE_STRING;
        }

        // Infer the key if it is not an association operation
        if (item->type != NODE_BINARY && item->data.binary.op != OP_ASSOC)
        {
            Node* key = node_new(NODE_NUMBER);
            key->data.number.value = (double)i;

            // Create an association operation
            Node* assoc = node_new(NODE_BINARY);
            assoc->data.binary.op = OP_ASSOC;
            assoc->data.binary.leftexpr = key;
            assoc->data.binary.rightexpr = item;

            item = assoc;
        }

        map->data.map.assoc = item;

        expr = expr->data.sequence.next;
        if (expr)
        {
            Node* nextmap = node_new(NODE_MAP);
            nextmap->data.map.prev = map;
            map->data.map.next = nextmap;
            map = nextmap;
        }

        ++i;
    }

    node_free(exprs);
    return maproot;
}

Node* parser_function(
    Parser* parser
)
{
    if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '['))
    {
        parser_seterror(parser, "Expected opening '['");
        return NULL;
    }

    lexer_next(parser->lexer);

    LexerState state = lexer_savestate(parser->lexer);

    Node* params = node_new(NODE_SEQUENCE);
    Node* param = params;
    for (;;)
    {
        Node* ident = parser_stringorident(parser);
        if (!ident || ident->type != NODE_IDENT)
        {
            if (ident)
            {
                node_free(ident);
                ident = NULL;
            }

            node_free(params);
            params = NULL;

            // Failed to parse the parametesr
            break;
        }

        param->data.sequence.expr = ident;

        if (lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, '|'))
        {
            lexer_next(parser->lexer);
            break;
        }

        param->data.sequence.next = node_new(NODE_SEQUENCE);
        param = param->data.sequence.next;
    }

    // Restart at the beginning if the parameter parsing failed
    if (!params)
    {
        lexer_restorestate(parser->lexer, state);
    }

    Node* exprs = parser_exprs(parser, true);
    if (!exprs)
    {
        return NULL;
    }

    if (!lexer_istokentypeandid(parser->lexer, TOK_SYMBOL, ']'))
    {
        parser_seterror(parser, "Expected closing ']'");
        node_free(exprs);
        return NULL;
    }
    lexer_next(parser->lexer);

    Node* function = node_new(NODE_FUNCTION);
    function->data.function.params = params;
    function->data.function.exprs = exprs;
    return function;
}

Node* parser_stringorident(
    Parser* parser
)
{
    Node* node;
    if (lexer_istokentype(parser->lexer, TOK_STRING))
    {
        node = node_new(NODE_STRING);
    }
    else if (lexer_istokentype(parser->lexer, TOK_IDENT))
    {
        node = node_new(NODE_IDENT);
    }
    else
    {
        parser_seterror(parser, "Expected a string or identifier");
        return NULL;
    }

    size_t length = lexer_gettokenlength(parser->lexer);
    const char* string = lexer_gettokenstring(parser->lexer);
    StringId id = stringpool_getidsubstring(parser->stringpool, string, length);
    node->data.string.id = id;

    lexer_next(parser->lexer);

    return node;
}

static void unexpectedtokenerror(
    Parser*     parser
)
{
    char buffer[MAX_PARSER_ERROR_LENGTH];
    size_t length = lexer_gettokenlength(parser->lexer);
    const char* string = lexer_gettokenstring(parser->lexer);
    memcpy(buffer, string, length);
    buffer[length] = '\0';
    parser_seterror(parser, "Unexpected token '%s'", buffer);
}
