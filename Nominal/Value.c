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

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Define snprintf for compiles which do not support it
#ifdef _MSC_VER

#define snprintf c99_snprintf

int c99_vsnprintf(
    char*       str,
    size_t      size,
    const char* format,
    va_list     ap)
{
    int count = -1;

    if (size != 0)
    {
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    }
    if (count == -1)
    {
        count = _vscprintf(format, ap);
    }

    return count;
}

int c99_snprintf(
    char*       str,
    size_t      size,
    const char* format,
    ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

#endif

NomValue NomValue_Nil(
    NomState*   state
    )
{
    assert(state);

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

size_t NomValue_AsString(
    char*       buffer,
    size_t      bufferSize,
    NomValue    value
    )
{
    assert(buffer);

    size_t count = 0;
    
    switch (GET_TYPE(value))
    {
    case TYPE_INTEGER:
        count += snprintf(buffer, bufferSize, "%d", NomNumber_AsInt(value));
        break;
    case TYPE_REAL:
        count += snprintf(buffer, bufferSize, "%f", NomNumber_AsDouble(value));
        break;
    case TYPE_STRING:
    case TYPE_POOLED_STRING:
        count += snprintf(buffer, bufferSize, "\"%s\"", NomString_AsString(value));
        break;
    case TYPE_MAP:
    {
        // Opening '{'
        count += snprintf(buffer, bufferSize, "{");
        if (count >= bufferSize)
        {
            break;
        }

        // Iterate over each pair
        bool printComma = false;
        NomMapIterator iterator = { 0 };
        while (NomMap_MoveNext(value, &iterator))
        {
            if (printComma)
            {
                // Print ", "
                count += snprintf(buffer + count, bufferSize - count, ", ");
                if (count >= bufferSize)
                {
                    break;
                }
            }
            else
            {
                // Print " "
                count += snprintf(buffer + count, bufferSize - count, " ");
                if (count >= bufferSize)
                {
                    break;
                }
            }

            // Print the key
            count += NomValue_AsString(buffer + count, bufferSize - count, iterator.key);
            if (count >= bufferSize)
            {
                break;
            }

            // Print " -> "
            count += snprintf(buffer + count, bufferSize - count, " -> ");
            if (count >= bufferSize)
            {
                break;
            }

            // Print the value
            count += NomValue_AsString(buffer + count, bufferSize - count, iterator.value);
            if (count >= bufferSize)
            {
                break;
            }
            
            if (!printComma)
            {
                printComma = true;
            }
        }

        // Closing '}'
        count += snprintf(buffer + count, bufferSize - count, " }");
    } break;
    case TYPE_NIL:
        count += snprintf(buffer, bufferSize, "nil");
        break;
    default:
        count += snprintf(buffer, bufferSize, "<unknown>");
        break;
    }

    return count;
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

bool NomValue_Insert(
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (NomMap_Check(value))
    {
        result = NomMap_Insert(value, key, keyValue);
    }
    return result;
}

bool NomValue_Set(
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (NomMap_Check(value))
    {
        result = NomMap_Set(value, key, keyValue);
    }
    return result;
}

bool NomValue_InsertOrSet(
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (NomMap_Check(value))
    {
        result = NomMap_InsertOrSet(value, key, keyValue);
    }
    return result;
}

NomValue NomValue_Get(
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

bool NomValue_TryGet(
    NomValue    value,
    NomValue    key,
    NomValue*   keyValue
    )
{
    assert(keyValue);

    NomState* state = NomValue_GetState(value);

    *keyValue = NomValue_Nil(state);
    if (NomMap_Check(value))
    {
        return NomMap_TryGet(value, key, keyValue);
    }

    return false;
}

NomState* NomValue_GetState(
    NomValue    value
    )
{
    return NomState_GetInstance(GET_STATE_ID_BITS(value));
}