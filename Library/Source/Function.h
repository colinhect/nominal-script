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

#include <stddef.h>

///
/// \brief Checks if a value is a function.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a function; false otherwise.
bool NomFunction_Check(
    NomValue    value
    );

///
/// \brief Creates a new Nominal function.
///
/// \param state
///     The state to create the value for.
/// \param ip
///     The instruction pointer where the function begins.
///
/// \returns The new Nominal function.
NomValue NomFunction_Create(
    NomState*   state,
    uint32_t    ip
    );

///
/// \brief Gets the instruction pointer of a function.
///
/// \param state
///     The state.
/// \param function
///     The function.
///
/// \returns The instruction pointer where the function begins.
uint32_t NomFunction_GetInstructionPointer(
    NomState*   state,
    NomValue    function
    );

#endif
