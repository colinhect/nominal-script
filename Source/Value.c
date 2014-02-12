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
#include "Nominal/Number.h"
#include "Nominal/Integer.h"
#include "Nominal/Real.h"

#include "Type.h"
#include "Value.h"
#include "State.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

NomValue NomValue_Nil(
    void
    )
{
    NomValue value;
    value.data = 0;
    return value;
}

bool NomValue_Equals(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    return NomValue_Compare(state, value, other) == 0;
}

int NomValue_Compare(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    if (NomNumber_Check(value))
    {
        if (NomInteger_Check(value) || NomInteger_Check(other))
        {
            int l = NomNumber_AsInt(value);
            int r = NomNumber_AsInt(other);
            return l == r ? 0 : (l < r ? -1 : 1);
        }
        else if (NomReal_Check(value) || NomReal_Check(other))
        {
            double l = NomNumber_AsDouble(value);
            double r = NomNumber_AsDouble(other);
            return l == r ? 0 : (l < r ? -1 : 1);
        }
        else
        {
            return -1;
        }
    }
    else if (NomString_Check(value))
    {
        if (NomString_Check(other))
        {
            if (GET_TYPE_BITS(value) == TYPE_POOLED_STRING &&
                GET_TYPE_BITS(other) == TYPE_POOLED_STRING)
            {
                unsigned l = GET_ID_BITS(value);
                unsigned r = GET_ID_BITS(other);
                return l == r ? 0 : (l < r ? -1 : 1);
            }
            const char* l = NomString_AsString(state, value);
            const char* r = NomString_AsString(state, other);
            return strcmp(l, r);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        uint64_t l = value.data;
        uint64_t r = other.data;
        return l == r ? 0 : (l < r ? -1 : 1);
    }
}

void NomValue_AsString(NomState* s, char* dest, NomValue value)
{
    switch ((Type)GET_TYPE_BITS(value))
    {
    case TYPE_INTEGER:
        sprintf(dest, "%d", NomNumber_AsInt(value));
        break;
    case TYPE_REAL:
        sprintf(dest, "%f", NomNumber_AsDouble(value));
        break;
    case TYPE_STRING:
    case TYPE_POOLED_STRING:
        sprintf(dest, "\"%s\"", NomString_AsString(s, value));
        break;
    case TYPE_NIL:
        sprintf(dest, "nil");
        break;
    default:
        sprintf(dest, "<unknown>");
        break;
    }
}

#define ARITH(l, r, op, name)\
    if (!NomNumber_Check(l) || !NomNumber_Check(r))\
    {\
        NomState_SetError(state, "Cannot %s non-numeric values", name);\
    }\
    else if (NomReal_Check(l) || NomReal_Check(r))\
    {\
        result = NomReal_FromDouble(NomNumber_AsDouble(l) op NomNumber_AsDouble(r));\
    }\
    else\
    {\
        result = NomInteger_FromInt(NomNumber_AsInt(l) op NomNumber_AsInt(r));\
    }

NomValue NomValue_Add(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = NomValue_Nil();
    ARITH(left, right, +, "add");
    return result;
}

NomValue NomValue_Subtract(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = NomValue_Nil();
    ARITH(left, right, -, "subtract");
    return result;
}

NomValue NomValue_Multiply(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = NomValue_Nil();
    ARITH(left, right, *, "multiply");
    return result;
}

NomValue NomValue_Divide(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = NomValue_Nil();
    ARITH(left, right, /, "divide");
    return result;
}

NomValue NomValue_Negate(
    NomState*   state,
    NomValue    value
    )
{
    NomValue result = NomValue_Nil();

    if (!NomNumber_Check(value))
    {
        NomState_SetError(state, "Cannot negate a non-numeric value");
    }
    else if (NomReal_Check(value))
    {
        result = NomReal_FromDouble(-NomNumber_AsDouble(value));
    }
    else
    {
        result = NomInteger_FromInt(-NomNumber_AsInt(value));
    }

    return result;
}