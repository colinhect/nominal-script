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
#ifndef MAP_H
#define MAP_H

#include "Nominal/Map.h"

// Returns whether the keys in a map are contiguous natural numbers starting
// at zero
bool Map_IsContiguous(
    NomState*   state,
    NomValue    map
    );

// Moves to the next key/value pair in a map (moves to the first pair if the
// iterator is initialized to zero)
bool Map_MoveNext(
    NomState*       state,
    NomValue        map,
    NomIterator*    iterator
    );

// Inserts a new key/value in a map, returning true if the key/value was
// inserted or false if a value already exists for the key
bool Map_Insert(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

// Sets the value for a key in a map, returning true if the set was successful
// or false if no value exists for the specified key
bool Map_Set(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

// Inserts a new value or sets an existing value in a map, returning true if a
// new value was inserted or false if an existing value was changed
bool Map_InsertOrSet(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

// Gets the value for a key in a map, returning nil no value is set for the
// specified key
NomValue Map_Get(
    NomState*   state,
    NomValue    map,
    NomValue    key
    );

// Tries to get the value for a key in a map, returning true if the value was
// found or false otherwise
bool Map_TryGet(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue*   value
    );

#endif
