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

#include "node.h"
#include "stringpool.h"
#include "lexer.h"

#define MAX_PARSER_ERROR_LENGTH (1024)

// A parser
typedef struct Parser
{
    Lexer*      lexer;
    StringPool* stringpool;
    char        error[MAX_PARSER_ERROR_LENGTH];
    char        fullerror[MAX_PARSER_ERROR_LENGTH];
} Parser;

// Creates a new parser
Parser* parser_new(
    const char* source,
    StringPool* stringpool
);

// Frees a parser
void parser_free(
    Parser* parser
);

// Sets the error message of a parser
void parser_seterror(
    Parser*     parser,
    const char* fmt,
    ...
);

// Returns the error message of a parser
const char* parser_geterror(
    Parser* parser
);

// Parses a sequence of expressions (returns NULL if a parse error occurred)
Node* parser_exprs(
    Parser* parser,
    bool    newlines
);

// Parses an expression (returns NULL if a parse error occurred)
Node* parser_expr(
    Parser* parser
);

// Parses a primary expression (returns NULL if a parse error occurred)
Node* parser_primaryexpr(
    Parser* parser
);

// Parses a secondary expression (returns NULL if a parse error occurred)
Node* parser_secondaryexpr(
    Parser* parser
);

// Parses an expression in parenthesis (returns NULL if a parse error occurred)
Node* parser_parenexpr(
    Parser* parser
);

// Parses a binary expression (returns NULL if a parse error occurred)
Node* parser_binexpr(
    Parser* parser,
    int     prec,
    Node*   leftexpr
);

// Parses a map (returns NULL if a parse error occurred)
Node* parser_map(
    Parser* parser
);

// Parses a function (returns NULL if a parse error occurred)
Node* parser_function(
    Parser* parser
);

// Parses a string or identifier (returns NULL if a parse error occurred)
Node* parser_stringorident(
    Parser* parser
);

#endif
