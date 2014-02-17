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
///
/// \remarks Every Nominal value is created for a specific Nominal state
/// instance.  Mixing Nominal values from differring Nominal state instances
/// will result in undefined behavior.
typedef struct
{
    uint64_t data;
} NomValue;

///
/// \brief Creates a new Nominal nil value.
///
/// \param state
///     The Nominal state to create the value for.
///
/// \returns The nil value.
NomValue NomValue_Nil(
    NomState*   state
    );

///
/// \brief Returns whether a Nominal value is equal to another.
///
/// \param value
///     The value to compare.
/// \param other
///     The other value to compare.
///
/// \returns True if they are equal; false otherwise.
bool NomValue_Equals(
    NomValue    value,
    NomValue    other
    );

///
/// \brief Returns the hash of a Nominal value.
///
/// \param value
///     The value to hash.
///
/// \returns The hash of the value.
long long NomValue_Hash(
    NomValue    value
    );

///
/// \brief Gets a representation of a Nominal value as a string.
///
/// \param buffer
///     The destination string to copy to.
/// \param bufferSize
///     The size of the buffer.
/// \param value
///     The value.
///
/// \returns The number of characters written to the buffer.
size_t NomValue_AsString(
    char*       buffer,
    size_t      bufferSize,
    NomValue    value
    );

///
/// \brief Adds another value to a Nominal value.
///
/// \param value
///     The value to add to.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NomValue NomValue_Add(
    NomValue    value,
    NomValue    other
    );

///
/// \brief Subtracts another value from a Nominal value.
///
/// \param value
///     The value to subtract from.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NomValue NomValue_Subtract(
    NomValue    value,
    NomValue    other
    );

///
/// \brief Multiplies a Nominal value by another value.
///
/// \param value
///     The value to multiply.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NomValue NomValue_Multiply(
    NomValue    value,
    NomValue    other
    );

///
/// \brief Divides a Nominal value by another value.
///
/// \param value
///     The value to divide.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NomValue NomValue_Divide(
    NomValue    value,
    NomValue    other
    );

///
/// \brief Negates a Nominal value.
///
/// \param value
///     The value to negate.
///
/// \returns The resulting value.
NomValue NomValue_Negate(
    NomValue    value
    );

///
/// \brief Indexes a Nominal value by a key.
///
/// \param value
///     The value to index.
/// \param key
///     The key.
///
/// \returns The resulting value.
NomValue NomValue_Index(
    NomValue    value,
    NomValue    key
    );

///
/// \brief Gets a value's associated Nominal state.
///
/// \param value
///     The value.
///
/// \returns The associated state; NULL if no associated state is found.
NomState* NomValue_GetState(
    NomValue    value
    );

#endif