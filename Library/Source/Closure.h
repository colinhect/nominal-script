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
#ifndef CLOSURE_H
#define CLOSURE_H

#include "Nominal/Value.h"

#include <stddef.h>

///
/// \brief Checks if a value is a closure.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a closure; false otherwise.
bool NomClosure_Check(
    NomValue    value
    );

///
/// \brief Creates a new Nominal closure.
///
/// \param state
///     The state to create the value for.
/// \param ip
///     The instruction pointer where the closure begins.
///
/// \returns The new Nominal closure.
NomValue NomClosure_Create(
    NomState*   state,
    size_t      ip
    );

///
/// \brief Gets the instruction pointer of a closure.
///
/// \param state
///     The state.
/// \param closure
///     The closure.
///
/// \returns The instruction pointer where the closure begins.
size_t NomClosure_GetInstructionPointer(
    NomState*   state,
    NomValue    closure
    );

#endif
