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
#ifndef NOMINAL_NUMBER_H
#define NOMINAL_NUMBER_H

#include "Nominal/Value.h"

#include <limits.h>

///
/// \brief Checks if a value is a number.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a number; false otherwise.
bool NomNumber_Check(
    NomValue    value
    );

///
/// \brief Creates a Nominal number value from an int.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromInt(
    int             value
    );

///
/// \brief Creates a Nominal number value from an unsigned int.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromUnsignedInt(
    unsigned int    value
    );

///
/// \brief Creates a Nominal number value from an long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromLong(
    long            value
    );

///
/// \brief Creates a Nominal number value from an unsigned long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromUnsignedLong(
    unsigned long   value
    );

///
/// \brief Creates a Nominal number value from a long long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromLongLong(
    long long           value
    );

///
/// \brief Creates a Nominal number value from an unsigned long long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromUnsignedLongLong(
    unsigned long long  value
    );

///
/// \brief Creates a Nominal number value from a size.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromSize(
    size_t  value
    );

///
/// \brief Creates a Nominal number value from a float.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromFloat(
    float       value
    );

///
/// \brief Creates a Nominal number value from a double.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NomValue NomNumber_FromDouble(
    double      value
    );

///
/// \brief Returns the Nominal number as an int.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (INT_MAX if the value is not a number).
int NomNumber_AsInt(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned int.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (UINT_MAX if the value is not a number).
unsigned int NomNumber_AsUnsignedInt(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (LONG_MAX if the value is not a number).
long NomNumber_AsLong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (ULONG_MAX if the value is not a number).
unsigned long NomNumber_AsUnsignedLong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a long long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (LLONG_MAX if the value is not a number).
long long NomNumber_AsLongLong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned long long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (ULLONG_MAX if the value is not a number).
unsigned long long NomNumber_AsUnsignedLongLong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a size.
///
/// \param value
///     The number value.
///
/// \returns The resulting value ((size_t)-1 if the value is not a number).
size_t NomNumber_AsSize(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a float.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (NAN if the value is not a number).
float NomNumber_AsFloat(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a double.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (NAN if the value is not a number).
double NomNumber_AsDouble(
    NomValue    value
    );

#endif