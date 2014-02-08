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
#include "Nominal/String.h"

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable : 4056)
#define INFINITY (FLT_MAX+FLT_MAX)
#define NAN (INFINITY-INFINITY)
#endif

NomValue NOM_NIL = { NOM_TYPE_NIL, 0 };

int NomValue_IsNumber(NomValue value)
{
    switch (value.fields.type)
    {
    case NOM_TYPE_INTEGER:
    case NOM_TYPE_REAL:
        return 1;
    default:
        return 0;
    }
}

int NomValue_AsInt(NomValue value)
{
    switch (value.fields.type)
    {
    case NOM_TYPE_INTEGER:
        return value.fields.data.integerValue;
    case NOM_TYPE_REAL:
        return (int)value.fields.data.realValue;
    default:
        return -1;
    }
}

float NomValue_AsFloat(NomValue value)
{
    switch (value.fields.type)
    {
    case NOM_TYPE_INTEGER:
        return (float)value.fields.data.integerValue;
    case NOM_TYPE_REAL:
        return value.fields.data.realValue;
    default:
        return NAN;
    }
}

double NomValue_AsDouble(NomValue value)
{
    switch (value.fields.type)
    {
    case NOM_TYPE_INTEGER:
        return (double)value.fields.data.integerValue;
    case NOM_TYPE_REAL:
        return (double)value.fields.data.realValue;
    default:
        return NAN;
    }
}

int NomValue_Equals(
    NomValue    value,
    NomValue    other
    )
{
    // Incomplete implementation
    return
        value.fields.type == other.fields.type &&
        value.fields.data.integerValue == other.fields.data.integerValue;
}

void NomValue_AsString(NomState* s, char* dest, NomValue value)
{
    switch (value.fields.type)
    {
    case NOM_TYPE_INTEGER:
        sprintf(dest, "%d", NomValue_AsInt(value));
        break;
    case NOM_TYPE_REAL:
        sprintf(dest, "%f", NomValue_AsDouble(value));
        break;
    case NOM_TYPE_STRING:
    case NOM_TYPE_STATIC_STRING:
        sprintf(dest, "\"%s\"", NomString_AsString(s, value));
        break;
    case NOM_TYPE_NIL:
        sprintf(dest, "nil");
        break;
    default:
        sprintf(dest, "<unknown>");
        break;
    }
}