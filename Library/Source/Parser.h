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
#ifndef PARSER_H
#define PARSER_H

#include "Node.h"
#include "StringPool.h"
#include "Lexer.h"

///
/// \brief A parser.
typedef struct _Parser Parser;

///
/// \brief Creates a new parser.
///
/// \param source
///     The Nominal source code to parse.
/// \param stringPool
///     The string pool to use.
///
/// \returns The parser.
Parser* Parser_Create(
    const char* source,
    StringPool* stringPool
    );

///
/// \brief Frees a parser.
///
/// \param parser
///     The parser to free.
void Parser_Free(
    Parser* parser
    );

///
/// \brief Sets the error message of a parser.
///
/// \param parser
///     The parser.
/// \param fmt
///     The error message format.
void Parser_SetError(
    Parser*     parser,
    const char* fmt,
    ...
    );

///
/// \brief Returns the error message of a parser.
///
/// \param parser
///     The parser.
///
/// \returns The error message.
const char* Parser_GetError(
    Parser* parser
    );

///
/// \brief Parses a sequence of expressions.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_Exprs(
    Parser* parser
    );

///
/// \brief Parses an expression.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_Expr(
    Parser* parser
    );

///
/// \brief Parses a primary expression.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_PrimaryExpr(
    Parser* parser
    );

///
/// \brief Parses a secondary expression.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_SecondaryExpr(
    Parser* parser
    );

///
/// \brief Parses an expression in parenthesis.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_ParenExpr(
    Parser* parser
    );

///
/// \brief Parses a binary expression.
///
/// \param parser
///     The parser.
/// \param prec
///     The current precedence
/// \param leftExpr
///     The left-hand expression
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_BinExpr(
    Parser* parser,
    int     prec,
    Node*   leftExpr
    );

///
/// \brief Parses a map.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_Map(
    Parser* parser
    );

///
/// \brief Parses a string or identifier.
///
/// \param parser
///     The parser.
///
/// \returns The parsed AST node; NULL if a parse error occurred.
Node* Parser_StringOrIdent(
    Parser* parser
    );

#endif
