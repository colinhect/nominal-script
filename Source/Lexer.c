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
#include "Lexer.h"
#include "bytecode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct _LexerState
{
    unsigned index;
    unsigned line;
    TokenType type;
    unsigned startIndex;
    unsigned length;
    unsigned id;
    unsigned skippedWhitespace;
} LexerState;

typedef struct _Lexer
{
    const char* source;
    LexerState state;
} Lexer;

char ReadNext(Lexer* l)
{
    char c;

    if (l->state.type == TOK_EOI)
    {
        return '\0';
    }

    c = l->source[l->state.index++];
    if (c == '\n')
    {
        ++l->state.line;
    }
    else if (c == '\0')
    {
        l->state.type = TOK_EOI;
    }

    return c;
}

char PeekNext(Lexer* l)
{
    if (l->state.type == TOK_EOI)
    {
        return '\0';
    }
    return l->source[l->state.index];
}

int StartsWith(const char* str, const char* pre)
{
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

Lexer* Lexer_Create(const char* source)
{
    LexerState state = { 0, 1, TOK_SYMBOL, 0, 0, 0, 0 };
    Lexer* l = (Lexer*)malloc(sizeof(Lexer));
    l->state = state;
    l->source = source;
    return l;
}

int Lexer_Next(Lexer* l)
{
    LexerState state;
    char       c;

    l->state.skippedWhitespace = 0;

    // Skip whitespace
    do
    {
        c = ReadNext(l);
        if (c == '\0')
        {
            return 0;
        }
        if (isspace(c))
        {
            l->state.skippedWhitespace = 1;
        }
    } while (isspace(c));

    // Keep the token's start index and assume it is at least length 1
    l->state.startIndex = l->state.index - 1;
    l->state.length = 1;

    // Operator
    state = l->state;
    l->state.type = TOK_OPERATOR;
    switch (c)
    {
    case ':':
        {
            if (PeekNext(l) == '=')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_LET;
                return 1;
            }
        } break;
    case '=':
        {
            if (PeekNext(l) == '=')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_EQ;
            }
            else
            {
                l->state.id = OP_SET;
            }
            return 1;
        } break;
    case '+':
        {
            l->state.id = OP_ADD;
            return 1;
        } break;
    case '-': {
            if (PeekNext(l) == '>')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_ASSOC;
            }
            else
            {
                l->state.id = OP_SUB;
            }
            return 1;
        } break;
    case '*':
        {
            l->state.id = OP_MUL;
            return 1;
        } break;
    case '/':
        {
            l->state.id = OP_DIV;
            return 1;
        } break;
    case '!': {
            if (PeekNext(l) == '=')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_NE;
            }
            else
            {
                l->state.id = OP_NOT;
            }
            return 1;
        } break;
    case '>':
        {
            if (PeekNext(l) == '=')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_GTE;
            }
            else
            {
                l->state.id = OP_GT;
            }
            return 1;
        } break;
    case '<':
        {
            if (PeekNext(l) == '=')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_LT;
            }
            else if (PeekNext(l) == '-')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_RET;
            }
            else
            {
                l->state.id = OP_LTE;
            }
            return 1;
        } break;
    case '|':
        {
            if (PeekNext(l) == '|')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_OR;
                return 1;
            }
        } break;
    case '&':
        {
            if (PeekNext(l) == '&')
            {
                ReadNext(l);
                l->state.length = 2;
                l->state.id = OP_AND;
                return 1;
            }
        } break;
    }

    // The attempt to parse an operator accomplished nothing
    l->state = state;

    // Identifier or Keyword
    if (isalpha(c))
    {
        const char* current;

        while (isalnum(PeekNext(l)))
        {
            ReadNext(l);
            ++l->state.length;
        }

        current = &l->source[l->state.startIndex];

        if (StartsWith(current, "true"))
        {
            l->state.type = TOK_KEYWORD;
            l->state.id = KW_TRUE;
        }
        else if (StartsWith(current, "false"))
        {
            l->state.type = TOK_KEYWORD;
            l->state.id = KW_FALSE;
        }
        else if (StartsWith(current, "nil"))
        {
            l->state.type = TOK_KEYWORD;
            l->state.id = KW_NIL;
        }
        else
        {
            l->state.type = TOK_IDENT;
        }

        return 1;
    }

    // Integer or float
    if (isdigit(c))
    {
        while (isdigit(PeekNext(l)))
        {
            ReadNext(l);
            ++l->state.length;
        }

        if (PeekNext(l) == '.')
        {
            ReadNext(l);
            ++l->state.length;

            while (isdigit(PeekNext(l)))
            {
                ReadNext(l);
                ++l->state.length;
            }

            l->state.type = TOK_REAL;
        }
        else
        {
            l->state.type = TOK_INTEGER;
        }

        return 1;
    }

    // String
    if (c == '\"')
    {
        while (PeekNext(l) != '\"')
        {
            ReadNext(l);
            ++l->state.length;
        }
        ReadNext(l);

        --l->state.length;
        ++l->state.startIndex;

        l->state.type = TOK_STRING;
        return 1;
    }

    // Anything else is a symbol
    l->state.type = TOK_SYMBOL;
    l->state.id = c;

    return 1;
}

int Lexer_IsTokenType(Lexer* l, TokenType type)
{
    return l->state.type == type;
}

int Lexer_IsTokenId(Lexer* l, unsigned id)
{
    return l->state.id == id;
}

int Lexer_IsTokenTypeAndId(Lexer* l, TokenType type, unsigned id)
{
    return Lexer_IsTokenType(l, type) && Lexer_IsTokenId(l, id);
}

TokenType Lexer_GetTokenType(Lexer* l)
{
    return l->state.type;
}

unsigned Lexer_GetTokenId(Lexer* l)
{
    return l->state.id;
}

int Lexer_SkippedWhitespace(Lexer* l)
{
    return l->state.skippedWhitespace;
}

size_t Lexer_GetTokenLength(Lexer* l)
{
    return l->state.length;
}

const char* Lexer_GetTokenString(Lexer* l)
{
    return &l->source[l->state.startIndex];
}

long Lexer_GetTokenAsInt(Lexer* l)
{
    return atol(Lexer_GetTokenString(l));
}

double Lexer_GetTokenAsFloat(Lexer* l)
{
    return atof(Lexer_GetTokenString(l));
}