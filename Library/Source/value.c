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

#include "map.h"
#include "function.h"
#include "heap.h"
#include "state.h"

#include <nominal.h>
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

NomValue nom_nil(
    void
    )
{
    NomValue value = { QNAN_VALUE };
    return value;
}

NomValue nom_true(
    void
    )
{
    NomValue value = nom_nil();
    SET_TYPE(value, TYPE_BOOLEAN);
    SET_ID(value, 1);

    return value;
}

NomValue nom_false(
    void
    )
{
    NomValue value = nom_nil();
    SET_TYPE(value, TYPE_BOOLEAN);
    SET_ID(value, 0);

    return value;
}

bool nom_istrue(
    NomState*   state,
    NomValue    value
    )
{
    assert(state);

    if (GET_TYPE(value) == TYPE_BOOLEAN)
    {
        return GET_ID(value) == 1;
    }
    else
    {
        return !nom_equals(state, value, nom_nil());
    }
}

bool nom_equals(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    if (IS_NUMBER(left))
    {
        return left.number == right.number;
    }
    else if (nom_isstring(left) && nom_isstring(right))
    {
        // If one of the strings is not interned
        if (GET_TYPE(left) != TYPE_INTERNED_STRING ||
            GET_TYPE(right) != TYPE_INTERNED_STRING)
        {
            return strcmp(nom_getstring(state, left), nom_getstring(state, right)) == 0;
        }
    }

    return left.raw == right.raw;
}

long long nom_hash(
    NomState*   state,
    NomValue    value
    )
{
    StringPool* stringpool = state_getstringpool(state);

    long long hash = value.raw;
    switch (GET_TYPE(value))
    {
    case TYPE_STRING:
        hash = hashstring((UserData)nom_getstring(state, value), (UserData)state);
        break;
    case TYPE_INTERNED_STRING:
        hash = stringpool_hash(stringpool, GET_ID(value));
        break;
    }

    return (Hash)hash;
}

size_t nom_tostring(
    NomState*   state,
    char*       buffer,
    size_t      buffersize,
    NomValue    value
    )
{
    assert(buffer);

    size_t count = 0;

    switch (GET_TYPE(value))
    {
    case TYPE_NIL:
        count += snprintf(buffer, buffersize, "nil");
        break;
    case TYPE_NUMBER:
        count += snprintf(buffer, buffersize, "%.256g", nom_todouble(value));
        break;
    case TYPE_BOOLEAN:
        count += snprintf(buffer, buffersize, "%s", nom_istrue(state, value) ? "true" : "false");
        break;
    case TYPE_STRING:
    case TYPE_INTERNED_STRING:
        count += snprintf(buffer, buffersize, "\"%s\"", nom_getstring(state, value));
        break;
    case TYPE_MAP:
    {
        bool contiguous = map_iscontiguous(state, value);

        // Opening '{'
        count += snprintf(buffer, buffersize, "{");
        if (count >= buffersize)
        {
            break;
        }

        // Iterate over each pair
        bool printcomma = false;
        NomIterator iterator = { 0 };
        while (nom_next(state, value, &iterator))
        {
            if (printcomma)
            {
                // Print ", "
                count += snprintf(buffer + count, buffersize - count, ", ");
                if (count >= buffersize)
                {
                    break;
                }
            }
            else
            {
                // Print " "
                count += snprintf(buffer + count, buffersize - count, " ");
                if (count >= buffersize)
                {
                    break;
                }
            }

            if (!contiguous)
            {
                // Print the key
                count += nom_tostring(state, buffer + count, buffersize - count, iterator.key);
                if (count >= buffersize)
                {
                    break;
                }

                // Print " -> "
                count += snprintf(buffer + count, buffersize - count, " -> ");
                if (count >= buffersize)
                {
                    break;
                }
            }

            // Print the value
            count += nom_tostring(state, buffer + count, buffersize - count, iterator.value);
            if (count >= buffersize)
            {
                break;
            }

            if (!printcomma)
            {
                printcomma = true;
            }
        }

        // Closing '}'
        count += snprintf(buffer + count, buffersize - count, " }");
    } break;
    case TYPE_FUNCTION:
    {
        HeapObjectId id = GET_ID(value);
        count += snprintf(buffer, buffersize, "<function with ID 0x%08x>", id);
        break;
    }
    default:
        count += snprintf(buffer, buffersize, "<unknown>");
        break;
    }

    return count;
}

#define ARITH(l, r, op, name)\
    if (!IS_NUMBER(l) || !IS_NUMBER(r))\
    {\
        nom_seterror(state, "Cannot %s non-numeric values", name);\
    }\
    else\
    {\
        result.number = l.number op r.number;\
    }

NomValue nom_add(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = nom_nil();
    ARITH(left, right, +, "add");
    return result;
}

NomValue nom_sub(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = nom_nil();
    ARITH(left, right, -, "subtract");
    return result;
}

NomValue nom_mul(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = nom_nil();
    ARITH(left, right, *, "multiply");
    return result;
}

NomValue nom_div(
    NomState*   state,
    NomValue    left,
    NomValue    right
    )
{
    NomValue result = nom_nil();
    ARITH(left, right, / , "divide");
    return result;
}

NomValue nom_neg(
    NomState*   state,
    NomValue    value
    )
{
    NomValue result = nom_nil();

    if (!nom_isnumber(value))
    {
        nom_seterror(state, "Cannot negate a non-numeric value");
    }
    else
    {
        result = nom_fromdouble(-nom_todouble(value));
    }

    return result;
}

bool nom_isinvokable(
    NomState*   state,
    NomValue    value
    )
{
    (void)state;
    return nom_isfunction(value);
}

NomValue nom_invoke(
    NomState*   state,
    NomValue    value,
    uint8_t     argcount,
    NomValue*   args
    )
{
    return state_invoke(state, value, argcount, args);
}

bool nom_isiterable(
    NomState*   state,
    NomValue    value
    )
{
    (void)state;
    return nom_ismap(value);
}

bool nom_next(
    NomState*       state,
    NomValue        value,
    NomIterator*    iterator
    )
{
    bool result = false;
    if (nom_ismap(value))
    {
        result = map_next(state, value, iterator);
    }
    return result;
}

bool nom_insert(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
    )
{
    bool result = false;
    if (nom_ismap(value))
    {
        result = map_insert(state, value, key, keyvalue);
    }
    return result;
}

bool nom_set(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
    )
{
    bool result = false;
    if (nom_ismap(value))
    {
        result = map_set(state, value, key, keyvalue);
    }
    return result;
}

bool nom_insertorset(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
    )
{
    bool result = false;
    if (nom_ismap(value))
    {
        result = map_insertorset(state, value, key, keyvalue);
    }
    return result;
}

NomValue nom_get(
    NomState*   state,
    NomValue    value,
    NomValue    key
    )
{
    NomValue result = nom_nil();
    if (nom_ismap(value))
    {
        map_tryget(state, value, key, &result);
    }

    return result;
}

bool nom_tryget(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue*   keyvalue
    )
{
    assert(keyvalue);

    *keyvalue = nom_nil();
    if (nom_ismap(value))
    {
        return map_tryget(state, value, key, keyvalue);
    }

    return false;
}

void nom_acquire(
    NomState*   state,
    NomValue    value
    )
{
    assert(state);

    if (IS_HEAP_OBJECT(value))
    {
        Heap* heap = state_getheap(state);

        HeapObjectId id = GET_ID(value);
        HeapObject* object = heap_getobject(heap, id);
        ++object->refcount;
    }
}

void nom_release(
    NomState*   state,
    NomValue    value
    )
{
    assert(state);

    if (IS_HEAP_OBJECT(value))
    {
        Heap* heap = state_getheap(state);

        HeapObjectId id = GET_ID(value);
        HeapObject* object = heap_getobject(heap, id);
        --object->refcount;

        if (object->refcount <= 0)
        {
            heap_dealloc(heap, id);
        }
    }
}
