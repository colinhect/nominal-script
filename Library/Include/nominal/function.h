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
/// \file
///////////////////////////////////////////////////////////////////////////////
#ifndef NOM_FUNCTION_H
#define NOM_FUNCTION_H

#include "nominal/export.h"
#include "nominal/value.h"
#include "nominal/state.h"

///
/// \brief A function pointer to a native function invoked in the place of a
/// Nominal function.
typedef NomValue(*NomFunction)(NomState* state);

///
/// \brief Checks if a value is a function.
///
/// \param state
///     The state.
/// \param value
///     The value in question.
///
/// \returns True if the value is a function; false otherwise.
NOM_EXPORT bool nom_isfunction(
    NomState*   state,
    NomValue    value
);

///
/// \brief Creates a new Nominal function.
///
/// \param state
///     The state to create the value for.
/// \param function
///     The native function callback.
///
/// \returns The new Nominal function (must be released).
NOM_EXPORT NomValue nom_newfunction(
    NomState*   state,
    NomFunction function
);

#endif
