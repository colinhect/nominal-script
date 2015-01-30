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
#include "Nominal/Value.h"
#include "Nominal/Number.h"

#include "Value.h"

#include <math.h>

#define FROM_NUMBER(v)      NomValue nv; nv.number = v; return nv
#define AS_NUMBER(v, t, e)  return IS_NUMBER(v) ? (t)v.number : e

bool NomNumber_Check(
    NomValue    value
    )
{
    return IS_NUMBER(value);
}

NomValue NomNumber_FromInt(
    int             value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromUnsignedInt(
    unsigned int    value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromLong(
    long            value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromUnsignedLong(
    unsigned long   value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromLongLong(
    long long           value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromUnsignedLongLong(
    unsigned long long  value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromSize(
    size_t  value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromFloat(
    float       value
    )
{
    FROM_NUMBER((double)value);
}

NomValue NomNumber_FromDouble(
    double      value
    )
{
    FROM_NUMBER(value);
}

int NomNumber_AsInt(
    NomValue    value
    )
{
    AS_NUMBER(value, int, INT_MAX);
}

unsigned int NomNumber_AsUnsignedInt(
    NomValue    value
    )
{
    AS_NUMBER(value, unsigned int, UINT_MAX);
}

long NomNumber_AsLong(
    NomValue    value
    )
{
    AS_NUMBER(value, long, LONG_MAX);
}

unsigned long NomNumber_AsUnsignedLong(
    NomValue    value
    )
{
    AS_NUMBER(value, unsigned long, ULONG_MAX);
}

long long NomNumber_AsLongLong(
    NomValue    value
    )
{
    AS_NUMBER(value, long long, LLONG_MAX);
}

unsigned long long NomNumber_AsUnsignedLongLong(
    NomValue    value
    )
{
    AS_NUMBER(value, unsigned long long, ULLONG_MAX);
}

size_t NomNumber_AsSize(
    NomValue    value
    )
{
    AS_NUMBER(value, size_t, (size_t)-1);
}

float NomNumber_AsFloat(
    NomValue    value
    )
{
    AS_NUMBER(value, float, NAN);
}

double NomNumber_AsDouble(
    NomValue    value
    )
{
    AS_NUMBER(value, double, NAN);
}