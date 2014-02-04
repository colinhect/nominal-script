#include "Lexer.h"
#include "bytecode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

Lexer* CreateLexer(const char* source)
{
    LexerState state = { 0, 1, TOK_SYMBOL, 0, 0, 0, 0 };
    Lexer* l = (Lexer*)malloc(sizeof(Lexer));
    l->state = state;
    l->source = source;
    return l;
}

int MoveNext(Lexer* l)
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

            l->state.type = TOK_FLOAT;
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

int IsTokenType(Lexer* l, TokenType type)
{
    return l->state.type == type;
}

int IsTokenId(Lexer* l, unsigned id)
{
    return l->state.id == id;
}

int IsTokenTypeAndId(Lexer* l, TokenType type, unsigned id)
{
    return IsTokenType(l, type) && IsTokenId(l, id);
}

TokenType GetTokenType(Lexer* l)
{
    return l->state.type;
}

unsigned GetTokenId(Lexer* l)
{
    return l->state.id;
}

int SkippedWhitespace(Lexer* l)
{
    return l->state.skippedWhitespace;
}

void CopyTokenValue(Lexer* l, char* dest)
{
    strncpy(dest, &l->source[l->state.startIndex], l->state.length);
    dest[l->state.length] = '\0';
}

long GetTokenAsInt(Lexer* l)
{
    char buffer[32];
    CopyTokenValue(l, buffer);
    return atol(buffer);
}

double GetTokenAsFloat(Lexer* l)
{
    char buffer[32];
    CopyTokenValue(l, buffer);
    return atof(buffer);
}