#include "Parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

Parser* CreateParser(const char* source)
{
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = CreateLexer(source);
    MoveNext(p->l);
    return p;
}

void FreeParser(Parser* p)
{
    free(p->l);
    free(p);
}

void SetParseError(Parser* p, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(p->error, fmt, args);
    va_end(args);
}

Node* ParseExpr(Parser* p)
{
    return ParseBinExpr(p, 0, ParsePrimaryExpr(p));
}

Node* ParsePrimaryExpr(Parser* p)
{
    // Unary operator
    if (IsTokenType(p->l, TOK_OPERATOR))
    {
        NodeData    data = { 0 };
        op_code     op;
        Node*       n;
        
        op = (op_code)GetTokenId(p->l);
        if (op == OP_SUB)
        {
            op = OP_NEG;
        }

        if (op != OP_NOT && op != OP_NEG)
        {
            SetParseError(p, "Operator '%s' is not unary", OP_STR[op]);
            return NULL;
        }

        data.integerValue = op;
        MoveNext(p->l);

        n = ParsePrimaryExpr(p);
        if (!n)
        {
            return NULL;
        }

        return CreateNode(NODE_UNARY_OP, n, NULL, data);
    }
    else
    {
        return ParseSecondaryExpr(p);
    }
}

Node* ParseSecondaryExpr(Parser* p)
{
    Node*       n;
    NodeData   data = { 0 };
    TokenType  type;
    unsigned    id;

    type = GetTokenType(p->l);
    id = GetTokenId(p->l);

    switch (type)
    {
    case TOK_SYMBOL:
        if (id == '(')
        {
            n = ParseParenExpr(p);
            break;
        }
    case TOK_INTEGER:
        data.integerValue = GetTokenAsInt(p->l);
        n = CreateNode(NODE_INTEGER, NULL, NULL, data);
        MoveNext(p->l);
        break;
    case TOK_FLOAT:
        data.realValue = GetTokenAsFloat(p->l);
        n = CreateNode(NODE_REAL, NULL, NULL, data);
        MoveNext(p->l);
        break;
    case TOK_KEYWORD:
        if (id == KW_NIL)
        {
            n = CreateNode(NODE_NIL, NULL, NULL, data);
            MoveNext(p->l);
            break;
        }
    default:
        {
            char buffer[64];
            CopyTokenValue(p->l, buffer);
            SetParseError(p, "Unexpected token '%s'", buffer);
            return NULL;
        }
    }

    return n;
}

Node* ParseParenExpr(Parser* p)
{
    Node* n;

    if (!IsTokenTypeAndId(p->l, TOK_SYMBOL, '('))
    {
        SetParseError(p, "Expected opening '('");
        return NULL;
    }

    MoveNext(p->l);

    n = ParseExpr(p);
    if (!n)
    {
        return NULL;
    }

    if (!IsTokenTypeAndId(p->l, TOK_SYMBOL, ')'))
    {
        FreeNode(n);
        SetParseError(p, "Expected closing ')'");
        return NULL;
    }

    MoveNext(p->l);

    return n;
}

Node* ParseBinExpr(Parser* p, int prec, Node* left)
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

        data.integerValue = GetTokenId(p->l);

        op_prec = IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[GetTokenId(p->l)] : -1;
        if (op_prec < prec)
        {
            return left;
        }

        op = (op_code)GetTokenId(p->l);
        MoveNext(p->l);
        right = ParsePrimaryExpr(p);
        if (!right)
        {
            FreeNode(left);
            return NULL;
        }

        next_op_prec = IsTokenType(p->l, TOK_OPERATOR) ? OP_PREC[GetTokenId(p->l)] : -1;
        if (op_prec < next_op_prec)
        {
            right = ParseBinExpr(p, op_prec + 1, right);
            if (!right)
            {
                FreeNode(left);
                return NULL;
            }
        }

        left = CreateNode(NODE_BINARY_OP, left, right, data);
    }
}