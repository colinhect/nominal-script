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
#ifndef SCOPE_H
#define SCOPE_H

#include "Nominal/Value.h"
#include "StringPool.h"

#include <stdbool.h>

///
/// \brief A scope.
typedef struct _Scope Scope;

///
/// \brief Creates a new scope.
///
/// \returns The new scope.
Scope* Scope_Create(
    void
    );

///
/// \brief Frees a scope.
///
/// \param scope
///     The scope to free.
void Scope_Free(
    Scope*  scope
    );

///
/// \brief Sets a new value
bool Scope_Let(
    Scope*      scope,
    StringId    id,
    NomValue    value
    );

bool Scope_Set(
    Scope*      scope,
    StringId    id,
    NomValue    value
    );

NomValue Scope_Get(
    Scope*      scope,
    StringId    id
    );

void Scope_Begin(
    Scope*      scope,
    StringId    id
    );

void Scope_End(
    Scope*      scope,
    StringId    id
    );

#endif