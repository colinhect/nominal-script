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
#include "Parser.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_PARSER_ERROR_LENGTH (256)

struct Parser
{
    Lexer*      lexer;
    StringPool* stringPool;
    char        error[MAX_PARSER_ERROR_LENGTH];
};

void SetUnexpectedTokenError(
    Parser*     parser
    )
{
    char buffer[MAX_PARSER_ERROR_LENGTH];
    size_t length = Lexer_GetTokenLength(parser->lexer);
    const char* string = Lexer_GetTokenString(parser->lexer);
    memcpy(buffer, string, length);
    buffer[length] = '\0';
    Parser_SetError(parser, "Unexpected token '%s'", buffer);
}

Parser* Parser_New(
    const char* source,
    StringPool* stringPool
    )
{
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    assert(parser);

    parser->lexer = Lexer_New(source);
    parser->stringPool = stringPool;

    // Move to the first token
    Lexer_Next(parser->lexer);

    return parser;
}

void Parser_Free(
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

void Parser_SetError(
    Parser*     parser,
    const char* fmt,
    ...
    )
{
    va_list args;
    va_start(args, fmt);
    vsprintf(parser->error, fmt, args);
    va_end(args);
}

const char* Parser_GetError(
    Parser* parser
    )
{
    return parser->error;
}

Node* Parser_Exprs(
    Parser* parser
    )
{
    // Parse an expression
    Node* expr = Parser_Expr(parser);
    if (!expr)
    {
        return NULL;
    }

    // If there is a trailing comma then recursively parse another expression
    // sequence
    Node* exprs = NULL;
    if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ','))
    {
        Lexer_Next(parser->lexer);
        exprs = Parser_Exprs(parser);
        if (!exprs)
        {
            Node_Free(expr);
            return NULL;
        }
    }

    // Create a sequence node for this expression and the potential expressions
    // after it
    Node* sequence = Node_New(NODE_SEQUENCE);
    sequence->data.sequence.expr = expr;
    sequence->data.sequence.next = exprs;
    return sequence;
}

Node* Parser_Expr(
    Parser* parser
    )
{
    return Parser_BinExpr(parser, 0, Parser_PrimaryExpr(parser));
}

Node* Parser_PrimaryExpr(
    Parser* parser
    )
{
    // Unary operator
    if (Lexer_IsTokenType(parser->lexer, TOK_OPERATOR))
    {
        Operator op = (Operator)Lexer_GetTokenId(parser->lexer);
        if (op == OP_SUB)
        {
            op = OP_NEG;
        }

        if (op != OP_NOT && op != OP_NEG)
        {
            Parser_SetError(parser, "Operator '%s' is not unary", OP_STR[op]);
            return NULL;
        }

        Lexer_Next(parser->lexer);

        if (Lexer_SkippedWhitespace(parser->lexer))
        {
            Parser_SetError(parser, "Unary operator '%s' cannot have trailing whitespace", OP_STR[op]);
            return NULL;
        }

        Node* expr = Parser_PrimaryExpr(parser);
        if (!expr)
        {
            return NULL;
        }

        // Create the unary node around the expression
        Node* unary = Node_New(NODE_UNARY);
        unary->data.unary.op = op;
        unary->data.unary.expr = expr;
        return unary;
    }
    else
    {
        return Parser_SecondaryExpr(parser);
    }
}

Node* Parser_SecondaryExpr(
    Parser* parser
    )
{
    TokenType type = Lexer_GetTokenType(parser->lexer);
    unsigned id = Lexer_GetTokenId(parser->lexer);

    Node* node = NULL;
    switch (type)
    {
    case TOK_SYMBOL:

        // Paren expression
        if (id == '(')
        {
            node = Parser_ParenExpr(parser);
            break;
        }

        // Map literal
        else if (id == '{')
        {
            node = Parser_Map(parser);
            break;
        }

        // Function literal
        else if (id == '[')
        {
            node = Parser_Function(parser);
            break;
        }

        else
        {
            SetUnexpectedTokenError(parser);
            break;
        }

        // Number literal
    case TOK_NUMBER:
        node = Node_New(NODE_NUMBER);
        node->data.number.value = Lexer_GetTokenAsNumber(parser->lexer);
        Lexer_Next(parser->lexer);
        break;

        // String literal or identifier
    case TOK_STRING:
    case TOK_IDENT:
        node = Parser_StringOrIdent(parser);
        break;

    default:
        SetUnexpectedTokenError(parser);
    }

    if (node && !Lexer_SkippedWhitespace(parser->lexer))
    {
        // Parse and trailing indexing operations
        for (;;)
        {
            // Check for bracket index
            if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '['))
            {
                Lexer_Next(parser->lexer);

                // Parse the expression in the brackets
                Node* key = Parser_Expr(parser);
                if (!key)
                {
                    Node_Free(node);
                    return NULL;
                }

                // Expect a closing bracket
                if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ']'))
                {
                    Parser_SetError(parser, "Expected closing ']'");
                    Node_Free(key);
                    Node_Free(node);
                    return NULL;
                }
                Lexer_Next(parser->lexer);

                // Create the index node
                Node* index = Node_New(NODE_INDEX);
                index->data.index.bracket = true;
                index->data.index.expr = node;
                index->data.index.key = key;

                node = index;
            }

            // Check for dot index
            else if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '.'))
            {
                Lexer_Next(parser->lexer);

                // Expect an identifier
                if (!Lexer_IsTokenType(parser->lexer, TOK_IDENT))
                {
                    Parser_SetError(parser, "Right side of '.' operation must be an identifier");
                    Node_Free(node);
                    return NULL;
                }

                // Parse the identifier
                Node* key = Parser_StringOrIdent(parser);
                if (!key)
                {
                    Node_Free(node);
                    return NULL;
                }

                // Use identifier as a string
                key->type = NODE_STRING;

                // Create the index node
                Node* index = Node_New(NODE_INDEX);
                index->data.index.bracket = false;
                index->data.index.expr = node;
                index->data.index.key = key;

                node = index;
            }

            // Check for invocation
            else if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ':'))
            {
                Lexer_Next(parser->lexer);

                Node* expr = node;
                Node* args = Node_New(NODE_SEQUENCE);

                // Create the invocation node
                node = Node_New(NODE_INVOCATION);
                node->data.invocation.expr = expr;
                node->data.invocation.args = args;

                // Parse the argments
                if (Lexer_SkippedWhitespace(parser->lexer))
                {
                    for (;;)
                    {
                        // Save the parser state
                        LexerState state = Lexer_SaveState(parser->lexer);

                        // Try to parse an argument
                        Node* arg = Parser_PrimaryExpr(parser);
                        if (arg)
                        {
                            // Append the argument to the end of the invocation node's
                            // argument sequence
                            args->data.sequence.expr = arg;

                            // Create the next node
                            Node* next = Node_New(NODE_SEQUENCE);
                            args->data.sequence.next = next;

                            // Move to the next node
                            args = next;
                        }
                        else
                        {
                            // Failed to parse an argument which indicates the end of
                            // argument list

                            // Restore the parser state
                            Lexer_RestoreState(parser->lexer, state);

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

Node* Parser_ParenExpr(
    Parser* parser
    )
{
    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '('))
    {
        Parser_SetError(parser, "Expected opening '('");
        return NULL;
    }

    Lexer_Next(parser->lexer);

    Node* expr = Parser_Expr(parser);
    if (!expr)
    {
        return NULL;
    }

    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ')'))
    {
        Parser_SetError(parser, "Expected closing ')'");
        Node_Free(expr);
        return NULL;
    }

    Lexer_Next(parser->lexer);

    return expr;
}

Node* Parser_BinExpr(
    Parser* parser,
    int     prec,
    Node*   leftExpr
    )
{
    if (!leftExpr)
    {
        return NULL;
    }

    for (;;)
    {
        Operator op = (Operator)Lexer_GetTokenId(parser->lexer);

        // Check that the current token is an operator
        if (!Lexer_IsTokenType(parser->lexer, TOK_OPERATOR))
        {
            return leftExpr; // The current token is not an operator
        }

        // Get operator precedence and move to next token
        int opPrec = OP_PREC[Lexer_GetTokenId(parser->lexer)];

        // This operator has less precedence
        if (opPrec < prec)
        {
            return leftExpr;
        }

        // Move past the operator
        Lexer_Next(parser->lexer);

        // Parse the right-hand expression
        Node* rightExpr = Parser_PrimaryExpr(parser);
        if (!rightExpr)
        {
            Node_Free(leftExpr);
            return NULL;
        }

        // Check if the token past the right-hand expression is an operator
        if (Lexer_IsTokenType(parser->lexer, TOK_OPERATOR))
        {
            int nextOpPrec = OP_PREC[Lexer_GetTokenId(parser->lexer)];

            // If the current operator is of less precedence then parse the
            // next binary expression first
            if (opPrec < nextOpPrec)
            {
                rightExpr = Parser_BinExpr(parser, opPrec + 1, rightExpr);
                if (!rightExpr)
                {
                    Node_Free(leftExpr);
                    return NULL;
                }
            }
        }

        // Verify the binary operation is valid
        if (op == OP_LET && leftExpr->type != NODE_IDENT && !(leftExpr->type == NODE_INDEX && leftExpr->data.index.bracket == false))
        {
            Parser_SetError(parser, "The left side of a ':=' expression must be an identifier");
            Node_Free(leftExpr);
            Node_Free(rightExpr);
            return NULL;
        }

        // Create the binary operation node
        Node* binary = Node_New(NODE_BINARY);
        binary->data.binary.op = op;
        binary->data.binary.leftExpr = leftExpr;
        binary->data.binary.rightExpr = rightExpr;

        leftExpr = binary;
    }
}

Node* Parser_Map(
    Parser* parser
    )
{
    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '{'))
    {
        Parser_SetError(parser, "Expected opening '{'");
        return NULL;
    }

    Lexer_Next(parser->lexer);

    Node* mapRoot = Node_New(NODE_MAP);
    if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '}'))
    {
        Lexer_Next(parser->lexer);

        // An empty map
        return mapRoot;
    }

    // Parse the expressions in the map
    Node* exprs = Parser_Exprs(parser);
    if (!exprs)
    {
        return NULL;
    }
    else if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '}'))
    {
        Parser_SetError(parser, "Expected closing '}'");
        Node_Free(mapRoot);
        Node_Free(exprs);
        return NULL;
    }

    Lexer_Next(parser->lexer);

    size_t i = 0;
    Node* map = mapRoot;
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
            item->data.binary.leftExpr->type = NODE_STRING;
        }

        // Infer the key if it is not an association operation
        if (item->type != NODE_BINARY && item->data.binary.op != OP_ASSOC)
        {
            Node* key = Node_New(NODE_NUMBER);
            key->data.number.value = (double)i;

            // Create an association operation
            Node* assoc = Node_New(NODE_BINARY);
            assoc->data.binary.op = OP_ASSOC;
            assoc->data.binary.leftExpr = key;
            assoc->data.binary.rightExpr = item;

            item = assoc;
        }

        map->data.map.assoc = item;

        expr = expr->data.sequence.next;
        if (expr)
        {
            Node* nextMap = Node_New(NODE_MAP);
            nextMap->data.map.prev = map;
            map->data.map.next = nextMap;
            map = nextMap;
        }

        ++i;
    }

    Node_Free(exprs);
    return mapRoot;
}

Node* Parser_Function(
    Parser* parser
    )
{
    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '['))
    {
        Parser_SetError(parser, "Expected opening '['");
        return NULL;
    }

    Lexer_Next(parser->lexer);

    LexerState state = Lexer_SaveState(parser->lexer);

    Node* params = Node_New(NODE_SEQUENCE);
    Node* param = params;
    for (;;)
    {
        Node* ident = Parser_StringOrIdent(parser);
        if (!ident || ident->type != NODE_IDENT)
        {
            if (ident)
            {
                Node_Free(ident);
                ident = NULL;
            }

            Node_Free(params);
            params = NULL;

            // Failed to parse the parametesr
            break;
        }

        param->data.sequence.expr = ident;

        if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '|'))
        {
            Lexer_Next(parser->lexer);
            break;
        }

        param->data.sequence.next = Node_New(NODE_SEQUENCE);
        param = param->data.sequence.next;
    }

    // Restart at the beginning if the parameter parsing failed
    if (!params)
    {
        Lexer_RestoreState(parser->lexer, state);
    }

    // Parse the body of the function
    Node* exprs = Parser_Exprs(parser);
    if (!exprs)
    {
        return NULL;
    }

    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ']'))
    {
        Parser_SetError(parser, "Expected closing ']'");
        Node_Free(exprs);
        return NULL;
    }
    Lexer_Next(parser->lexer);

    Node* function = Node_New(NODE_FUNCTION);
    function->data.function.params = params;
    function->data.function.exprs = exprs;
    return function;
}

Node* Parser_StringOrIdent(
    Parser* parser
    )
{
    Node* node;
    if (Lexer_IsTokenType(parser->lexer, TOK_STRING))
    {
        node = Node_New(NODE_STRING);
    }
    else if (Lexer_IsTokenType(parser->lexer, TOK_IDENT))
    {
        node = Node_New(NODE_IDENT);
    }
    else
    {
        Parser_SetError(parser, "Expected a string or identifier");
        return NULL;
    }

    size_t length = Lexer_GetTokenLength(parser->lexer);
    const char* string = Lexer_GetTokenString(parser->lexer);
    StringId id = StringPool_InsertOrFindSubString(parser->stringPool, string, length);
    node->data.string.id = id;

    Lexer_Next(parser->lexer);

    return node;
}
