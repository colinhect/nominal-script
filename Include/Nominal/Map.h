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

#include "Nominal/State.h"
#include "Nominal/Value.h"

///
/// \brief Checks if a Nominal value is a map.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a map; false otherwise.
bool NomMap_Check(
    NomValue value
    );

///
/// \brief Creates a new Nominal map.
///
/// \param state
///     The state.
///
/// \returns The new Nominal map.
NomValue NomMap_Create(
    NomState*    state
    );

///
/// \brief Sets the value for a key in a Nominal map.
///
/// \param state
///        The state.
/// \param map
///        The map to set the value in.
/// \param key
///        The key.
///    \param value
///        The new value.
///
/// \returns False if the map already had a value for the given key; true
/// otherwise.
bool NomMap_Set(
    NomState*    state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    );

///
/// \brief Gets the value for a key in a Nominal map.
///
/// \param state
///        The state.
/// \param map
///        The map to get the value from.
/// \param key
///        The key.
///
/// \returns The value for the given key; nil if no value is set for the given
/// key.
NomValue NomMap_Get(
    NomState*    state,
    NomValue    map,
    NomValue    key
    );

///
/// \brief Tries to get the value for a key in a Nominal map.
///
/// \param state
///        The state.
/// \param map
///        The map to get the value from.
/// \param key
///        The key.
/// \param value
///        The value (nil if no value is set for the given key)..
///
/// \returns True if the value was found; false otherwise.
bool NomMap_TryGet(
    NomState*    state,
    NomValue    map,
    NomValue    key,
    NomValue*   value
    );

#endif