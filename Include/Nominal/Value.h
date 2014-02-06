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
#ifndef NOMINAL_VALUE_H
#define NOMINAL_VALUE_H

#include <stdint.h>

typedef struct _NomState NomState;

///
/// \brief Enumeration of each type a Nominal value can be.
typedef enum
{
    NOM_TYPE_NIL,
    NOM_TYPE_INTEGER,
    NOM_TYPE_REAL,
    NOM_TYPE_BOOLEAN,
    NOM_TYPE_STRING,
    NOM_TYPE_STATIC_STRING,
    NOM_TYPE_MAP,
    NOM_TYPE_FUNCTION
} NomType;

///
/// \brief A Nominal value.
typedef union
{
    struct
    {
        char type;
        union
        {
            int         integerValue;
            float       realValue;
            unsigned    handle;
        } data;
    } fields;
    uint64_t raw;
} NomValue;

///
/// \brief The constant value for nil.
NomValue NOM_NIL;

///
/// \brief Checks whether a Nominal value is a real or integer.
///
/// \param value
///     The value in question.
///
/// \returns 1 if the value is a real or integer; 0 otherwise.
int NomValue_IsNumber(
    NomValue    value
    );

///
/// \brief Returns the Nominal value coerced to an int.
///
/// \returns The resulting value.
int NomValue_AsInt(
    NomValue    value
    );

///
/// \brief Returns the Nominal value coerced to a float.
///
/// \returns The resulting value.
float NomValue_AsFloat(
    NomValue    value
    );

///
/// \brief Returns the Nominal value coerced to a double.
///
/// \returns The resulting value.
double NomValue_AsDouble(
    NomValue    value
    );

void NomValue_AsString(
    NomState*   state,
    char*       dest,
    NomValue    value);

#endif