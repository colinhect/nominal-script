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

#include "Value.h"
#include "Map.h"
#include "Function.h"
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

typedef struct
{
    uint32_t    ip;
    NomValue    scope;
} StackFrame;

struct NomState
{
    NomValue        stack[STATE_MAX_STACK_SIZE];
    uint32_t        sp;

    StackFrame      callstack[STATE_MAX_CALLSTACK_SIZE];
    uint32_t        cp;

    unsigned char   bytecode[STATE_MAX_BYTE_CODE];
    uint32_t        ip;
    uint32_t        end;

    Heap*           heap;
    StringPool*     stringpool;

    char            error[2048];
    bool            errorflag;
};

// Pops a stack frame from the callstack and returns the return instruction
// pointer
#define POP_FRAME()\
    state->callstack[--state->cp].ip

// Pushes a stack frame on the callstack given the return instruction pointer
// and the scope
#define PUSH_FRAME(v, s)\
    state->callstack[state->cp].ip = v;\
    state->callstack[state->cp++].scope = s

// Returns the top stack frame on the callstack
#define TOP_FRAME()\
    state->callstack[state->cp - 1]

// Pops a value from the value stack and returns it
#define POP_VALUE()\
    state->stack[--state->sp]

// Pushes a value to the value stack
#define PUSH_VALUE(v)\
    state->stack[state->sp++] = v

// Returns the top value in the value stack
#define TOP_VALUE()\
    state->stack[state->sp - 1]

// Reads the a typed value from the byte code at the current instruction
#define READAS(t)\
    *(t*)&state->bytecode[state->ip]; state->ip += sizeof(t)

static void compile(
    NomState*   state,
    const char* source)
{
    assert(state);
    assert(source);

    state->errorflag = false;

    Parser* p = parser_new(source, state->stringpool);
    Node* node = parser_exprs(p);

    if (!node)
    {
        state_seterror(state, parser_geterror(p));
    }
    else
    {
        state->end = generatecode(state, node, state->bytecode, state->ip);
        node_free(node);
    }

    parser_free(p);
}

NomState* nom_newstate(
    void
    )
{
    NomState* state = (NomState*)malloc(sizeof(NomState));
    assert(state);

    memset(state, 0, sizeof(NomState));

    state->heap = heap_new();
    state->stringpool = stringpool_new(STRING_POOL_STRING_COUNT);

    // Global scope
    PUSH_FRAME(0, nom_newmap(state));

    // Declare intrinsic globals
    StringId id = stringpool_getid(state->stringpool, "nil");
    state_letinterned(state, id, nom_nil());
    id = stringpool_getid(state->stringpool, "true");
    state_letinterned(state, id, nom_true());
    id = stringpool_getid(state->stringpool, "false");
    state_letinterned(state, id, nom_false());

    return state;
}

void nom_freestate(
    NomState*   state
    )
{
    assert(state);

    // Free the string pool
    if (state->stringpool)
    {
        stringpool_free(state->stringpool);
    }

    // Free the heap
    if (state->heap)
    {
        heap_free(state->heap);
    }

    free(state);
}

Heap* state_getheap(
    NomState*   state
    )
{
    assert(state);
    return state->heap;
}

StringPool* state_getstringpool(
    NomState*   state
    )
{
    assert(state);
    return state->stringpool;
}

void state_letinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
    )
{
    assert(state);

    NomValue string = string_newinterned(id);

    // Attempt to define the variable at the top-most scope
    NomValue scope = TOP_FRAME().scope;
    if (!map_insert(state, scope, string, value))
    {
        state_seterror(state, "Variable '%s' already exists", stringpool_find(state->stringpool, id));
    }
}

void state_setinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
    )
{
    assert(state);

    NomValue string = string_newinterned(id);

    bool success = false;

    // For each stack frame
    for (int i = state->cp - 1; i >= 0; --i)
    {
        // Try to set the variable
        NomValue scope = state->callstack[i].scope;
        if (map_set(state, scope, string, value))
        {
            success = true;
            break;
        }
    }

    if (!success)
    {
        state_seterror(state, "No variable '%s'", stringpool_find(state->stringpool, id));
    }
}

NomValue state_getinterned(
    NomState*   state,
    StringId    id
    )
{
    assert(state);

    NomValue result = nom_nil();
    NomValue string = string_newinterned(id);

    bool success = false;

    // For each stack frame
    for (int i = state->cp - 1; i >= 0; --i)
    {
        // Try to get the variable value
        NomValue scope = state->callstack[i].scope;
        if (map_tryget(state, scope, string, &result))
        {
            success = true;
            break;
        }
    }

    if (!success)
    {
        state_seterror(state, "No variable '%s' in scope", stringpool_find(state->stringpool, id));
    }

    return result;
}

void state_seterror(
    NomState*   state,
    const char* fmt,
    ...
    )
{
    va_list args;
    va_start(args, fmt);
    vsprintf(state->error, fmt, args);
    va_end(args);
    state->errorflag = true;
}

NomValue nom_execute(
    NomState*   state,
    const char* source
    )
{
    compile(state, source);

    StringId id;
    NomValue l, r, result;
    OpCode op;
    uint32_t count, ip;

    while (state->ip < state->end && !state->errorflag)
    {
        op = (OpCode)state->bytecode[state->ip++];
        switch (op)
        {
        case OPCODE_LET:
            id = READAS(StringId);
            state_letinterned(state, id, TOP_VALUE());
            break;

        case OPCODE_SET:
            id = READAS(StringId);
            state_setinterned(state, id, TOP_VALUE());
            break;

        case OPCODE_GET:
            id = READAS(StringId);
            result = state_getinterned(state, id);
            PUSH_VALUE(result);
            break;

        case OPCODE_ADD:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_add(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_SUB:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_subtract(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_MUL:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_multiply(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_DIV:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_divide(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_NEG:
            l = POP_VALUE();
            result = nom_negate(state, l);
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
            state->ip = POP_FRAME();
            break;

        case OPCODE_VALUE_LET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_insert(state, r, l, TOP_VALUE()))
            {
                state_seterror(state, "Value for key '%s' already exists", nom_getstring(state, l));
            }
            break;

        case OPCODE_VALUE_SET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_set(state, r, l, TOP_VALUE()))
            {
                state_seterror(state, "No value for key '%s'", nom_getstring(state, l));
            }
            break;

        case OPCODE_VALUE_GET:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_tryget(state, r, l, &result))
            {
                state_seterror(state, "No value for key '%s'", nom_getstring(state, l));
            }
            else
            {
                PUSH_VALUE(result);
            }
            break;

        case OPCODE_BRACKET_SET:
            l = POP_VALUE();
            r = POP_VALUE();
            nom_insertorset(state, r, l, TOP_VALUE());
            break;

        case OPCODE_BRACKET_GET:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_get(state, r, l);
            PUSH_VALUE(result);
            break;

        case OPCODE_PUSH:
            result = READAS(NomValue);
            PUSH_VALUE(result);
            break;

        case OPCODE_POP:
            (void)POP_VALUE();
            break;

        case OPCODE_MAP:
            count = READAS(uint32_t);
            result = nom_newmap(state);
            for (uint32_t i = 0; i < count; ++i)
            {
                NomValue key = POP_VALUE();
                NomValue value = POP_VALUE();

                map_insertorset(state, result, key, value);
            }
            PUSH_VALUE(result);
            break;

        case OPCODE_FUNCTION:
            ip = READAS(uint32_t);
            count = READAS(uint32_t);
            result = function_new(state, ip);
            for (uint32_t i = 0; i < count; ++i)
            {
                StringId parameter = READAS(StringId);
                function_addparam(state, result, parameter);
            }
            PUSH_VALUE(result);
            break;

        case OPCODE_GOTO:
            ip = READAS(uint32_t);
            state->ip = ip;
            break;

        case OPCODE_INVOKE:
            count = READAS(uint32_t);
            result = POP_VALUE();
            if (function_check(result))
            {
                PUSH_FRAME(state->ip, nom_newmap(state));
                state->ip = function_getip(state, result);
                size_t paramcount = function_getparamcount(state, result);
                if (count <= paramcount)
                {
                    size_t i = paramcount;
                    while (i > 0)
                    {
                        --i;
                        NomValue arg = nom_nil();
                        if (i < count)
                        {
                            arg = POP_VALUE();
                        }
                        StringId param = function_getparam(state, result, i);
                        state_letinterned(state, param, arg);
                    }
                }
                else
                {
                    state_seterror(state, "Too many arguments given (expected %u)", paramcount);
                }
            }
            else
            {
                state_seterror(state, "Value cannot be invoked");
            }
            break;
        }
    }
        
    result = nom_nil();
    if (!state->errorflag && state->sp > 0)
    {
        result = POP_VALUE();
    }

    state->sp = 0;
    state->cp = 1;
    state->ip = state->end;

    return result;
}

void nom_dumpbytecode(
    NomState*   state,
    const char* source
    )
{
    assert(state);

    if (source)
    {
        compile(state, source);
    }
    else
    {
        state->ip = 0;
    }

    while (state->ip < state->end)
    {
        OpCode op = (OpCode)state->bytecode[state->ip++];
        printf("0x%08x: %s\t", state->ip - 1, OPCODE_NAMES[op]);

        switch (op)
        {
        case OPCODE_LET:
        case OPCODE_GET:
        case OPCODE_SET:
        {
            StringId id = READAS(StringId);
            const char* string = stringpool_find(state->stringpool, id);
            printf("\t%s", string);
        } break;

        case OPCODE_PUSH:
        {
            NomValue value = READAS(NomValue);
            char buffer[256];
            nom_tostring(state, buffer, 256, value);
            printf("%s", buffer);
        } break;

        case OPCODE_MAP:
        {
            uint32_t itemCount = READAS(uint32_t);
            printf("\t%u", itemCount);
        } break;

        case OPCODE_FUNCTION:
        {
            uint32_t ip = READAS(uint32_t);
            uint32_t paramcount = READAS(uint32_t);
            printf("0x%x %u ", ip, paramcount);
            for (uint32_t i = 0; i < paramcount; ++i)
            {
                StringId id = READAS(StringId);
                const char* param = stringpool_find(state->stringpool, id);
                printf("%s", param);
                if (i < paramcount - 1)
                {
                    printf(" ");
                }
            }
        } break;

        case OPCODE_GOTO:
        {
            uint32_t ip = READAS(uint32_t);
            printf("0x%x", ip);
        } break;

        case OPCODE_INVOKE:
        {
            uint32_t argcount = READAS(uint32_t);
            printf("%u", argcount);
        } break;

        default:
            break;
        }

        printf("\n");
    }

    state->ip = state->end;
}

bool nom_error(
    NomState*   state
    )
{
    assert(state);
    return state->errorflag;
}

const char* nom_geterror(
    NomState*   state
    )
{
    assert(state);
    state->errorflag = false;
    return state->error;
}
