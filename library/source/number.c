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
#include "value.h"

#include <math.h>

bool nom_isnumber(
    NomValue    value
)
{
    return IS_NUMBER(value);
}

NomValue nom_fromint(
    int             value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromunsignedint(
    unsigned int    value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromlong(
    long            value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromunsignedlong(
    unsigned long   value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromlonglong(
    long long           value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromunsignedlonglong(
    unsigned long long  value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromsize(
    size_t  value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromfloat(
    float       value
)
{
    NomValue result;
    result.number = (double)value;
    return result;
}

NomValue nom_fromdouble(
    double      value
)
{
    NomValue result;
    result.number = value;
    return result;
}

int nom_toint(
    NomValue    value
)
{
    int result = INT_MAX;
    if (IS_NUMBER(value))
    {
        result = (int)value.number;
    }
    return result;
}

unsigned int nom_tounsignedint(
    NomValue    value
)
{
    unsigned int result = UINT_MAX;
    if (IS_NUMBER(value))
    {
        result = (unsigned int)value.number;
    }
    return result;
}

long nom_tolong(
    NomValue    value
)
{
    long result = LONG_MAX;
    if (IS_NUMBER(value))
    {
        result = (long)value.number;
    }
    return result;
}

unsigned long nom_tounsignedlong(
    NomValue    value
)
{
    unsigned long result = ULONG_MAX;
    if (IS_NUMBER(value))
    {
        result = (unsigned long)value.number;
    }
    return result;
}

long long nom_tolonglong(
    NomValue    value
)
{
    long long result = LLONG_MAX;
    if (IS_NUMBER(value))
    {
        result = (long long)value.number;
    }
    return result;
}

unsigned long long nom_tounsignedlonglong(
    NomValue    value
)
{
    unsigned long long result = ULLONG_MAX;
    if (IS_NUMBER(value))
    {
        result = (unsigned long long)value.number;
    }
    return result;
}

size_t nom_tosize(
    NomValue    value
)
{
    size_t result = (size_t)-1;
    if (IS_NUMBER(value))
    {
        result = (size_t)value.number;
    }
    return result;
}

float nom_tofloat(
    NomValue    value
)
{
    float result = NAN;
    if (IS_NUMBER(value))
    {
        result = (float)value.number;
    }
    return result;
}

double nom_todouble(
    NomValue    value
)
{
    double result = NAN;
    if (IS_NUMBER(value))
    {
        result = value.number;
    }
    return result;
}
