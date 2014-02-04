#pragma once

#include "Node.h"
#include "ByteCode.h"
#include "Lexer.h"

typedef struct
{
    Lexer* l;
    char error[256];
} Parser;

Parser* CreateParser(const char* source);
void FreeParser(Parser* p);

void SetParseError(Parser* p, const char* fmt, ...);

Node* ParseExpr(Parser* p);
Node* ParsePrimaryExpr(Parser* p);
Node* ParseSecondaryExpr(Parser* p);
Node* ParseParenExpr(Parser* p);
Node* ParseBinExpr(Parser* p, int prec, Node* left);
