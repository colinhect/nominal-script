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
#ifndef NOMINAL_STRING_H
#define NOMINAL_STRING_H

#include "Nominal/Value.h"

///
/// \brief Checks if a value is a string.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a string; false otherwise.
bool NomString_Check(
    NomValue    value
    );

///
/// \brief Creates a new Nominal string value from a string.
///
/// \param state
///     The state to create the value for.
/// \param value
///     The NULL-terminated string value.
/// \param pooled
///     Whether the string should be pooled (normally false).
///
/// \returns The new Nominal value.
NomValue NomString_FromString(
    NomState*   state,
    const char* value,
    bool        pooled
    );

///
/// \brief Returns the value of a Nominal string as a string.
///
/// \param state
///     The state.
/// \param value
///     The string value.
///
/// \returns A pointer to a NULL terminated string; NULL if the value is not
/// a string.
const char* NomString_AsString(
    NomState*   state,
    NomValue    value
    );

#endif