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

#include "Nominal/Value.h"
#include "StringPool.h"

#include <stddef.h>

// Returns whether a value is a function
bool Function_Check(
    NomValue    value
    );

// Creates a new function
NomValue Function_New(
    NomState*   state,
    uint32_t    ip
    );

// Adds a parameter to a function
void Function_AddParameter(
    NomState*   state,
    NomValue    function,
    StringId    parameter
    );

// Returns the number of parameters a function has
size_t Function_GetParameterCount(
    NomState*   state,
    NomValue    function
    );

// Returns thh ID of the interned string representing the name of the
// parameter at the specified index in a function
StringId Function_GetParameter(
    NomState*   state,
    NomValue    function,
    size_t      index
    );

// Gets the instruction pointer of a function
uint32_t Function_GetInstructionPointer(
    NomState*   state,
    NomValue    function
    );

#endif
