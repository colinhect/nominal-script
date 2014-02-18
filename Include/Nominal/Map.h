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
#ifndef NOMINAL_MAP_H
#define NOMINAL_MAP_H

#include "Nominal/Value.h"

///
/// \brief An iterator for a Nominal map.
typedef struct _NomMapIterator
{
    NomValue    map;
    NomValue    key;
    NomValue    value;
    size_t      index;
    void*       bucketNode;
} NomMapIterator;

///
/// \brief Checks if a Nominal value is a map.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a map; false otherwise.
bool NomMap_Check(
    NomValue    value
    );

///
/// \brief Creates a new Nominal map.
///
/// \param state
///     The Nominal state to create the value for.
///
/// \returns The new Nominal map.
NomValue NomMap_Create(
    NomState*   state
    );

///
/// \brief Moves to the next pair in the map.
///
/// \param map
///     The map.
/// \param iterator
///     The iterator.  If initialized to zero then it will be intialized and
///     moved to the first pair in the map.
///
/// \returns True if the next pair was moved to; false if there were no more
/// pairs in the map.
bool NomMap_MoveNext(
    NomValue        map,
    NomMapIterator* iterator
    );

///
/// \brief Inserts a new key/value in a Nominal map.
///
/// \param map
///     The map to insert the value in.
/// \param key
///     The key.
/// \param value
///     The new value.
///
/// \returns True if the key/value was inserted; false if a value already
/// exists for the key.
bool NomMap_Insert(
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

///
/// \brief Sets the value for a key in a Nominal map.
///
/// \param map
///     The map to set the value in.
/// \param key
///     The key.
/// \param value
///     The new value.
///
/// \returns True if the set was successful; false if no value exists for the
/// given key.
bool NomMap_Set(
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

///
/// \brief Inserts a new value or sets an existing value in a Nominal map.
///
/// \param map
///     The map to insert or set the value in.
/// \param key
///     The key.
/// \param value
///     The new value.
///
/// \returns True if a new value was inserted; false if an existing value was
/// changed.
bool NomMap_InsertOrSet(
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

///
/// \brief Gets the value for a key in a Nominal map.
///
/// \param map
///     The map to get the value from.
/// \param key
///     The key.
///
/// \returns The value for the given key; nil if no value is set for the given
/// key.
NomValue NomMap_Get(
    NomValue    map,
    NomValue    key
    );

///
/// \brief Tries to get the value for a key in a Nominal map.
///
/// \param map
///     The map to get the value from.
/// \param key
///     The key.
/// \param value
///     The value (nil if no value is set for the given key).
///
/// \returns True if the value was found; false otherwise.
bool NomMap_TryGet(
    NomValue    map,
    NomValue    key,
    NomValue*   value
    );

#endif