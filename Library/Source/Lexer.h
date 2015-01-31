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

///
/// \brief A syntactic keyword.
typedef enum
{
    KW_TRUE,
    KW_FALSE,
    KW_NIL
} Keyword;

///
/// \brief A token type.
typedef enum
{
    TOK_SYMBOL,

    TOK_OPERATOR,
    TOK_KEYWORD,
    TOK_IDENT,

    TOK_STRING,
    TOK_NUMBER,

    TOK_EOI
} TokenType;

///
/// \brief Contains the position and state of a lexer.
typedef struct _LexerState
{
    unsigned    index;
    unsigned    line;
    TokenType   type;
    unsigned    startIndex;
    unsigned    length;
    unsigned    id;
    bool        skippedWhitespace;
} LexerState;

///
/// \brief A lexer.
typedef struct Lexer Lexer;

///
/// \brief Creates a new lexer.
///
/// \param source
///        The Nominal source code to lex.
///
/// \returns The new lexer.
Lexer* Lexer_Create(
    const char* source
    );

///
/// \brief Moves to the next token.
///
/// \param lexer
///     The lexer.
///
/// \returns True if there are more tokens to lex; false otherwise.
bool Lexer_Next(
    Lexer*  lexer
    );

///
/// \brief Returns whether the current token is of a given type.
///
/// \param lexer
///     The lexer.
/// \param type
///     The token type to compare.
///
/// \returns True if the token is of the type; false otherwise.
bool Lexer_IsTokenType(
    Lexer*      lexer,
    TokenType   type
    );

///
/// \brief Returns whether the current token has a given ID.
///
/// \param lexer
///     The lexer.
/// \param id
///     The token ID to compare.
///
/// \returns True if the token has the ID; false otherwise.
bool Lexer_IsTokenId(
    Lexer*      lexer,
    unsigned    id
    );

///
/// \brief Returns whether the current token is of a given type and has a
/// given ID.
///
/// \param lexer
///     The lexer.
/// \param type
///     The token type to compare.
/// \param id
///     The token ID to compare.
///
/// \returns True if the token is of the type and has the ID; false otherwise.
bool Lexer_IsTokenTypeAndId(
    Lexer*      lexer,
    TokenType   type,
    unsigned    id
    );

///
/// \brief Returns the type of the current token.
///
/// \param lexer
///     The lexer.
TokenType Lexer_GetTokenType(
    Lexer*  lexer
    );

///
/// \brief Returns the ID of the current token.
///
/// \param lexer
///     The lexer.
unsigned Lexer_GetTokenId(
    Lexer*  lexer
    );

///
/// \brief Returns whether the lexer skipped whitespace when it moved to the
/// current token.
///
/// \param lexer
///     The lexer.
bool Lexer_SkippedWhitespace(
    Lexer*  lexer
    );

///
/// \brief Returns the length of the current token.
///
/// \param lexer
///     The lexer.
size_t Lexer_GetTokenLength(
    Lexer*  lexer
    );

///
/// \brief Returns a pointer to where the current token starts in the source
///
/// \remarks Not null terminated.
///
/// \param lexer
///     The lexer.
const char* Lexer_GetTokenString(
    Lexer*  lexer
    );

///
/// \brief Returns the value of the token as a number.
///
/// \param lexer
///     The lexer.
double Lexer_GetTokenAsNumber(
    Lexer*  lexer
    );

///
/// \brief Saves the state of a lexer.
///
/// \param lexer
///     The lexer.
///
/// \returns The state of the lexer.
LexerState Lexer_SaveState(
    Lexer*  lexer
    );

///
/// \brief Restores the state of a lexer.
///
/// \param lexer
///     The lexer.
/// \param state
///     The state to restore.
void Lexer_RestoreState(
    Lexer*      lexer,
    LexerState  state
    );

#endif
