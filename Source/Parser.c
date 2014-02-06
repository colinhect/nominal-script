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
    Lexer* l;
    StringPool* s;
    char error[256];
} Parser;

Parser* Parser_Create(const char* source, StringPool* stringPool)
{
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = Lexer_Create(source);
    p->s = stringPool;
    Lexer_Next(p->l);
    return p;
}

void Parser_Free(Parser* p)
{
    free(p->l);
    free(p);
}

void Parser_SetError(Parser* p, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(p->error, fmt, args);
    va_end(args);
}

const char* Parser_GetError(Parser* p)
{
    return p->error;
}

Node* Parser_Expr(Parser* p)
{
    return Parser_BinExpr(p, 0, Parser_PrimaryExpr(p));
}

Node* Parser_PrimaryExpr(Parser* p)
{
    // Unary operator
    if (Lexer_IsTokenType(p->l, TOK_OPERATOR))
    {
        OpCode  op;
        Node*   n;
        
        op = (OpCode)Lexer_GetTokenId(p->l);
        if (op == OP_SUB)
        {
            op = OP_NEG;
        }

        if (op != OP_NOT && op != OP_NEG)
        {
            Parser_SetError(p, "Operator '%s' is not unary", OP_STR[op]);
            return NULL;
        }

        Lexer_Next(p->l);

        n = Parser_PrimaryExpr(p);
        if (!n)
        {
            return NULL;
        }

        return Node_WithInteger(NODE_UNARY_OP, op, n, NULL);
    }
    else
    {
        return Parser_SecondaryExpr(p);
    }
}

Node* Parser_SecondaryExpr(Parser* p)
{
    Node*       n;
    TokenType   type;
    unsigned    id;

    type = Lexer_GetTokenType(p->l);
    id = Lexer_GetTokenId(p->l);

    switch (type)
    {
    case TOK_SYMBOL:
        if (id == '(')
        {
            n = Parser_ParenExpr(p);
            break;
        }
    case TOK_INTEGER:
        n = Node_WithInteger(NODE_INTEGER, Lexer_GetTokenAsInt(p->l), NULL, NULL);
        Lexer_Next(p->l);
        break;
    case TOK_REAL:
        n = Node_WithReal(NODE_REAL, Lexer_GetTokenAsFloat(p->l), NULL, NULL);
        Lexer_Next(p->l);
        break;
    case TOK_STRING:
        {
            size_t length = Lexer_GetTokenLength(p->l);
            const char* string = Lexer_GetTokenString(p->l);
            StringId id = StringPool_InsertOrFindSubString(p->s, string, length);
            n = Node_WithHandle(NODE_STRING, (unsigned)id, NULL, NULL);
            Lexer_Next(p->l);
        } break;
    case TOK_IDENT:
        {
            size_t length = Lexer_GetTokenLength(p->l);
            const char* string = Lexer_GetTokenString(p->l);
            StringId id = StringPool_InsertOrFindSubString(p->s, string, length);
            n = Node_WithHandle(NODE_IDENT, (unsigned)id, NULL, NULL);
            Lexer_Next(p->l);
        } break;
    case TOK_KEYWORD:
        if (id == KW_NIL)
        {
            n = Node_WithoutData(NODE_NIL, NULL, NULL);
            Lexer_Next(p->l);
            break;
        }
    default:
        {
            char buffer[64];
            size_t length = Lexer_GetTokenLength(p->l);
            const char* string = Lexer_GetTokenString(p->l);
            memcpy(buffer, string, length);
            buffer[length] = '\0';
            Parser_SetError(p, "Unexpected token '%s'", buffer);
            return NULL;
        }
    }

    return n;
}

Node* Parser_ParenExpr(Parser* p)
{
    Node* n;

    if (!Lexer_IsTokenTypeAndId(p->l, TOK_SYMBOL, '('))
    {
        Parser_SetError(p, "Expected opening '('");
        return NULL;
    }

    Lexer_Next(p->l);

    n = Parser_Expr(p);
    if (!n)
    {
        return NULL;
    }

    if (!Lexer_IsTokenTypeAndId(p->l, TOK_SYMBOL, ')'))
    {
        Node_Free(n);
        Parser_SetError(p, "Expected closing ')'");
        return NULL;
    }

    Lexer_Next(p->l);

    return n;
}

Node* Parser_BinExpr(Parser* p, int prec, Node* left)
{
    if (!left)
    {
        return NULL;
    }

    for (;;)
    {
        OpCode  op;
        Node*   right;
        int     opPrec;
        int     nextOpPrec;
        OpCode  nodeOp = (OpCode)Lexer_GetTokenId(p->l);

        opPrec = Lexer_IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[Lexer_GetTokenId(p->l)] : -1;
        if (opPrec < prec)
        {
            return left;
        }

        op = (OpCode)Lexer_GetTokenId(p->l);
        Lexer_Next(p->l);
        right = Parser_PrimaryExpr(p);
        if (!right)
        {
            Node_Free(left);
            return NULL;
        }

        nextOpPrec = Lexer_IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[Lexer_GetTokenId(p->l)] : -1;
        if (opPrec < nextOpPrec)
        {
            right = Parser_BinExpr(p, opPrec + 1, right);
            if (!right)
            {
                Node_Free(left);
                return NULL;
            }
        }

        left = Node_WithInteger(NODE_BINARY_OP, nodeOp, left, right);
    }
}