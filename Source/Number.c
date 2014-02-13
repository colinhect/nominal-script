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
#include "Nominal/Value.h"
#include "Nominal/Integer.h"
#include "Nominal/Real.h"

#include "Value.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

bool NomNumber_Check(NomValue value)
{
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
    case TYPE_REAL:
        return true;
    default:
        return false;
    }
}

int NomNumber_AsInt(NomValue value)
{
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
        return GET_INTEGER_BITS(value);
    case TYPE_REAL:
        {
            uint64_t v = GET_REAL_BITS(value);
            return (int)*((float*)&v);
        }
    default:
        return -1;
    }
}

float NomNumber_AsFloat(NomValue value)
{
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
        return (float)GET_INTEGER_BITS(value);
    case TYPE_REAL:
        {
            uint64_t v = GET_REAL_BITS(value);
            return *((float*)&v);
        }
    default:
        return NAN;
    }
}

double NomNumber_AsDouble(NomValue value)
{
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
        return (double)GET_INTEGER_BITS(value);
    case TYPE_REAL:
        {
            uint64_t v = GET_REAL_BITS(value);
            return (double)*((float*)&v);
        }
    default:
        return NAN;
    }
}