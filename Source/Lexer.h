#ifndef LEXER_H
#define LEXER_H

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

typedef struct
{
    unsigned index;
    unsigned line;
    TokenType type;
    unsigned startIndex;
    unsigned length;
    unsigned id;
    unsigned skippedWhitespace;
} LexerState;

typedef struct
{
    const char* source;
    LexerState state;
} Lexer;

Lexer* CreateLexer(const char* source);

int MoveNext(Lexer* l);

int IsTokenType(Lexer* l, TokenType type);
int IsTokenId(Lexer* l, unsigned id);
int IsTokenTypeAndId(Lexer* l, TokenType type, unsigned id);

TokenType GetTokenType(Lexer* l);
unsigned GetTokenId(Lexer* l);

int SkippedWhitespace(Lexer* l);

void CopyTokenValue(Lexer* l, char* dest);

long GetTokenAsInt(Lexer* l);
double GetTokenAsFloat(Lexer* l);

#endif