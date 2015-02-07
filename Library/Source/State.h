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

#include "Heap.h"
#include "StringPool.h"

#define STATE_MAX_STACK_SIZE        (128)
#define STATE_MAX_CALLSTACK_SIZE    (32)
#define STATE_MAX_BYTE_CODE         (8096)

///
/// \brief Returns the heap used by the specified state.
///
/// \param state
///     The state.
///
/// \returns The heap used by the state.
Heap* NomState_GetHeap(
    NomState*   state
    );

///
/// \brief Returns the string pool used by the specified state.
///
/// \param state
///     The state.
///
/// \returns The string pool used by the state.
StringPool* NomState_GetStringPool(
    NomState*   state
    );

///
/// \brief Declares the value of a variable given an interned string ID based
/// on the current scope.
///
/// \warning The call may have encountered an error.  Check
/// NomState_ErrorOccurred directly after invoking this function.
///
/// \param state
///     The state.
/// \param id
///     The interned string ID.
/// \param value
///     The value.
///
/// \returns The value.
void NomState_FastLet(
    NomState*   state,
    StringId    id,
    NomValue    value
    );

///
/// \brief Sets the value of a variable given an interned string ID based
/// on the current scope.
///
/// \warning The call may have encountered an error.  Check
/// NomState_ErrorOccurred directly after invoking this function.
///
/// \param state
///     The state.
/// \param id
///     The interned string ID.
/// \param value
///     The value.
///
/// \returns The value.
void NomState_FastSet(
    NomState*   state,
    StringId    id,
    NomValue    value
    );

///
/// \brief Returns the value of a variable given an interned string ID based
/// on the current scope.
///
/// \warning The call may have encountered an error.  Check
/// NomState_ErrorOccurred directly after invoking this function.
///
/// \param state
///     The state.
/// \param id
///     The interned string ID.
///
/// \returns The value.
NomValue NomState_FastGet(
    NomState*   state,
    StringId    id
    );

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
