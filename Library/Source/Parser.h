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

// A parser
typedef struct Parser Parser;

// Creates a new parser
Parser* Parser_New(
    const char* source,
    StringPool* stringPool
    );

// Frees a parser
void Parser_Free(
    Parser* parser
    );

// Sets the error message of a parser
void Parser_SetError(
    Parser*     parser,
    const char* fmt,
    ...
    );

// Returns the error message of a parser
const char* Parser_GetError(
    Parser* parser
    );

// Parses a sequence of expressions (returns NULL if a parse error occurred)
Node* Parser_Exprs(
    Parser* parser
    );

// Parses an expression (returns NULL if a parse error occurred)
Node* Parser_Expr(
    Parser* parser
    );

// Parses a primary expression (returns NULL if a parse error occurred)
Node* Parser_PrimaryExpr(
    Parser* parser
    );

// Parses a secondary expression (returns NULL if a parse error occurred)
Node* Parser_SecondaryExpr(
    Parser* parser
    );

// Parses an expression in parenthesis (returns NULL if a parse error occurred)
Node* Parser_ParenExpr(
    Parser* parser
    );

// Parses a binary expression (returns NULL if a parse error occurred)
Node* Parser_BinExpr(
    Parser* parser,
    int     prec,
    Node*   leftExpr
    );

// Parses a map (returns NULL if a parse error occurred)
Node* Parser_Map(
    Parser* parser
    );

// Parses a function (returns NULL if a parse error occurred)
Node* Parser_Function(
    Parser* parser
    );

// Parses a string or identifier (returns NULL if a parse error occurred)
Node* Parser_StringOrIdent(
    Parser* parser
    );

#endif
