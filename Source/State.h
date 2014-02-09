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
#ifndef STATE_H
#define STATE_H

#include "Heap.h"
#include "StringPool.h"
#include "Scope.h"

#define STATE_MAX_STACK_SIZE    (1024)
#define STATE_MAX_BYTE_CODE     (8096)

typedef struct _NomState
{
    NomValue        stack[STATE_MAX_STACK_SIZE];
    size_t          sp;

    unsigned char   byteCode[STATE_MAX_BYTE_CODE];
    size_t          ip;

    Heap*           heap;
    StringPool*     stringPool;
    Scope*          globalScope;

    char            error[2048];
    bool            errorFlag;
} NomState;

///
/// \brief Sets the error message of the state.
///
/// \param state
///     The state.
/// \param fmt
///     The error message format.
void NomState_SetError(
    NomState*   state,
    const char* fmt,
    ...
    );

#endif