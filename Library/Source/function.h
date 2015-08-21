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
#ifndef FUNCTION_H
#define FUNCTION_H

#include "stringpool.h"

#include <nominal.h>
#include <stddef.h>

#define MAX_FUNCTION_PARAMS (16)

// The internal data of a Nominal function
typedef struct FunctionData
{
    uint32_t    ip;
    NomFunction nativefunction;
    StringId    params[MAX_FUNCTION_PARAMS];
    size_t      paramcount;
} FunctionData;

// Creates a new function
NomValue function_new(
    NomState*   state,
    uint32_t    ip
);

// Adds a parameter to a function
void function_addparam(
    NomState*   state,
    NomValue    function,
    StringId    param
);

// Returns the number of parameters a function has
size_t function_getparamcount(
    NomState*   state,
    NomValue    function
);

// Returns the ID of the interned string representing the name of the
// parameter at the specified index in a function
StringId function_getparam(
    NomState*   state,
    NomValue    function,
    size_t      index
);

// Returns true if the function has a native callback or false otherwise
bool function_isnative(
    NomState*   state,
    NomValue    function
);

// Gets the native function pointer of a function
NomFunction function_getnative(
    NomState*   state,
    NomValue    function
);

// Gets the instruction pointer of a function
uint32_t function_getip(
    NomState*   state,
    NomValue    function
);

#endif
