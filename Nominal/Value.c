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
#include "Nominal/Map.h"

#include "Value.h"
#include "State.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

NomValue NomValue_Nil(
    NomState*   state
    )
{
    NomValue value = { 0 };
    SET_STATE_ID_BITS(value, state->id);
    return value;
}

bool NomValue_Equals(
    NomValue    value,
    NomValue    other
    )
{
    if (NomNumber_Check(value))
    {
        if (NomReal_Check(value) || NomReal_Check(other))
        {
            return NomNumber_AsDouble(value) == NomNumber_AsDouble(other);
        }
    }
    else if (NomString_Check(value) && NomString_Check(other))
    {
        if (GET_TYPE(value) != TYPE_POOLED_STRING ||
            GET_TYPE(other) != TYPE_POOLED_STRING)
        {
            return strcmp(NomString_AsString(value), NomString_AsString(other)) == 0;
        }
    }

    return value.data == other.data;
}

long long NomValue_Hash(
    NomValue    value
    )
{
    long long hash = value.data;
    switch (GET_TYPE(value))
    {
    case TYPE_STRING:
        hash = HashString((UserData)NomString_AsString(value));
        break;
    case TYPE_POOLED_STRING:
        {
            NomState* state = NomValue_GetState(value);
            hash = StringPool_Hash(state->stringPool, GET_ID_BITS(value));
        }
        break;
    }

    return (Hash)hash;
}

void NomValue_AsString(char* string, NomValue value)
{
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
        sprintf(string, "%d", NomNumber_AsInt(value));
        break;
    case TYPE_REAL:
        sprintf(string, "%f", NomNumber_AsDouble(value));
        break;
    case TYPE_STRING:
    case TYPE_POOLED_STRING:
        sprintf(string, "\"%s\"", NomString_AsString(value));
        break;
    case TYPE_NIL:
        sprintf(string, "nil");
        break;
    default:
        sprintf(string, "<unknown>");
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
        result = NomReal_FromDouble(state, NomNumber_AsDouble(l) op NomNumber_AsDouble(r));\
    }\
    else\
    {\
        result = NomInteger_FromInt(state, NomNumber_AsInt(l) op NomNumber_AsInt(r)); \
    }

NomValue NomValue_Add(
    NomValue    value,
    NomValue    other
    )
{
    NomState* state = NomValue_GetState(value);
    NomValue result = NomValue_Nil(state);
    ARITH(value, other, +, "add");
    return result;
}

NomValue NomValue_Subtract(
    NomValue    value,
    NomValue    other
    )
{
    NomState* state = NomValue_GetState(value);
    NomValue result = NomValue_Nil(state);
    ARITH(value, other, -, "subtract");
    return result;
}

NomValue NomValue_Multiply(
    NomValue    value,
    NomValue    other
    )
{
    NomState* state = NomValue_GetState(value);
    NomValue result = NomValue_Nil(state);
    ARITH(value, other, *, "multiply");
    return result;
}

NomValue NomValue_Divide(
    NomValue    value,
    NomValue    other
    )
{
    NomState* state = NomValue_GetState(value);
    NomValue result = NomValue_Nil(state);
    ARITH(value, other, / , "divide");
    return result;
}

NomValue NomValue_Negate(
    NomValue    value
    )
{
    NomState* state = NomValue_GetState(value);

    NomValue result = NomValue_Nil(state);

    if (!NomNumber_Check(value))
    {
        NomState_SetError(state, "Cannot negate a non-numeric value");
    }
    else if (NomReal_Check(value))
    {
        result = NomReal_FromDouble(state, -NomNumber_AsDouble(value));
    }
    else
    {
        result = NomInteger_FromInt(state, -NomNumber_AsInt(value));
    }

    return result;
}

NomValue NomValue_Index(
    NomValue    value,
    NomValue    key
    )
{
    NomState* state = NomValue_GetState(value);

    NomValue result = NomValue_Nil(state);
    if (NomMap_Check(value))
    {
        NomMap_TryGet(value, key, &result);
    }

    return result;
}

NomState* NomValue_GetState(
    NomValue    value
    )
{
    return NomState_GetInstance(GET_STATE_ID_BITS(value));
}