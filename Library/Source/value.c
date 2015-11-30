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
    va_list     ap
)
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
    ...
)
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
    SET_TYPE(value, VALUETYPE_BOOLEAN);
    SET_ID(value, 1);

    return value;
}

NomValue nom_false(
    void
)
{
    NomValue value = nom_nil();
    SET_TYPE(value, VALUETYPE_BOOLEAN);
    SET_ID(value, 0);

    return value;
}

bool nom_istrue(
    NomState*   state,
    NomValue    value
)
{
    (void)state;
    assert(state);

    if (GET_TYPE(value) == VALUETYPE_BOOLEAN)
    {
        return GET_ID(value) == 1;
    }
    else
    {
        return !nom_isnil(value);
    }
}

bool nom_isnil(
    NomValue    value
)
{
    bool result = value.raw == nom_nil().raw;
    return result;
}

bool nom_isobject(
    NomValue    value
)
{
    return GET_TYPE(value) == VALUETYPE_OBJECT;
}

bool nom_isclass(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    bool result = map_getclass(state, value).raw == state->classes.class.raw;
    return result;
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
    else if (nom_isstring(state, left) && nom_isstring(state, right))
    {
        // If one of the strings is not interned
        if (GET_TYPE(left) != VALUETYPE_INTERNED_STRING ||
                GET_TYPE(right) != VALUETYPE_INTERNED_STRING)
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
    long long hash = value.raw;

    ValueType type = GET_TYPE(value);
    switch (type)
    {
    case VALUETYPE_NIL:
    case VALUETYPE_NUMBER:
    case VALUETYPE_BOOLEAN:
        break;
    case VALUETYPE_INTERNED_STRING:
        hash = stringpool_hash(state->stringpool, GET_ID(value));
        break;
    case VALUETYPE_OBJECT:
    {
        HeapObject* object = heap_getobject(state->heap, value);
        if (object && object->type == OBJECTTYPE_STRING)
        {
            hash = hashstring((UserData)nom_getstring(state, value), (UserData)state);
        }
    }
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

    ValueType type = GET_TYPE(value);
    switch (type)
    {
    case VALUETYPE_NIL:
        count += snprintf(buffer, buffersize, "nil");
        break;
    case VALUETYPE_NUMBER:
        count += snprintf(buffer, buffersize, "%.256g", nom_todouble(value));
        break;
    case VALUETYPE_BOOLEAN:
        count += snprintf(buffer, buffersize, "%s", nom_istrue(state, value) ? "true" : "false");
        break;
    case VALUETYPE_INTERNED_STRING:
        count += snprintf(buffer, buffersize, "\"%s\"", nom_getstring(state, value));
        break;
    case VALUETYPE_OBJECT:
    {
        HeapObject* object = heap_getobject(state->heap, value);
        if (object)
        {
            switch (object->type)
            {
            case OBJECTTYPE_STRING:
                count += snprintf(buffer, buffersize, "\"%s\"", nom_getstring(state, value));
                break;
            case OBJECTTYPE_MAP:
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
            }
            break;
            case OBJECTTYPE_FUNCTION:
                count += snprintf(buffer, buffersize, "<function with ID 0x%08x>", GET_ID(value));
                break;
            default:
                count += snprintf(buffer, buffersize, "<unknown>");
                break;
            }
        }
    }
    break;
    default:
        count += snprintf(buffer, buffersize, "<unknown>");
        break;
    }

    return count;
}

NomValue nom_add(
    NomState*   state,
    NomValue    left,
    NomValue    right
)
{
    NomValue result = nom_nil();

    if (!IS_NUMBER(left) || !IS_NUMBER(right))
    {
        NomValue class = map_getclass(state, left);
        NomValue function;
        if (nom_get(state, class, state->strings.add, &function) &&
                nom_isfunction(state, function))
        {
            NomValue args[2] = { { left.raw }, { right.raw } };
            result = nom_call(state, function, 2, args);
        }
        else
        {
            nom_seterror(state, "Cannot add non-numeric values");
        }
    }
    else
    {
        result.number = left.number + right.number;
    }

    return result;
}

NomValue nom_sub(
    NomState*   state,
    NomValue    left,
    NomValue    right
)
{
    NomValue result = nom_nil();

    if (!IS_NUMBER(left) || !IS_NUMBER(right))
    {
        NomValue class = map_getclass(state, left);
        NomValue function;
        if (nom_get(state, class, state->strings.subtract, &function) &&
                nom_isfunction(state, function))
        {
            NomValue args[2] = { { left.raw }, { right.raw } };
            result = nom_call(state, function, 2, args);
        }
        else
        {
            nom_seterror(state, "Cannot subtract non-numeric values");
        }
    }
    else
    {
        result.number = left.number - right.number;
    }

    return result;
}

NomValue nom_mul(
    NomState*   state,
    NomValue    left,
    NomValue    right
)
{
    NomValue result = nom_nil();

    if (!IS_NUMBER(left) || !IS_NUMBER(right))
    {
        NomValue class = map_getclass(state, left);
        NomValue function;
        if (nom_get(state, class, state->strings.multiply, &function) &&
                nom_isfunction(state, function))
        {
            NomValue args[2] = { { left.raw }, { right.raw } };
            result = nom_call(state, function, 2, args);
        }
        else
        {
            nom_seterror(state, "Cannot multiply non-numeric values");
        }
    }
    else
    {
        result.number = left.number * right.number;
    }

    return result;
}

NomValue nom_div(
    NomState*   state,
    NomValue    left,
    NomValue    right
)
{
    NomValue result = nom_nil();

    if (!IS_NUMBER(left) || !IS_NUMBER(right))
    {
        NomValue class = map_getclass(state, left);
        NomValue function;
        if (nom_get(state, class, state->strings.divide, &function) &&
                nom_isfunction(state, function))
        {
            NomValue args[2] = { { left.raw }, { right.raw } };
            result = nom_call(state, function, 2, args);
        }
        else
        {
            nom_seterror(state, "Cannot divide non-numeric values");
        }
    }
    else
    {
        result.number = left.number / right.number;
    }

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

bool nom_iscallable(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    value = function_resolve(state, value);
    bool result = nom_isfunction(state, value);

    return result;
}

NomValue nom_call(
    NomState*   state,
    NomValue    value,
    uint8_t     argcount,
    NomValue*   args
)
{
    assert(state);

    NomValue result = nom_nil();

    value = function_resolve(state, value);
    if (nom_isfunction(state, value))
    {
        result = state_call(state, value, argcount, args);
    }

    return result;
}

bool nom_isiterable(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    return nom_ismap(state, value);
}

bool nom_next(
    NomState*       state,
    NomValue        value,
    NomIterator*    iterator
)
{
    bool result = false;
    if (nom_ismap(state, value))
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
    if (nom_ismap(state, value))
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
    if (nom_ismap(state, value))
    {
        result = map_set(state, value, key, keyvalue);
    }
    return result;
}

bool nom_update(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue    keyvalue
)
{
    bool result = false;
    if (nom_ismap(state, value))
    {
        result = map_update(state, value, key, keyvalue);
    }
    return result;
}

NomValue nom_find(
    NomState*   state,
    NomValue    value,
    NomValue    key
)
{
    NomValue result = nom_nil();
    if (nom_ismap(state, value))
    {
        map_get(state, value, key, &result);
    }

    return result;
}

bool nom_get(
    NomState*   state,
    NomValue    value,
    NomValue    key,
    NomValue*   keyvalue
)
{
    assert(keyvalue);

    bool result = false;

    *keyvalue = nom_nil();
    if (nom_ismap(state, value))
    {
        result = map_get(state, value, key, keyvalue);
    }

    return result;
}

void nom_acquire(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    HeapObject* object = heap_getobject(state->heap, value);
    if (object)
    {
        ++object->refcount;
    }
}

void nom_release(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    HeapObject* object = heap_getobject(state->heap, value);
    if (object)
    {
        --object->refcount;
    }
}

void value_visit(
    NomState*       state,
    NomValue        value,
    ValueVisitor    visitor
)
{
    assert(state);
    assert(visitor);

    visitor(state, value);

    // If the value is a map then visit all keys/values
    if (nom_ismap(state, value))
    {
        NomIterator iterator = { 0 };
        while (nom_next(state, value, &iterator))
        {
            value_visit(state, iterator.key, visitor);
            value_visit(state, iterator.value, visitor);
        }
    }
}
