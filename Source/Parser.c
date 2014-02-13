///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2014 Colin Hill
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef struct _Parser
{
    Lexer*      lexer;
    StringPool* stringPool;
    char        error[256];
} Parser;

void ErrorUnexpectedToken(
    Parser*     parser
    )
{
    char buffer[128];
    size_t length = Lexer_GetTokenLength(parser->lexer);
    const char* string = Lexer_GetTokenString(parser->lexer);
    memcpy(buffer, string, length);
    buffer[length] = '\0';
    Parser_SetError(parser, "Unexpected token '%s'", buffer);
}

Parser* Parser_Create(
    const char* source,
    StringPool* stringPool
    )
{
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = Lexer_Create(source);
    parser->stringPool = stringPool;
    Lexer_Next(parser->lexer);
    return parser;
}

void Parser_Free(
    Parser* parser
    )
{
    free(parser->lexer);
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

    // If there is a trailing comma, recursively parse another expression
    // sequence
    Node* exprs = NULL;
    if (Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ','))
    {
        Lexer_Next(parser->lexer);
        exprs = Parser_Exprs(parser);
    }

    Node* n = Node_Create(NODE_SEQUENCE);
    n->data.sequence.node = expr;
    n->data.sequence.next = exprs;
    return n;
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
        OpCode op = (OpCode)Lexer_GetTokenId(parser->lexer);
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

        Node* expr = Parser_PrimaryExpr(parser);
        if (!expr)
        {
            return NULL;
        }

        Node* unary = Node_Create(NODE_UNARY);
        unary->data.unary.op = op;
        unary->data.unary.node = expr;
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

    Node* n;
    switch (type)
    {
    case TOK_SYMBOL:

        // Paren expression
        if (id == '(')
        {
            n = Parser_ParenExpr(parser);
            break;
        }

        // Map literal
        else if (id == '{')
        {
            Lexer_Next(parser->lexer);

            n = Parser_Exprs(parser);
            if (!n)
            {
                return NULL;
            }
            else if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, '}'))
            {
                Parser_SetError(parser, "Expected closing '}'");
                return NULL;
            }

            size_t i = 0;
            Node* map = n;
            while (map)
            {
                Node* item = map->data.sequence.node;

                // Infer the key if it is not a association operation
                if (item->type != NODE_BINARY && item->data.binary.op != OP_ASSOC)
                {
                    Node* key = Node_Create(NODE_INTEGER);
                    key->data.integer.value = i;

                    // Create an association operation
                    Node* assoc = Node_Create(NODE_BINARY);
                    assoc->data.binary.op = OP_ASSOC;
                    assoc->data.binary.left = key;
                    assoc->data.binary.right = item;

                    map->data.sequence.node = assoc;
                }

                map->type = NODE_MAP;
                map = map->data.sequence.next;
                ++i;
            }
            break;
        }

    // Integer literal
    case TOK_INTEGER:
        n = Node_Create(NODE_INTEGER);
        n->data.integer.value = Lexer_GetTokenAsInteger(parser->lexer);
        Lexer_Next(parser->lexer);
        break;

    // Real literal
    case TOK_REAL:
        n = Node_Create(NODE_REAL);
        n->data.real.value = Lexer_GetTokenAsReal(parser->lexer);
        Lexer_Next(parser->lexer);
        break;

    // String literal
    case TOK_STRING:
        {
            size_t length = Lexer_GetTokenLength(parser->lexer);
            const char* string = Lexer_GetTokenString(parser->lexer);
            StringId id = StringPool_InsertOrFindSubString(parser->stringPool, string, length);

            n = Node_Create(NODE_STRING);
            n->data.string.id = id;

            Lexer_Next(parser->lexer);
        } break;

    // Identifier
    case TOK_IDENT:
        {
            size_t length = Lexer_GetTokenLength(parser->lexer);
            const char* string = Lexer_GetTokenString(parser->lexer);
            StringId id = StringPool_InsertOrFindSubString(parser->stringPool, string, length);

            n = Node_Create(NODE_IDENT);
            n->data.string.id = id;

            Lexer_Next(parser->lexer);
        } break;

    // Keyword literals
    case TOK_KEYWORD:
        if (id == KW_NIL)
        {
            n = Node_Create(NODE_NIL);
            Lexer_Next(parser->lexer);
            break;
        }

    default:
        {
            ErrorUnexpectedToken(parser);
            return NULL;
        }
    }

    return n;
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

    Node* n = Parser_Expr(parser);
    if (!n)
    {
        return NULL;
    }

    if (!Lexer_IsTokenTypeAndId(parser->lexer, TOK_SYMBOL, ')'))
    {
        Node_Free(n);
        Parser_SetError(parser, "Expected closing ')'");
        return NULL;
    }

    Lexer_Next(parser->lexer);

    return n;
}

Node* Parser_BinExpr(
    Parser* parser,
    int     prec,
    Node*   left
    )
{
    if (!left)
    {
        return NULL;
    }

    for (;;)
    {
        OpCode nodeOp = (OpCode)Lexer_GetTokenId(parser->lexer);
        int opPrec = Lexer_IsTokenType(parser->lexer, TOK_OPERATOR)
            ? OP_PREC[Lexer_GetTokenId(parser->lexer)]
            : -1;

        if (opPrec < prec)
        {
            return left;
        }

        Lexer_Next(parser->lexer);
        Node* right = Parser_PrimaryExpr(parser);
        if (!right)
        {
            Node_Free(left);
            return NULL;
        }

        int nextOpPrec = Lexer_IsTokenType(parser->lexer, TOK_OPERATOR)
            ? OP_PREC[Lexer_GetTokenId(parser->lexer)]
            : -1;

        if (opPrec < nextOpPrec)
        {
            right = Parser_BinExpr(parser, opPrec + 1, right);
            if (!right)
            {
                Node_Free(left);
                return NULL;
            }
        }

        if (nodeOp == OP_LET && left->type != NODE_IDENT)
        {
            Parser_SetError(parser, "The left side of a let operation must be an identifier");
            Node_Free(left);
            Node_Free(right);
            return NULL;
        }

        Node* newLeft = Node_Create(NODE_BINARY);
        newLeft->data.binary.op = nodeOp;
        newLeft->data.binary.left = left;
        newLeft->data.binary.right = right;
        left = newLeft;
    }
}