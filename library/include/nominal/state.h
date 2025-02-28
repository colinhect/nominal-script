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
#ifndef NOM_STATE_H
#define NOM_STATE_H

#include "nominal/export.h"
#include "nominal/value.h"

///
/// \brief Creates a new Nominal state.
///
/// \returns A pointer to the new Nominal state.
NOM_EXPORT NomState* nom_newstate(
    void
);

///
/// \brief Frees a Nominal state
///
/// \param state
///     The Nominal state to free.
NOM_EXPORT void nom_freestate(
    NomState*   state
);

///
/// \brief Imports a module.
///
/// \note If the module was already imported then no action is taken.
///
/// \warning This function could have encountered an error.  Check nom_error()
///          directly after calling this function.
///
/// \param state
///     The state.
/// \param module
///     The module to import.
///
/// \returns The imported module.
NOM_EXPORT NomValue nom_import(
    NomState*   state,
    const char* module
);

///
/// \brief Declares a variable in the current scope.
///
/// \warning This function could have encountered an error.  Check nom_error()
///          directly after calling this function.
///
/// \param state
///     The state.
/// \param identifier
///     The identifier of the variable.
/// \param value
///     The value of the variable.
NOM_EXPORT void nom_letvar(
    NomState*   state,
    const char* identifier,
    NomValue    value
);

///
/// \brief Sets the value of an existing variable accessible from the current
///        scope.
///
/// \warning This function could have encountered an error.  Check nom_error()
///          directly after calling this function.
///
/// \param state
///     The state.
/// \param identifier
///     The identifier of the variable.
/// \param value
///     The value of the variable.
NOM_EXPORT void nom_setvar(
    NomState*   state,
    const char* identifier,
    NomValue    value
);

///
/// \brief Gets the value of an existing variable accessible from the current
///        scope.
///
/// \warning This function could have encountered an error.  Check nom_error()
///          directly after calling this function.
///
/// \param state
///     The state.
/// \param identifier
///     The identifier of the variable.
///
/// \returns The value of the variable.
NOM_EXPORT NomValue nom_getvar(
    NomState*   state,
    const char* identifier
);

///
/// \brief Returns the number of available arguments on the call stack.
///
/// \param state
///     The state.
///
/// \returns The number of arguments.
NOM_EXPORT size_t nom_getargcount(
    NomState*   state
);

///
/// \brief Gets the value of the argument at the given index on the call
///        stack.
///
/// \param state
///     The state.
/// \param index
///     The index of the argument.
///
/// \returns The value of the argument; nil if the index is greater than the
///          number of arguments currently on the call stack.
NOM_EXPORT NomValue nom_getarg(
    NomState*   state,
    size_t      index
);

///
/// \brief Executes a snippet of Nominal source code.
///
/// \warning The execution could have encountered an error.  Check
///          nom_error() directly after calling this function.
///
/// \param state
///     The state.
/// \param source
///     The Nominal source code.
NOM_EXPORT void nom_execute(
    NomState*   state,
    const char* source
);

///
/// \brief Executes a snippet of Nominal source code and returns the resulting
///        value.
///
/// \warning The execution could have encountered an error.  Check
///          nom_error() directly after calling this function.
///
/// \param state
///     The state.
/// \param source
///     The Nominal source code.
///
/// \returns The resulting value of the expression.
NOM_EXPORT NomValue nom_evaluate(
    NomState*   state,
    const char* source
);

///
/// \brief Executes a file containing Nominal source code.
///
/// \warning The execution could have encountered an error.  Check
///          nom_error() directly after calling this function.
///
/// \param state
///     The state.
/// \param path
///     The path to the Nominal source code file.
NOM_EXPORT void nom_dofile(
    NomState*   state,
    const char* path
);

///
/// \brief Prints the byte code resulting from a snippet of Nominal source
///        code to stdout.
///
/// \warning The source code could have failed to parse.  Check nom_error()
///          directly after calling this function.
///
/// \param state
///     The state.
/// \param source
///     The Nominal source code.  If NULL, then all of the state's byte code
///     is printed.
///
/// \returns The result of the executed code.
NOM_EXPORT void nom_dumpbytecode(
    NomState*   state,
    const char* source
);

///
/// \brief Returns whether an error has occurred.
///
/// \param state
///     The state.
NOM_EXPORT bool nom_error(
    NomState*   state
);

///
/// \brief Sets an error that has occurred.
///
/// \param state
///     The state
/// \param fmt
///     The formatted error message.
NOM_EXPORT void nom_seterror(
    NomState*   state,
    const char* fmt,
    ...
);

///
/// \brief Returns the message of the last occuring error.
///
/// \param state
///     The state.
NOM_EXPORT const char* nom_geterror(
    NomState*   state
);

///
/// \brief Reclaims all unreferenced objects.
///
/// \param state
///     The state.
///
/// \returns The number of objects reclaimed.
NOM_EXPORT int nom_collectgarbage(
    NomState*   state
);

#endif
