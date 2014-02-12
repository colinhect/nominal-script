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
#ifndef NOMINAL_VALUE_H
#define NOMINAL_VALUE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct _NomState NomState;

///
/// \brief A Nominal value.
typedef struct
{
    uint64_t data;
} NomValue;

///
/// \brief Creates a new Nominal nil value.
NomValue NomValue_Nil(
    void
    );

///
/// \brief Returns whether a Nominal value is equal to another.
///
/// \param state
///     The state.
/// \param value
///     The value to compare.
/// \param other
///     The other value to compare.
///
/// \returns True if they are equal; false otherwise.
bool NomValue_Equals(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Compares a Nominal value with another Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param other
///     The value to compare to.
///
/// \returns 0 if the values are equivalent; -1 if the value is less than the
/// other; 1 if the value is greater than the other.
int NomValue_Compare(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Gets a representation of a Nominal value as a string.
///
/// \param state
///     The state.
/// \param dest
///     The destination string to copy to.
/// \param value
///     The value.
void NomValue_AsString(
    NomState*   state,
    char*       dest,
    NomValue    value
    );

///
/// \brief Performs addition on two Nominal values.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NomValue NomValue_Add(
    NomState*   state,
    NomValue    left,
    NomValue    right
    );

///
/// \brief Performs subtraction on two Nominal values.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NomValue NomValue_Subtract(
    NomState*   state,
    NomValue    left,
    NomValue    right
    );

///
/// \brief Performs multiplication on two Nominal values.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NomValue NomValue_Multiply(
    NomState*   state,
    NomValue    left,
    NomValue    right
    );

///
/// \brief Performs division on two Nominal values.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NomValue NomValue_Divide(
    NomState*   state,
    NomValue    left,
    NomValue    right
    );

///
/// \brief Negates a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to negate.
///
/// \returns The resulting value.
NomValue NomValue_Negate(
    NomState*   state,
    NomValue    value
    );

#endif