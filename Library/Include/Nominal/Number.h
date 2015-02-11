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
#ifndef NOMINAL_NUMBER_H
#define NOMINAL_NUMBER_H

#include "Nominal/Export.h"
#include "Nominal/Value.h"

#include <stddef.h>
#include <limits.h>

///
/// \brief Checks if a value is a number.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a number; false otherwise.
NOMINAL_EXPORT bool nom_isnumber(
    NomValue    value
    );

///
/// \brief Creates a Nominal number value from an int.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromint(
    int             value
    );

///
/// \brief Creates a Nominal number value from an unsigned int.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromunsignedint(
    unsigned int    value
    );

///
/// \brief Creates a Nominal number value from an long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromlong(
    long            value
    );

///
/// \brief Creates a Nominal number value from an unsigned long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromunsignedlong(
    unsigned long   value
    );

///
/// \brief Creates a Nominal number value from a long long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromlonglong(
    long long       value
    );

///
/// \brief Creates a Nominal number value from an unsigned long long.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromunsignedlonglong(
    unsigned long long  value
    );

///
/// \brief Creates a Nominal number value from a size.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromsize(
    size_t  value
    );

///
/// \brief Creates a Nominal number value from a float.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromfloat(
    float       value
    );

///
/// \brief Creates a Nominal number value from a double.
///
/// \param value
///     The value.
///
/// \returns The Nominal value.
NOMINAL_EXPORT NomValue nom_fromdouble(
    double      value
    );

///
/// \brief Returns the Nominal number as an int.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (INT_MAX if the value is not a number).
NOMINAL_EXPORT int nom_toint(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned int.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (UINT_MAX if the value is not a number).
NOMINAL_EXPORT unsigned int nom_tounsignedint(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (LONG_MAX if the value is not a number).
NOMINAL_EXPORT long nom_tolong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (ULONG_MAX if the value is not a number).
NOMINAL_EXPORT unsigned long nom_tounsignedlong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a long long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (LLONG_MAX if the value is not a number).
NOMINAL_EXPORT long long nom_tolonglong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as an unsigned long long.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (ULLONG_MAX if the value is not a number).
NOMINAL_EXPORT unsigned long long nom_tounsignedlonglong(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a size.
///
/// \param value
///     The number value.
///
/// \returns The resulting value ((size_t)-1 if the value is not a number).
NOMINAL_EXPORT size_t nom_tosize(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a float.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (NAN if the value is not a number).
NOMINAL_EXPORT float nom_tofloat(
    NomValue    value
    );

///
/// \brief Returns the Nominal number as a double.
///
/// \param value
///     The number value.
///
/// \returns The resulting value (NAN if the value is not a number).
NOMINAL_EXPORT double nom_todouble(
    NomValue    value
    );

#endif
