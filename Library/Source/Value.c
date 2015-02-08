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
#include "Nominal/String.h"
#include "Nominal/Number.h"
#include "Nominal/Map.h"

#include "Value.h"
#include "Map.h"
#include "Function.h"
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

NomValue Nom_Nil(
    void
    )
{
    NomValue value = { QNAN_VALUE };
    return value;
}

NomValue Nom_True(
    void
    )
{
    NomValue value = { QNAN_VALUE };
    return value;
}

NomValue Nom_False(
    void
    )
{
    NomValue value = { QNAN_VALUE };
    return value;
}

bool Nom_Equals(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    if (IS_NUMBER(value))
    {
        return value.number == other.number;
    }
    else if (Nom_IsString(value) && Nom_IsString(other))
    {
        // If one of the strings is not interned
        if (GET_TYPE(value) != TYPE_INTERNED_STRING ||
            GET_TYPE(other) != TYPE_INTERNED_STRING)
        {
            return strcmp(Nom_AsRawString(state, value), Nom_AsRawString(state, other)) == 0;
        }
    }

    return value.raw == other.raw;
}

long long Nom_Hash(
    NomState*   state,
    NomValue    value
    )
{
    StringPool* stringPool = State_GetStringPool(state);

    long long hash = value.raw;
    switch (GET_TYPE(value))
    {
    case TYPE_STRING:
        hash = HashString((UserData)Nom_AsRawString(state, value), (UserData)state);
        break;
    case TYPE_INTERNED_STRING:
        hash = StringPool_Hash(stringPool, GET_ID(value));
        break;
    }

    return (Hash)hash;
}

size_t Nom_AsString(
    NomState*   state,
    char*       buffer,
    size_t      bufferSize,
    NomValue    value
    )
{
    assert(buffer);

    size_t count = 0;

    switch (GET_TYPE(value))
    {
    case TYPE_NIL:
        count += snprintf(buffer, bufferSize, "nil");
        break;
    case TYPE_NUMBER:
        count += snprintf(buffer, bufferSize, "%.256g", Nom_AsDouble(value));
        break;
    case TYPE_BOOLEAN:
        break;
    case TYPE_STRING:
    case TYPE_INTERNED_STRING:
        count += snprintf(buffer, bufferSize, "\"%s\"", Nom_AsRawString(state, value));
        break;
    case TYPE_MAP:
    {
        bool contiguous = Map_IsContiguous(state, value);

        // Opening '{'
        count += snprintf(buffer, bufferSize, "{");
        if (count >= bufferSize)
        {
            break;
        }

        // Iterate over each pair
        bool printComma = false;
        NomIterator iterator = { 0 };
        while (Nom_MoveNext(state, value, &iterator))
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

            if (!contiguous)
            {
                // Print the key
                count += Nom_AsString(state, buffer + count, bufferSize - count, iterator.key);
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
            }

            // Print the value
            count += Nom_AsString(state, buffer + count, bufferSize - count, iterator.value);
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
    case TYPE_FUNCTION:
    {
        ObjectId id = GET_ID(value);
        count += snprintf(buffer, bufferSize, "<function with ID 0x%08x>", id);
        break;
    }
    default:
        count += snprintf(buffer, bufferSize, "<unknown>");
        break;
    }

    return count;
}

#define ARITH(l, r, op, name)\
    if (!IS_NUMBER(l) || !IS_NUMBER(r))\
    {\
        State_SetError(state, "Cannot %s non-numeric values", name);\
    }\
    else\
    {\
        result.number = l.number op r.number;\
    }

NomValue Nom_Add(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    NomValue result = Nom_Nil();
    ARITH(value, other, +, "add");
    return result;
}

NomValue Nom_Subtract(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    NomValue result = Nom_Nil();
    ARITH(value, other, -, "subtract");
    return result;
}

NomValue Nom_Multiply(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    NomValue result = Nom_Nil();
    ARITH(value, other, *, "multiply");
    return result;
}

NomValue Nom_Divide(
    NomState*   state,
    NomValue    value,
    NomValue    other
    )
{
    NomValue result = Nom_Nil();
    ARITH(value, other, / , "divide");
    return result;
}

NomValue Nom_Negate(
    NomState*   state,
    NomValue    value
    )
{
    NomValue result = Nom_Nil();

    if (!Nom_IsNumber(value))
    {
        State_SetError(state, "Cannot negate a non-numeric value");
    }
    else
    {
        result = Nom_FromDouble(-Nom_AsDouble(value));
    }

    return result;
}

bool Nom_IsIterable(
    NomState*   state,
    NomValue    value
    )
{
    (void)state;
    return Nom_IsMap(value);
}

bool Nom_MoveNext(
    NomState*       state,
    NomValue        value,
    NomIterator*    iterator
    )
{
    bool result = false;
    if (Nom_IsMap(value))
    {
        result = Map_MoveNext(state, value, iterator);
    }
    return result;
}

bool Nom_Insert(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (Nom_IsMap(value))
    {
        result = Map_Insert(state, value, key, keyValue);
    }
    return result;
}

bool Nom_Set(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (Nom_IsMap(value))
    {
        result = Map_Set(state, value, key, keyValue);
    }
    return result;
}

bool Nom_InsertOrSet(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyValue
    )
{
    bool result = false;
    if (Nom_IsMap(value))
    {
        result = Map_InsertOrSet(state, value, key, keyValue);
    }
    return result;
}

NomValue Nom_Get(
    NomState*   state,
    NomValue    value,
    NomValue    key
    )
{
    NomValue result = Nom_Nil();
    if (Nom_IsMap(value))
    {
        Map_TryGet(state, value, key, &result);
    }

    return result;
}

bool Nom_TryGet(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue*   keyValue
    )
{
    assert(keyValue);

    *keyValue = Nom_Nil();
    if (Nom_IsMap(value))
    {
        return Map_TryGet(state, value, key, keyValue);
    }

    return false;
}
