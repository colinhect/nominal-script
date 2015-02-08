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
#ifndef NOMINAL_MAP_H
#define NOMINAL_MAP_H

#include "Nominal/Export.h"
#include "Nominal/Value.h"

#include <stddef.h>

///
/// \brief Checks if a value is a map.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a map; false otherwise.
NOMINAL_EXPORT bool Nom_IsMap(
    NomValue    value
    );

///
/// \brief Creates a new Nominal map.
///
/// \param state
///     The state to create the value for.
///
/// \returns The new Nominal map.
NOMINAL_EXPORT NomValue Nom_NewMap(
    NomState*   state
    );

#endif
