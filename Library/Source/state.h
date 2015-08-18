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
#ifndef STATE_H
#define STATE_H

#include "heap.h"
#include "stringpool.h"

#include <nominal.h>

#define STATE_MAX_STACK_SIZE        (128)
#define STATE_MAX_CALLSTACK_SIZE    (32)
#define STATE_MAX_BYTE_CODE         (8096)
#define STATE_STRING_POOL_SIZE      (512)

typedef struct StackFrame
{
    uint32_t    ip;
    uint8_t     argcount;
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

    NomValue        nilclass;
    NomValue        numberclass;
    NomValue        booleanclass;
    NomValue        stringclass;
    NomValue        mapclass;
    NomValue        functionclass;
    NomValue        classclass;

    char            error[2048];
    bool            errorflag;
};

// Declares the value of a variable given an interned string ID based
// on the current scope
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
void state_letinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
);

// Sets the value of a variable given an interned string ID based
// on the current scope
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
void state_setinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
);

// Returns the value of a variable given an interned string ID based
// on the current scope
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
NomValue state_getinterned(
    NomState*   state,
    StringId    id
);

// Invokes a function
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
NomValue state_invoke(
    NomState*   state,
    NomValue    value,
    uint8_t     argcount,
    NomValue*   args
);

// Begins execution at the current instruction pointer
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
NomValue state_execute(
    NomState*   state
);

// Creates a new map representing a class
//
// The call may have encountered an error; check nom_error() directly
// after invoking this function
NomValue state_newclass(
    NomState*   state,
    const char* name);

#endif
