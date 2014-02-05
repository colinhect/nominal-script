#ifndef PARSER_H
#define PARSER_H

#include "Node.h"
#include "ByteCode.h"
#include "Lexer.h"

typedef struct _Parser Parser;

Parser* Parser_Create(const char* source);
void Parser_Free(Parser* p);

void Parser_SetError(Parser* p, const char* fmt, ...);
const char* Parser_GetError(Parser* p);

Node* Parser_Expr(Parser* p);
Node* Parser_PrimaryExpr(Parser* p);
Node* Parser_SecondaryExpr(Parser* p);
Node* Parser_ParenExpr(Parser* p);
Node* Parser_BinExpr(Parser* p, int prec, Node* left);

#endif