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

        Node* n = Parser_PrimaryExpr(parser);
        if (!n)
        {
            return NULL;
        }

        return Node_WithInteger(NODE_UNARY_OP, op, n, NULL);
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
        if (id == '(')
        {
            n = Parser_ParenExpr(parser);
            break;
        }
    case TOK_INTEGER:
        n = Node_WithInteger(NODE_INTEGER, Lexer_GetTokenAsInteger(parser->lexer), NULL, NULL);
        Lexer_Next(parser->lexer);
        break;
    case TOK_REAL:
        n = Node_WithReal(NODE_REAL, Lexer_GetTokenAsReal(parser->lexer), NULL, NULL);
        Lexer_Next(parser->lexer);
        break;
    case TOK_STRING:
        {
            size_t length = Lexer_GetTokenLength(parser->lexer);
            const char* string = Lexer_GetTokenString(parser->lexer);
            StringId id = StringPool_InsertOrFindSubString(parser->stringPool, string, length);
            n = Node_WithHandle(NODE_STRING, (unsigned)id, NULL, NULL);
            Lexer_Next(parser->lexer);
        } break;
    case TOK_IDENT:
        {
            size_t length = Lexer_GetTokenLength(parser->lexer);
            const char* string = Lexer_GetTokenString(parser->lexer);
            StringId id = StringPool_InsertOrFindSubString(parser->stringPool, string, length);
            n = Node_WithHandle(NODE_IDENT, (unsigned)id, NULL, NULL);
            Lexer_Next(parser->lexer);
        } break;
    case TOK_KEYWORD:
        if (id == KW_NIL)
        {
            n = Node_WithoutData(NODE_NIL, NULL, NULL);
            Lexer_Next(parser->lexer);
            break;
        }
    default:
        {
            char buffer[64];
            size_t length = Lexer_GetTokenLength(parser->lexer);
            const char* string = Lexer_GetTokenString(parser->lexer);
            memcpy(buffer, string, length);
            buffer[length] = '\0';
            Parser_SetError(parser, "Unexpected token '%s'", buffer);
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

        OpCode op = (OpCode)Lexer_GetTokenId(parser->lexer);
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

        left = Node_WithInteger(NODE_BINARY_OP, nodeOp, left, right);
    }
}