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
#ifndef NOM_NUMBER_H
#define NOM_NUMBER_H

#include "nominal/export.h"
#include "nominal/value.h"

#include <stddef.h>
#include <limits.h>

///
/// \brief Checks if a value is a Nominal number.
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a Nominal number; false otherwise.
NOM_EXPORT bool nom_isnumber(
    NomValue    value
);

///
/// \brief Creates a Nominal number from an int.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromint(
    int             value
);

///
/// \brief Creates a Nominal number from an unsigned int.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromunsignedint(
    unsigned int    value
);

///
/// \brief Creates a Nominal number from an long.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromlong(
    long            value
);

///
/// \brief Creates a Nominal number from an unsigned long.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromunsignedlong(
    unsigned long   value
);

///
/// \brief Creates a Nominal number from a long long.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromlonglong(
    long long       value
);

///
/// \brief Creates a Nominal number from an unsigned long long.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromunsignedlonglong(
    unsigned long long  value
);

///
/// \brief Creates a Nominal number from a size.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromsize(
    size_t      value
);

///
/// \brief Creates a Nominal number from a float.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromfloat(
    float       value
);

///
/// \brief Creates a Nominal number from a double.
///
/// \param value
///     The numeric value for the new Nominal number.
///
/// \returns The new Nominal number.
NOM_EXPORT NomValue nom_fromdouble(
    double      value
);

///
/// \brief Returns the Nominal number as an int.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (INT_MAX if the value is not a number).
NOM_EXPORT int nom_toint(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as an unsigned int.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (UINT_MAX if the value is not a number).
NOM_EXPORT unsigned int nom_tounsignedint(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as a long.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (LONG_MAX if the value is not a number).
NOM_EXPORT long nom_tolong(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as an unsigned long.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (ULONG_MAX if the value is not a number).
NOM_EXPORT unsigned long nom_tounsignedlong(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as a long long.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (LLONG_MAX if the value is not a number).
NOM_EXPORT long long nom_tolonglong(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as an unsigned long
///        long.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (ULLONG_MAX if the value is not a number).
NOM_EXPORT unsigned long long nom_tounsignedlonglong(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as a size.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (-1 if the value is not a number).
NOM_EXPORT size_t nom_tosize(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as a float.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (NAN if the value is not a number).
NOM_EXPORT float nom_tofloat(
    NomValue    value
);

///
/// \brief Returns the numeric value of a Nominal number as a double.
///
/// \param value
///     The Nominal number to get the numeric value of.
///
/// \returns The resulting value (NAN if the value is not a number).
NOM_EXPORT double nom_todouble(
    NomValue    value
);

#endif
