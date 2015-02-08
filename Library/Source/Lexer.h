///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2015 Colin Hill
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
#include <stdbool.h>
#include <stdint.h>

// A token type
typedef enum
{
    TOK_SYMBOL,
    TOK_OPERATOR,
    TOK_IDENT,
    TOK_STRING,
    TOK_NUMBER,
    TOK_EOI
} TokenType;

// Contains the position and state of a lexer
typedef struct
{
    unsigned    index;
    unsigned    line;
    TokenType   type;
    unsigned    startIndex;
    unsigned    length;
    unsigned    id;
    bool        skippedWhitespace;
} LexerState;

// A lexer
typedef struct Lexer Lexer;

// Creates a new lexer
Lexer* Lexer_New(
    const char* source
    );

// Moves to the next token, returning true if there are more tokens to lex or
// false otherwise
bool Lexer_Next(
    Lexer*  lexer
    );

// Returns whether the current token is of a specified type
bool Lexer_IsTokenType(
    Lexer*      lexer,
    TokenType   type
    );

// Returns whether the current token has a specified ID
bool Lexer_IsTokenId(
    Lexer*      lexer,
    unsigned    id
    );

// Returns whether the current token is of a specified type and has a
// specified ID
bool Lexer_IsTokenTypeAndId(
    Lexer*      lexer,
    TokenType   type,
    unsigned    id
    );

// Returns the type of the current token
TokenType Lexer_GetTokenType(
    Lexer*  lexer
    );

// Returns the ID of the current token
unsigned Lexer_GetTokenId(
    Lexer*  lexer
    );

// Returns whether the lexer skipped whitespace when it moved to the current
// token
bool Lexer_SkippedWhitespace(
    Lexer*  lexer
    );

// Returns the length of the current token
size_t Lexer_GetTokenLength(
    Lexer*  lexer
    );

// Returns a pointer to where the current token starts in the source (the
// string is not NULL terminated)
const char* Lexer_GetTokenString(
    Lexer*  lexer
    );

// Returns the value of the token as a number
double Lexer_GetTokenAsNumber(
    Lexer*  lexer
    );

// Saves the state of a lexer
LexerState Lexer_SaveState(
    Lexer*  lexer
    );

// Restores the state of a lexer
void Lexer_RestoreState(
    Lexer*      lexer,
    LexerState  state
    );

#endif
