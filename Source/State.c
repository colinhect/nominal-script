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
#include "Nominal.h"

#include "State.h"
#include "Parser.h"
#include "CodeGen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

NomState* NomState_Create(
    )
{
    NomState* state;
    
    state = (NomState*)malloc(sizeof(NomState));
    state->sp = 0;
    state->ip = 0;
    state->heap = Heap_Create();
    state->stringPool = StringPool_Create(128);
    state->scope = Scope_Create();
    state->errorFlag = 0;

    return state;
}

void NomState_Free(
    NomState*   state
    )
{
    Scope_Free(state->scope);
    StringPool_Free(state->stringPool);
    Heap_Free(state->heap);
    free(state);
}

void NomState_SetError(
    NomState*   state,
    const char* fmt,
    ...
    )
{
    va_list args;
    va_start(args, fmt);
    vsprintf(state->error, fmt, args);
    va_end(args);
    state->errorFlag = 1;
}

#define TOP()           state->stack[state->sp - 1]
#define POP()           state->stack[--state->sp]
#define PUSH(v)         state->stack[state->sp++] = v
#define READAS(t)       *(t*)&state->byteCode[state->ip]; state->ip += sizeof(t)

#define ARITH(l, r, op, name)\
    if (!NomValue_IsNumber(l) || !NomValue_IsNumber(r))\
    {\
        NomState_SetError(state, "Cannot %s non-numeric values", name);\
        break;\
    }\
    else if (NomReal_Check(l) || NomReal_Check(r))\
    {\
        res = NomReal_FromFloat(NomValue_AsFloat(l) op NomValue_AsFloat(r));\
    }\
    else\
    {\
        res = NomInteger_FromInt(NomValue_AsInt(l) op NomValue_AsInt(r));\
    }

#define NEG(v)\
    if (!NomValue_IsNumber(v))\
    {\
        NomState_SetError(state, "Cannot negate a non-numeric value");\
        break;\
    }\
    else if (NomReal_Check(v))\
    {\
        res = NomReal_FromFloat(-NomValue_AsFloat(v));\
    }\
    else\
    {\
        res = NomInteger_FromInt(-NomValue_AsInt(v));\
    }

void NomState_Execute(
    NomState*   state,
    const char* source
    )
{
    Parser* p;
    Node*   node;
    size_t  end;

    state->errorFlag = 0;

    p = Parser_Create(source, state->stringPool);
    node = Parser_Expr(p);

    if (!node)
    {
        NomState_SetError(state, Parser_GetError(p));
        return;
    }

    end = GenerateCode(node, state->byteCode, state->ip);
    Node_Free(node);

    while (state->ip < end && !state->errorFlag)
    {
        StringId id;
        NomValue l;
        NomValue r;
        NomValue res;

        OpCode op = (OpCode)state->byteCode[state->ip++];
        switch (op)
        {
        case OP_PUSH:
            res = READAS(NomValue);
            PUSH(res);
            break;
        case OP_GET:
            id = READAS(StringId);
            res = Scope_Get(state->scope, id);
            PUSH(res);
            break;
        case OP_SET:
            id = READAS(StringId);
            if (!Scope_Set(state->scope, id, TOP()))
            {
                NomState_SetError(state, "No variable '%s'", StringPool_Find(state->stringPool, id));
            }
            break;
        case OP_LET:
            id = READAS(StringId);
            if (!Scope_Let(state->scope, id, TOP()))
            {
                NomState_SetError(state, "Variable '%s' already exists", StringPool_Find(state->stringPool, id));
            }
            break;
        case OP_ADD:
            l = POP();
            r = POP();
            ARITH(l, r, +, "add");
            PUSH(res);
            break;
        case OP_SUB:
            l = POP();
            r = POP();
            ARITH(l, r, -, "subtract");
            PUSH(res);
            break;
        case OP_MUL:
            l = POP();
            r = POP();
            ARITH(l, r, *, "multiply");
            PUSH(res);
            break;
        case OP_DIV:
            l = POP();
            r = POP();
            ARITH(l, r, /, "divide");
            PUSH(res);
            break;
        case OP_NEG:
            l = POP();
            NEG(l);
            PUSH(res);
            break;
        }
    }
}

NomValue NomState_Pop(
    NomState*   state
    )
{
    if (state->sp == 0)
    {
        return NOM_NIL;
    }
    else
    {
        return POP();
    }
}

int NomState_ErrorOccurred(
    NomState*   state
    )
{
    return state->errorFlag;
}

const char* NomState_GetError(
    NomState*   state
    )
{
    return state->error;
}