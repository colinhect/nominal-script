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
#include "String.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define STRING_POOL_STRING_COUNT    (256)

NomState* states[STATE_MAX_INSTANCES] = { 0 };

NomState* NomState_Create(
    void
    )
{
    NomState* state = (NomState*)malloc(sizeof(NomState));
    assert(state);

    // Find the next available ID
    state->id = 0;
    while (states[state->id])
    {
        ++state->id;
    }
    states[state->id] = state;

    state->sp = 0;
    state->ip = 0;
    state->heap = Heap_Create();
    state->stringPool = StringPool_Create(STRING_POOL_STRING_COUNT);
    state->errorFlag = 0;

    state->globalScope = NomMap_Create(state);

    return state;
}

void NomState_Free(
    NomState*   state
    )
{
    StringPool_Free(state->stringPool);
    Heap_Free(state->heap);
    states[state->id] = NULL;
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
    state->errorFlag = true;
}

#define TOP()           state->stack[state->sp - 1]
#define POP()           state->stack[--state->sp]
#define PUSH(v)         state->stack[state->sp++] = v
#define READAS(t)       *(t*)&state->byteCode[state->ip]; state->ip += sizeof(t)

NomValue NomState_Execute(
    NomState*   state,
    const char* source
    )
{
    state->errorFlag = false;

    NomValue nil = NomValue_Nil(state);

    Parser* p = Parser_Create(source, state->stringPool);
    Node* node = Parser_Exprs(p);

    if (!node)
    {
        NomState_SetError(state, Parser_GetError(p));
        return nil;
    }

    size_t end = GenerateCode(state, node, state->byteCode, state->ip);
    Node_Free(node);

    NomValue scope = state->globalScope;

    while (state->ip < end && !state->errorFlag)
    {
        StringId id;
        NomValue l;
        NomValue r;
        NomValue result = nil;
        NomValue string = nil;

        OpCode op = (OpCode)state->byteCode[state->ip++];

        switch (op)
        {
        case OPCODE_PUSH:
            result = READAS(NomValue);
            PUSH(result);
            break;
        case OPCODE_POP:
            POP();
            break;
        case OPCODE_GET:
            id = READAS(StringId);
            string = NomString_FromId(state, id);
            if (!NomMap_TryGet(scope, string, &result))
            {
                NomState_SetError(state, "No variable '%s' in scope", StringPool_Find(state->stringPool, id));
            }
            PUSH(result);
            break;
        case OPCODE_SET:
            id = READAS(StringId);
            string = NomString_FromId(state, id);
            if (!NomMap_Set(scope, string, TOP()))
            {
                NomState_SetError(state, "No variable '%s'", StringPool_Find(state->stringPool, id));
            }
            break;
        case OPCODE_LET:
            id = READAS(StringId);
            string = NomString_FromId(state, id);
            if (!NomMap_Insert(scope, string, TOP()))
            {
                NomState_SetError(state, "Variable '%s' already exists", StringPool_Find(state->stringPool, id));
            }
            break;
        case OPCODE_NEW_MAP:
            {
                NomValue map = NomMap_Create(state);

                int itemCount = NomNumber_AsInt(POP());
                for (int i = 0; i < itemCount; ++i)
                {
                    NomValue key = POP();
                    NomValue value = POP();

                    NomMap_InsertOrSet(map, key, value);
                }

                PUSH(map);
            } break;
        case OPCODE_ADD:
            l = POP();
            r = POP();
            result = NomValue_Add(l, r);
            PUSH(result);
            break;
        case OPCODE_SUB:
            l = POP();
            r = POP();
            result = NomValue_Subtract(l, r);
            PUSH(result);
            break;
        case OPCODE_MUL:
            l = POP();
            r = POP();
            result = NomValue_Multiply(l, r);
            PUSH(result);
            break;
        case OPCODE_DIV:
            l = POP();
            r = POP();
            result = NomValue_Divide(l, r);
            PUSH(result);
            break;
        case OPCODE_NEG:
            l = POP();
            result = NomValue_Negate(l);
            PUSH(result);
            break;
        case OPCODE_VALUE_LET:
            l = POP();
            r = POP();
            if (!NomValue_Insert(r, l, TOP()))
            {
                NomState_SetError(state, "Value for key '%s' already exists", NomString_AsString(l));
            }
            break;
        case OPCODE_VALUE_SET:
            l = POP();
            r = POP();
            if (!NomValue_Set(r, l, TOP()))
            {
                NomState_SetError(state, "No value for key '%s'", NomString_AsString(l));
            }
            break;
        case OPCODE_VALUE_GET:
            l = POP();
            r = POP();
            if (!NomValue_TryGet(r, l, &result))
            {
                NomState_SetError(state, "No value for key '%s'", NomString_AsString(l));
            }
            PUSH(result);
            break;
        case OPCODE_BRACKET_SET:
            l = POP();
            r = POP();
            NomValue_InsertOrSet(r, l, TOP());
            break;
        case OPCODE_BRACKET_GET:
            l = POP();
            r = POP();
            result = NomValue_Get(r, l);
            PUSH(result);
            break;
        }
    }

    // Return the result
    if (state->sp == 0)
    {
        return nil;
    }
    else
    {
        NomValue result = POP();
        state->sp = 0;
        return result;
    }
}

bool NomState_ErrorOccurred(
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

NomState* NomState_GetInstance(
    StateId id
    )
{
    return states[id];
}