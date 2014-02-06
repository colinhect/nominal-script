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
    TOK_REAL,

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

size_t Lexer_GetTokenLength(Lexer* l);
const char* Lexer_GetTokenString(Lexer* l);

long Lexer_GetTokenAsInt(Lexer* l);
double Lexer_GetTokenAsFloat(Lexer* l);

#endif