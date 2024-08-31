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
#ifndef NOM_VALUE_H
#define NOM_VALUE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nominal/export.h"

///
/// \brief A self-contained instance of a Nominal compiler and virtual
///        machine.
typedef struct NomState NomState;

///
/// \brief A Nominal value.
typedef union
{
    uint64_t    raw;
    double      number;

    struct
    {
        uint32_t    lower;
        uint32_t    upper;
    } data;
} NomValue;

///
/// \brief An iterator for a Nominal value.
typedef struct NomIterator
{
    NomValue    source;
    NomValue    key;
    NomValue    value;

    union
    {
        struct
        {
            size_t  index;
        } map;
    } data;
} NomIterator;

///
/// \brief Returns the value representing nil.
NOM_EXPORT NomValue nom_nil(
    void
);

///
/// \brief Returns the value representing true.
NOM_EXPORT NomValue nom_true(
    void
);

///
/// \brief Returns the value representing false.
NOM_EXPORT NomValue nom_false(
    void
);

///
/// \brief Returns whether a Nominal value is true.
///
/// \param state
///     The state.
/// \param value
///     The value in question.
///
/// \returns True if the value is true; false otherwise.
NOM_EXPORT bool nom_istrue(
    NomState*   state,
    NomValue    value
);

///
/// \brief Returns whether a Nominal value is nil.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is nil; false otherwise.
NOM_EXPORT bool nom_isnil(
    NomValue    value
);

///
/// \brief Returns whether a Nominal value is a reference to an object.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a reference to an object; false otherwise.
NOM_EXPORT bool nom_isobject(
    NomValue    value
);

///
/// \brief Returns whether a Nominal value is a reference to a class.
///
/// \param state
///     The state.
/// \param value
///     The value in question.
///
/// \returns True if the value is a reference to a class; false otherwise.
NOM_EXPORT bool nom_isclass(
    NomState*   state,
    NomValue    value
);

///
/// \brief Returns whether a value is equal to another.
///
/// \param state
///     The state.
/// \param left
///     The left value to compare.
/// \param right
///     The right value to compare.
///
/// \returns True if they are equal; false otherwise.
NOM_EXPORT bool nom_equals(
    NomState*   state,
    NomValue    left,
    NomValue    right
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
NOM_EXPORT long long nom_hash(
    NomState*   state,
    NomValue    value
);

///
/// \brief Gets a representation of a Nominal value as a string.
///
/// \param state
///     The state.
/// \param buffer
///     The destination string to copy to.
/// \param buffersize
///     The size of the buffer.
/// \param value
///     The value.
///
/// \returns The number of characters written to the buffer.
NOM_EXPORT size_t nom_tostring(
    NomState*   state,
    char*       buffer,
    size_t      buffersize,
    NomValue    value
);

///
/// \brief Adds another value to a Nominal value.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NOM_EXPORT NomValue nom_add(
    NomState*   state,
    NomValue    left,
    NomValue    right
);

///
/// \brief Subtracts another value from a Nominal value.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NOM_EXPORT NomValue nom_sub(
    NomState*   state,
    NomValue    left,
    NomValue    right
);

///
/// \brief Multiplies a Nominal value by another value.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NOM_EXPORT NomValue nom_mul(
    NomState*   state,
    NomValue    left,
    NomValue    right
);

///
/// \brief Divides a Nominal value by another value.
///
/// \param state
///     The state.
/// \param left
///     The left value.
/// \param right
///     The right value.
///
/// \returns The resulting value.
NOM_EXPORT NomValue nom_div(
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
NOM_EXPORT NomValue nom_neg(
    NomState*   state,
    NomValue    value
);

///
/// \brief Returns whether a Nominal value can be called.
///
/// \param state
///     The state.
/// \param value
///     The value in question.
///
/// \returns True if the value can be called; false otherwise.
NOM_EXPORT bool nom_iscallable(
    NomState*   state,
    NomValue    value
);

///
/// \brief Calls a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value to call.
/// \param argcount
///     The number of arguments to pass.
/// \param args
///     The array of argument values.
///
/// \returns The result of calling the value.
NOM_EXPORT NomValue nom_call(
    NomState*   state,
    NomValue    value,
    uint8_t     argcount,
    NomValue*   args
);

///
/// \brief Returns whether a Nominal value can be iterated over.
///
/// \param state
///     The state.
/// \param value
///     The value in question.
///
/// \returns True if the value can be iterated over; false otherwise.
NOM_EXPORT bool nom_isiterable(
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
///          pairs in the value or if the value is not iterable.
NOM_EXPORT bool nom_next(
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
/// \param keyvalue
///     The value to insert for the key.
///
/// \returns True if the value was inserted for they key; false if a value
///          already exists for the key or the value does not support settings
///          values for keys.
NOM_EXPORT bool nom_insert(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
);

///
/// \brief Updates a value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to set the value for.
/// \param keyvalue
///     The value to set for the key.
///
/// \returns True if the value was update for they key; false if a value does
///          not exist for the key or the value does not support updating
///          values for keys.
NOM_EXPORT bool nom_update(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
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
/// \param keyvalue
///     Returns the found value for the key; nil if no value exists for the
///     key.
///
/// \returns True if the value was found for the key; false otherwise.
NOM_EXPORT bool nom_find(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue*   keyvalue
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
NOM_EXPORT NomValue nom_get(
    NomState*   state,
    NomValue    value,
    NomValue    key
);

///
/// \brief Inserts or updates a value for a key in a Nominal value.
///
/// \param state
///     The state.
/// \param value
///     The value.
/// \param key
///     The key to insert or set the value for.
/// \param keyvalue
///     The value to insert or set for the key.
///
/// \returns True if the value was inserted for they key; false if a value
///          already exists for the key.
NOM_EXPORT bool nom_set(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
);

///
/// \brief Acquires a reference to a Nominal value, assuring that the value
///        will not be garbage collected until after it is released.
///
/// \param state
///     The state.
/// \param value
///     The value.
NOM_EXPORT void nom_acquire(
    NomState*   state,
    NomValue    value
);

///
/// \brief Releases a reference to a Nominal value, allowing the value to be
///        garbage collected.
///
/// \param state
///     The state.
/// \param value
///     The value.
NOM_EXPORT void nom_release(
    NomState*   state,
    NomValue    value
);

#endif
