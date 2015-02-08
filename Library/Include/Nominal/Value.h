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
#ifndef NOMINAL_VALUE_H
#define NOMINAL_VALUE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "Nominal/Export.h"

///
/// \brief A Nominal state.
typedef struct NomState NomState;

///
/// \brief A Nominal value.
typedef union
{
    uint64_t    raw;

    struct
    {
        uint32_t    lower;
        uint32_t    upper;
    } data;

    double      number;
} NomValue;

///
/// \brief An iterator for a Nominal value.
typedef struct
{
    NomValue    source;
    NomValue    key;
    NomValue    value;
    union
    {
        // Map-related iterator data
        struct
        {
            size_t  index;
        } map;
    } data;
} NomIterator;

///
/// \brief Returns the value representing nil.
NOMINAL_EXPORT NomValue Nom_Nil(
    void
    );

///
/// \brief Returns the value representing true.
NOMINAL_EXPORT NomValue Nom_True(
    void
    );

///
/// \brief Returns the value representing true.
NOMINAL_EXPORT NomValue Nom_False(
    void
    );

///
/// \brief Returns whether a value is equal to another.
///
/// \param state
///     The state.
/// \param value
///     The value to compare.
/// \param other
///     The other value to compare.
///
/// \returns True if they are equal; false otherwise.
NOMINAL_EXPORT bool Nom_Equals(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Returns the hash of a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to hash.
///
/// \returns The hash of the value.
NOMINAL_EXPORT long long Nom_Hash(
    NomState*   state,
    NomValue    value
    );

///
/// \brief Gets a representation of a value as a string.
///
/// \param state
///     The state.
/// \param buffer
///     The destination string to copy to.
/// \param bufferSize
///     The size of the buffer.
/// \param value
///     The value.
///
/// \returns The number of characters written to the buffer.
NOMINAL_EXPORT size_t Nom_AsString(
    NomState*   state,
    char*       buffer,
    size_t      bufferSize,
    NomValue    value
    );

///
/// \brief Adds another value to a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to add to.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NOMINAL_EXPORT NomValue Nom_Add(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Subtracts another value from a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to subtract from.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NOMINAL_EXPORT NomValue Nom_Subtract(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Multiplies a value by another value.
///
/// \param state
///     The state.
/// \param value
///     The value to multiply.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NOMINAL_EXPORT NomValue Nom_Multiply(
    NomState*   state,
    NomValue    value,
    NomValue    other
    );

///
/// \brief Divides a value by another value.
///
/// \param state
///     The state.
/// \param value
///     The value to divide.
/// \param other
///     The other value.
///
/// \returns The resulting value.
NOMINAL_EXPORT NomValue Nom_Divide(
    NomState*   state,
    NomValue    value,
    NomValue    other
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
NOMINAL_EXPORT NomValue Nom_Negate(
    NomState*   state,
    NomValue    value
    );

///
/// \brief Returns whether a Nominal value can be iterated over.
///
/// \param state
///     The state.
/// \param value
///     The value.
///
/// \returns True if the value can be iterated over; false otherwise.
NOMINAL_EXPORT bool Nom_IsIterable(
    NomState*   state,
    NomValue    value
    );

///
/// \brief Moves to the next key/value pair in the Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param iterator
///     The iterator.  If initialized to zero then it will be intialized and
///     moved to the first pair in the value.
///
/// \returns True if the next pair was moved to; false if there were no more
/// pairs in the value or if the value is not iterable.
NOMINAL_EXPORT bool Nom_MoveNext(
    NomState*       state,
    NomValue        value,
    NomIterator*    iterator
    );

///
/// \brief Inserts a new value into a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to insert into.
/// \param key
///     The key to insert the value for.
/// \param newValue
///     The value to insert for the key.
///
/// \returns True if the value was inserted for they key; false if a value
/// already exists for the key or the value does not support settings values
/// for keys.
NOMINAL_EXPORT bool Nom_Insert(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    );

///
/// \brief Sets a value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to set the value for.
/// \param newValue
///     The value to set for the key.
///
/// \returns True if the value was set for they key; false if a value does
/// not exist for the key or the value does not support setting values for
/// keys.
NOMINAL_EXPORT bool Nom_Set(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    );

///
/// \brief Inserts or sets a value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to insert or set the value for.
/// \param newValue
///     The value to insert or set for the key.
///
/// \returns True if the value was inserted for they key; false if a value
/// already exists for the key.
NOMINAL_EXPORT bool Nom_InsertOrSet(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    );

///
/// \brief Gets the value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to get the value for.
///
/// \returns The value for the key; nil of no value exists for the key.
NOMINAL_EXPORT NomValue Nom_Get(
    NomState*   state,
    NomValue    value,
    NomValue    key
    );

///
/// \brief Gets the value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to get the value for.
/// \param keyValue
///     Returns the found value for the key; nil if no value exists for the
///     key.
///
/// \returns True if the value was found for the key; false otherwise.
NOMINAL_EXPORT bool Nom_TryGet(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue*   keyValue
    );

#endif
