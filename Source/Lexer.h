#ifndef LEXER_H
#define LEXER_H

#include <string.h>

typedef enum
{
    KW_TRUE,
    KW_FALSE,
    KW_NIL
} Keyword;

typedef enum
{
    TOK_SYMBOL,

    TOK_OPERATOR,
    TOK_KEYWORD,
    TOK_IDENT,

    TOK_STRING,
    TOK_INTEGER,
    TOK_FLOAT,

    TOK_EOI
} TokenType;

typedef struct _LexerState LexerState;

typedef struct _Lexer Lexer;

Lexer* Lexer_Create(const char* source);

int Lexer_Next(Lexer* l);

int Lexer_IsTokenType(Lexer* l, TokenType type);
int Lexer_IsTokenId(Lexer* l, unsigned id);
int Lexer_IsTokenTypeAndId(Lexer* l, TokenType type, unsigned id);

TokenType Lexer_GetTokenType(Lexer* l);
unsigned Lexer_GetTokenId(Lexer* l);

int Lexer_SkippedWhitespace(Lexer* l);

void Lexer_CopyTokenValue(Lexer* l, char* dest);
size_t Lexer_GetTokenLength(Lexer* l);

long Lexer_GetTokenAsInt(Lexer* l);
double Lexer_GetTokenAsFloat(Lexer* l);

#endif