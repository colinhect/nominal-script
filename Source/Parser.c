#include "Parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef struct _Parser
{
    Lexer* l;
    char error[256];
} Parser;

Parser* Parser_Create(const char* source)
{
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = Lexer_Create(source);
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
        NodeData    data = { 0 };
        op_code     op;
        Node*       n;
        
        op = (op_code)Lexer_GetTokenId(p->l);
        if (op == OP_SUB)
        {
            op = OP_NEG;
        }

        if (op != OP_NOT && op != OP_NEG)
        {
            Parser_SetError(p, "Operator '%s' is not unary", OP_STR[op]);
            return NULL;
        }

        data.integerValue = op;
        Lexer_Next(p->l);

        n = Parser_PrimaryExpr(p);
        if (!n)
        {
            return NULL;
        }

        return Node_Create(NODE_UNARY_OP, n, NULL, data);
    }
    else
    {
        return Parser_SecondaryExpr(p);
    }
}

Node* Parser_SecondaryExpr(Parser* p)
{
    Node*       n;
    NodeData    data = { 0 };
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
        data.integerValue = Lexer_GetTokenAsInt(p->l);
        n = Node_Create(NODE_INTEGER, NULL, NULL, data);
        Lexer_Next(p->l);
        break;
    case TOK_FLOAT:
        data.realValue = Lexer_GetTokenAsFloat(p->l);
        n = Node_Create(NODE_REAL, NULL, NULL, data);
        Lexer_Next(p->l);
        break;
    case TOK_STRING:
        {
            size_t length = Lexer_GetTokenLength(p->l);
            data.stringValue = (char*)malloc(sizeof(char*) * (length + 1));
            Lexer_CopyTokenValue(p->l, data.stringValue);
            n = Node_Create(NODE_STRING, NULL, NULL, data);
            Lexer_Next(p->l);
        } break;
    case TOK_KEYWORD:
        if (id == KW_NIL)
        {
            n = Node_Create(NODE_NIL, NULL, NULL, data);
            Lexer_Next(p->l);
            break;
        }
    default:
        {
            char buffer[64];
            Lexer_CopyTokenValue(p->l, buffer);
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
        op_code op;
        Node* right;
        NodeData data;
        int op_prec;
        int next_op_prec;

        data.integerValue = Lexer_GetTokenId(p->l);

        op_prec = Lexer_IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[Lexer_GetTokenId(p->l)] : -1;
        if (op_prec < prec)
        {
            return left;
        }

        op = (op_code)Lexer_GetTokenId(p->l);
        Lexer_Next(p->l);
        right = Parser_PrimaryExpr(p);
        if (!right)
        {
            Node_Free(left);
            return NULL;
        }

        next_op_prec = Lexer_IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[Lexer_GetTokenId(p->l)] : -1;
        if (op_prec < next_op_prec)
        {
            right = Parser_BinExpr(p, op_prec + 1, right);
            if (!right)
            {
                Node_Free(left);
                return NULL;
            }
        }

        left = Node_Create(NODE_BINARY_OP, left, right, data);
    }
}