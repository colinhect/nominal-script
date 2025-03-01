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
#include "lexer.h"
#include "node.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Moves to the next character and returns it
char readnext(
    Lexer*  lexer
);

// Reads the next character and returns it without affecting the lexer state
char peeknext(
    Lexer*  lexer
);

// Returns whether a string starts with a sub-string
bool startswith(
    const char* str,
    const char* pre
);

Lexer* lexer_new(
    const char* source
)
{
    LexerState state = { 0, 1, TOK_SYMBOL, 0, 0, 0, 0 };
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    assert(lexer);

    lexer->state = state;
    lexer->source = source;
    return lexer;
}

bool lexer_next(
    Lexer*  lexer
)
{
    lexer->state.skippedwhitespace = false;
    lexer->state.skippednewline = false;

    // Skip whitespace
    char c;
    do
    {
        c = readnext(lexer);

        if (c == '\0')
        {
            lexer->state.skippedwhitespace = false;
            lexer->state.skippednewline = false;
            return false;
        }

        if (isspace(c))
        {
            lexer->state.skippedwhitespace = true;
        }

        if (c == '\n')
        {
            lexer->state.skippednewline = true;
        }

        // Skip single-line comments
        if (c == '-' && peeknext(lexer) == '-')
        {
            lexer->state.skippedwhitespace = true;
            readnext(lexer);

            do
            {
                c = readnext(lexer);
                if (c == '\0')
                {
                    return false;
                }
            }
            while (c != '\n');
            lexer->state.skippednewline = true;

            c = readnext(lexer);
            if (c == '\0')
            {
                return false;
            }
        }

        // Skip multi-line comments
        if (c == '{' && peeknext(lexer) == '-')
        {
            readnext(lexer);

            do
            {
                c = readnext(lexer);
                if (c == '\0')
                {
                    return false;
                }
                else if (c == '\n')
                {
                    lexer->state.skippednewline = true;
                }
            }
            while ((c != '-' || peeknext(lexer) != '}') && c != '\0');

            readnext(lexer);
            c = readnext(lexer);
            if (c == '\0')
            {
                return false;
            }
            else if (c == '\n')
            {
                lexer->state.skippednewline = true;
            }

            lexer->state.skippedwhitespace = true;
        }
    }
    while (isspace(c));

    // Keep the token's start index and assume it is at least length 1
    lexer->state.startindex = lexer->state.index - 1;
    lexer->state.length = 1;

    // Operator
    LexerState state = lexer->state;
    lexer->state.type = TOK_OPERATOR;
    switch (c)
    {
    case ':':
        if (peeknext(lexer) == '=')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_DEFINE;
            return true;
        }
        break;
    case '=':
        if (peeknext(lexer) == '=')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_EQ;
        }
        else
        {
            lexer->state.id = OP_ASSIGN;
        }
        return true;
    case '+':
        lexer->state.id = OP_ADD;
        return true;
    case '-':
        if (peeknext(lexer) == '>')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_ASSOC;
        }
        else
        {
            lexer->state.id = OP_SUB;
        }
        return true;
    case '*':
        lexer->state.id = OP_MUL;
        return true;
    case '/':
        lexer->state.id = OP_DIV;
        return true;
    case '!':
        if (peeknext(lexer) == '=')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_NE;
        }
        else
        {
            lexer->state.id = OP_NOT;
        }
        return true;
    case '>':
        if (peeknext(lexer) == '=')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_GTE;
        }
        else
        {
            lexer->state.id = OP_GT;
        }
        return true;
    case '<':
        if (peeknext(lexer) == '=')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_LTE;
        }
        else if (peeknext(lexer) == '-')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_RET;
        }
        else
        {
            lexer->state.id = OP_LT;
        }
        return true;
    case '|':
        if (peeknext(lexer) == '|')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_OR;
            return true;
        }
        break;
    case '&':
        if (peeknext(lexer) == '&')
        {
            readnext(lexer);
            lexer->state.length = 2;
            lexer->state.id = OP_AND;
            return true;
        }
        break;
    }

    // The attempt to parse an operator accomplished nothing
    lexer->state = state;

    // Identifier
    if (isalpha(c) || c == '_')
    {
        while (isalnum(peeknext(lexer)) || peeknext(lexer) == '_')
        {
            readnext(lexer);
            ++lexer->state.length;
        }

        lexer->state.type = TOK_IDENT;

        return true;
    }

    // Number
    if (isdigit(c))
    {
        while (isdigit(peeknext(lexer)))
        {
            readnext(lexer);
            ++lexer->state.length;
        }

        if (peeknext(lexer) == '.')
        {
            readnext(lexer);
            ++lexer->state.length;

            while (isdigit(peeknext(lexer)))
            {
                readnext(lexer);
                ++lexer->state.length;
            }
        }

        lexer->state.type = TOK_NUMBER;
        return true;
    }

    // String
    if (c == '\"')
    {
        while (peeknext(lexer) != '\"')
        {
            readnext(lexer);
            ++lexer->state.length;
        }
        readnext(lexer);

        --lexer->state.length;
        ++lexer->state.startindex;

        lexer->state.type = TOK_STRING;
        return true;
    }

    // Anything else is a symbol
    lexer->state.type = TOK_SYMBOL;
    lexer->state.id = c;

    return true;
}

bool lexer_istokentype(
    Lexer*      lexer,
    TokenType   type
)
{
    return lexer->state.type == type;
}

bool lexer_istokenid(
    Lexer*      lexer,
    unsigned    id
)
{
    return lexer->state.id == id;
}

bool lexer_istokentypeandid(
    Lexer*      lexer,
    TokenType   type,
    unsigned    id
)
{
    return lexer_istokentype(lexer, type) && lexer_istokenid(lexer, id);
}

TokenType lexer_gettokentype(
    Lexer*  lexer
)
{
    return lexer->state.type;
}

unsigned lexer_gettokenid(
    Lexer*  lexer
)
{
    return lexer->state.id;
}

bool lexer_skippedwhitespace(
    Lexer*  lexer
)
{
    return lexer->state.skippedwhitespace;
}

bool lexer_skippednewline(
    Lexer*  lexer
)
{
    return lexer->state.skippednewline;
}

size_t lexer_gettokenlength(
    Lexer*  lexer
)
{
    return lexer->state.length;
}

const char* lexer_gettokenstring(
    Lexer*  lexer
)
{
    return &lexer->source[lexer->state.startindex];
}

double lexer_gettokenasnumber(
    Lexer*  lexer
)
{
    return atof(lexer_gettokenstring(lexer));
}

LexerState lexer_savestate(
    Lexer*  lexer
)
{
    return lexer->state;
}

void lexer_restorestate(
    Lexer*      lexer,
    LexerState  state
)
{
    lexer->state = state;
}

char readnext(
    Lexer*  lexer
)
{
    if (lexer->state.type == TOK_EOI)
    {
        return '\0';
    }

    char c = lexer->source[lexer->state.index++];
    if (c == '\n')
    {
        ++lexer->state.line;
    }
    else if (c == '\0')
    {
        lexer->state.type = TOK_EOI;
    }

    return c;
}

char peeknext(
    Lexer*  lexer
)
{
    if (lexer->state.type == TOK_EOI)
    {
        return '\0';
    }
    return lexer->source[lexer->state.index];
}

bool startswith(
    const char* str,
    const char* pre
)
{
    size_t prelength = strlen(pre);
    size_t strlength = strlen(str);
    return strlength < prelength ? false : strncmp(pre, str, prelength) == 0;
}
