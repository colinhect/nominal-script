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

///
/// \brief A Nominal state.
typedef struct _NomState NomState;

///
/// \brief Creates a new Nominal state.
///
/// \returns The new Nominal state.
NomState* NomState_Create(
    );

///
/// \brief Frees a Nominal state.
///
/// \param state
///     The state to free.
void NomState_Free(
    NomState*   state
    );

void NomState_Execute(
    NomState*   state,
    const char* source
    );

NomValue NomState_Pop(
    NomState*   state
    );

int NomState_ErrorOccurred(
    NomState*   state
    );

const char* NomState_GetError(
    NomState*   state
    );

#endif