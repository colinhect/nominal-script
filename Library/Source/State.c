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
#include "Nominal.h"

#include "Map.h"
#include "Closure.h"
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

struct NomState
{
    NomValue        stack[STATE_MAX_STACK_SIZE];
    uint32_t        sp;

    uint32_t        callstack[STATE_MAX_CALLSTACK_SIZE];
    uint32_t        cp;

    unsigned char   byteCode[STATE_MAX_BYTE_CODE];
    uint32_t        ip;
    uint32_t        end;

    Heap*           heap;
    StringPool*     stringPool;

    char            error[2048];
    bool            errorFlag;

    NomValue        globalScope;
};

void CompileAndAppendByteCode(
    NomState*   state,
    const char* source)
{
    assert(state);
    assert(source);

    state->errorFlag = false;

    Parser* p = Parser_Create(source, state->stringPool);
    Node* node = Parser_Exprs(p);

    if (!node)
    {
        NomState_SetError(state, Parser_GetError(p));
    }
    else
    {
        state->end = GenerateCode(state, node, state->byteCode, state->ip);
        Node_Free(node);
    }

    Parser_Free(p);
}

NomState* NomState_Create(
    void
    )
{
    NomState* state = (NomState*)malloc(sizeof(NomState));
    assert(state);

    state->sp = 0;
    state->cp = 0;
    state->ip = 0;
    state->end = 0;
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
    assert(state);

    // Free the string pool
    if (state->stringPool)
    {
        StringPool_Free(state->stringPool);
    }

    // Free the heap
    if (state->heap)
    {
        Heap_Free(state->heap);
    }

    free(state);
}

Heap* NomState_GetHeap(
    NomState*   state
    )
{
    assert(state);
    return state->heap;
}

StringPool* NomState_GetStringPool(
    NomState*   state
    )
{
    assert(state);
    return state->stringPool;
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

// Pops an instruction pointer from the callstack and returns it
#define POP_IP()        state->callstack[--state->cp]

// Pushes an instruction pointer to the callstack
#define PUSH_IP(v)      state->callstack[state->cp++] = v

// Pops a value from the value stack and returns it
#define POP_VALUE()     state->stack[--state->sp]

// Pushes a value to the value stack
#define PUSH_VALUE(v)   state->stack[state->sp++] = v

// Returns the top value in the value stack
#define TOP_VALUE()     state->stack[state->sp - 1]

// Reads the a typed value from the byte code at the current instruction
#define READAS(t)       *(t*)&state->byteCode[state->ip]; state->ip += sizeof(t)

NomValue NomState_Execute(
    NomState*   state,
    const char* source
    )
{
    CompileAndAppendByteCode(state, source);

    NomValue scope = state->globalScope;
    while (state->ip < state->end && !state->errorFlag)
    {
        StringId id;
        NomValue l, r;
        NomValue result = NomValue_Nil();
        NomValue string = NomValue_Nil();

        OpCode op = (OpCode)state->byteCode[state->ip++];

        switch (op)
        {
        case OPCODE_LET:
            id = READAS(StringId);
            string = NomString_FromId(id);
            if (!NomMap_Insert(state, scope, string, TOP_VALUE()))
            {
                NomState_SetError(state, "Variable '%s' already exists", StringPool_Find(state->stringPool, id));
            }
            break;

        case OPCODE_GET:
            id = READAS(StringId);
            string = NomString_FromId(id);
            if (!NomMap_TryGet(state, scope, string, &result))
            {
                NomState_SetError(state, "No variable '%s' in scope", StringPool_Find(state->stringPool, id));
            }
            else
            {
                PUSH_VALUE(result);
            }
            break;

        case OPCODE_SET:
            id = READAS(StringId);
            string = NomString_FromId(id);
            if (!NomMap_Set(state, scope, string, TOP_VALUE()))
            {
                NomState_SetError(state, "No variable '%s'", StringPool_Find(state->stringPool, id));
            }
            break;

        case OPCODE_ADD:
            l = POP_VALUE();
            r = POP_VALUE();
            result = NomValue_Add(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_SUB:
            l = POP_VALUE();
            r = POP_VALUE();
            result = NomValue_Subtract(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_MUL:
            l = POP_VALUE();
            r = POP_VALUE();
            result = NomValue_Multiply(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_DIV:
            l = POP_VALUE();
            r = POP_VALUE();
            result = NomValue_Divide(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_NEG:
            l = POP_VALUE();
            result = NomValue_Negate(state, l);
            PUSH_VALUE(result);
            break;

        case OPCODE_EQ:
        case OPCODE_NE:
        case OPCODE_GT:
        case OPCODE_GTE:
        case OPCODE_LT:
        case OPCODE_LTE:
        case OPCODE_AND:
        case OPCODE_OR:
        case OPCODE_NOT:
        case OPCODE_ASSOC:
            break;

        case OPCODE_RET:
            state->ip = POP_IP();
            break;

        case OPCODE_VALUE_LET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!NomValue_Insert(state, r, l, TOP_VALUE()))
            {
                NomState_SetError(state, "Value for key '%s' already exists", NomString_AsString(state, l));
            }
            break;

        case OPCODE_VALUE_SET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!NomValue_Set(state, r, l, TOP_VALUE()))
            {
                NomState_SetError(state, "No value for key '%s'", NomString_AsString(state, l));
            }
            break;

        case OPCODE_VALUE_GET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!NomValue_TryGet(state, r, l, &result))
            {
                NomState_SetError(state, "No value for key '%s'", NomString_AsString(state, l));
            }
            else
            {
                PUSH_VALUE(result);
            }
            break;

        case OPCODE_BRACKET_SET:
            l = POP_VALUE();
            r = POP_VALUE();
            NomValue_InsertOrSet(state, r, l, TOP_VALUE());
            break;

        case OPCODE_BRACKET_GET:
            l = POP_VALUE();
            r = POP_VALUE();
            result = NomValue_Get(state, r, l);
            PUSH_VALUE(result);
            break;

        case OPCODE_PUSH:
            result = READAS(NomValue);
            PUSH_VALUE(result);
            break;

        case OPCODE_POP:
            (void)POP_VALUE();
            break;

        case OPCODE_NEW_MAP:
        {
            NomValue map = NomMap_Create(state);

            uint32_t itemCount = READAS(uint32_t);
            for (uint32_t i = 0; i < itemCount; ++i)
            {
                NomValue key = POP_VALUE();
                NomValue value = POP_VALUE();

                NomMap_InsertOrSet(state, map, key, value);
            }

            PUSH_VALUE(map);
        } break;

        case OPCODE_NEW_CLOSURE:
        {
            uint32_t ip = READAS(uint32_t);
            result = NomClosure_Create(state, ip);
            PUSH_VALUE(result);
        } break;

        case OPCODE_GOTO:
        {
            uint32_t ip = READAS(uint32_t);
            state->ip = ip;
        } break;

        case OPCODE_INVOKE:
            result = POP_VALUE();
            if (NomClosure_Check(result))
            {
                PUSH_IP(state->ip);
                state->ip = NomClosure_GetInstructionPointer(state, result);
            }
            else
            {
                NomState_SetError(state, "Value cannot be invoked");
            }
            break;
        }
    }

    // Return the result
    if (state->sp == 0)
    {
        return NomValue_Nil();
    }
    else
    {
        NomValue result = POP_VALUE();
        state->sp = 0;
        return result;
    }
}

void NomState_DumpByteCode(
    NomState*   state,
    const char* source
    )
{
    assert(state);
    assert(source);

    CompileAndAppendByteCode(state, source);

    while (state->ip < state->end && !state->errorFlag)
    {
        OpCode op = (OpCode)state->byteCode[state->ip++];
        printf("0x%08x: %s\t", state->ip - 1, OPCODE_NAMES[op]);

        switch (op)
        {
        case OPCODE_LET:
        case OPCODE_GET:
        case OPCODE_SET:
        {
            StringId id = READAS(StringId);
            const char* string = StringPool_Find(state->stringPool, id);
            printf("\t%s", string);
        } break;

        case OPCODE_PUSH:
        {
            NomValue value = READAS(NomValue);
            char buffer[256];
            NomValue_AsString(state, buffer, 256, value);
            printf("%s", buffer);
        } break;

        case OPCODE_NEW_MAP:
        {
            uint32_t itemCount = READAS(uint32_t);
            printf("%u", itemCount);
        } break;

        case OPCODE_NEW_CLOSURE:
        {
            uint32_t ip = READAS(uint32_t);
            printf("0x%08x", ip);
        } break;

        case OPCODE_GOTO:
        {
            uint32_t ip = READAS(uint32_t);
            printf("0x%08x", ip);
        } break;

        default:
            break;
        }

        printf("\n");
    }
}

bool NomState_ErrorOccurred(
    NomState*   state
    )
{
    assert(state);
    return state->errorFlag;
}

const char* NomState_GetError(
    NomState*   state
    )
{
    assert(state);
    return state->error;
}
