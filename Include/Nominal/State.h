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
#ifndef NOMINAL_STATE_H
#define NOMINAL_STATE_H

#include "Nominal/Value.h"

#include <stdbool.h>

///
/// \brief A Nominal state.
typedef struct _NomState NomState;

///
/// \brief Creates a new Nominal state.
///
/// \returns The new Nominal state.
NomState* NomState_Create(
    void
    );

///
/// \brief Frees a Nominal state.
///
/// \param state
///     The state to free.
void NomState_Free(
    NomState*   state
    );

///
/// \brief Executes a snippet of Nominal source code and returns the value.
///
/// \remarks The execution could have encountered an error.  Check
/// NomState_ErrorOccurred directly after invoking this function.
///
/// \param state
///     The state.
/// \param source
///     The Nominal source code.
///
/// \returns The result of the executed code.
NomValue NomState_Execute(
    NomState*   state,
    const char* source
    );

///
/// \brief Returns whether an error has occurred.
///
/// \param state
///     The state.
bool NomState_ErrorOccurred(
    NomState*   state
    );

///
/// \brief Returns the error message of the last error.
///
/// \param state
///     The state.
const char* NomState_GetError(
    NomState*   state
    );

#endif