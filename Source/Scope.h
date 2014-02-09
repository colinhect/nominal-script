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
/// \brief Creates a new variable in the scope.
///
/// \param scope
///     The scope.
/// \param id
///     The string ID of the variable name.
/// \param value
///     The variable value.
///
/// \returns True if the new variable is created; false if the a varaible
/// of the same name already exists.
bool Scope_Let(
    Scope*      scope,
    StringId    id,
    NomValue    value
    );

///
/// \param Sets the value of an existing variable.
///
/// \param scope
///     The scope.
/// \param id
///     The string ID of the variable name.
/// \param value
///     The new value for the variable.
///
/// \returns True if the variable was set; false if no variable of that name
/// exists.
bool Scope_Set(
    Scope*      scope,
    StringId    id,
    NomValue    value
    );

///
/// \param Gets the value of of an existing variable.
///
/// \param scope
///     The scope.
/// \param id
///     The string ID of the variable name.
/// \param result
///     A pointer to the value to set to the result of the existing variable
///     on success.
///
/// \returns True if the variable was retrieved; false if no variable of that
/// name exists.
bool Scope_Get(
    Scope*      scope,
    StringId    id,
    NomValue*   result
    );

#endif