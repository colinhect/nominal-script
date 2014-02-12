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

///
/// \brief Checks if a Nominal value is a number (integer or real).
///
/// \param value
///     The value in question.
///
/// \returns True if the value is a number; false otherwise.
bool NomNumber_Check(
    NomValue    value
    );

///
/// \brief Returns the Nominal number coerced to an int.
///
/// \param value
///     The number value to coerce.
///
/// \returns The resulting value (-1 if the value is not a number).
int NomNumber_AsInt(
    NomValue    value
    );

///
/// \brief Returns the Nominal number coerced to a float.
///
/// \param value
///     The number value to coerce.
///
/// \returns The resulting value (NaN if the value is not a number).
float NomNumber_AsFloat(
    NomValue    value
    );

///
/// \brief Returns the Nominal number coerced to a double.
///
/// \param value
///     The number value to coerce.
///
/// \returns The resulting value (NaN if the value is not a number).
double NomNumber_AsDouble(
    NomValue    value
    );

#endif