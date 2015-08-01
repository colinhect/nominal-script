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
#include "string.h"

#include "state.h"
#include "heap.h"
#include "value.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool nom_isstring(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    ValueType type = GET_TYPE(value);
    if (type == VALUETYPE_INTERNED_STRING)
    {
        result = true;
    }
    else
    {
        Heap* heap = state_getheap(state);
        HeapObject* object = heap_getobject(heap, value);
        if (object)
        {
            result = object->type == OBJECTTYPE_STRING;
        }
    }

    return result;
}

NomValue nom_newstring(
    NomState*   state,
    const char* value
)
{
    assert(state);

    Heap* heap = state_getheap(state);
    NomValue string = heap_alloc(heap, OBJECTTYPE_STRING, strlen(value) + 1, free);

    // Copy the string to the object's data
    char* data = heap_getdata(heap, string);
    strcpy(data, value);

    return string;
}

NomValue nom_newinternedstring(
    NomState*   state,
    const char* value
)
{
    assert(state);
    assert(value);

    StringPool* stringpool = state_getstringpool(state);
    StringId id = stringpool_getid(stringpool, value);

    NomValue string = nom_nil();
    SET_TYPE(string, VALUETYPE_INTERNED_STRING);
    SET_ID(string, id);

    return string;
}

const char* nom_getstring(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    StringPool* stringpool = state_getstringpool(state);

    const char* string = NULL;
    ValueType type = GET_TYPE(value);
    if (type == VALUETYPE_INTERNED_STRING)
    {
        string = stringpool_find(stringpool, GET_ID(value));
    }
    else
    {
        Heap* heap = state_getheap(state);
        HeapObject* object = heap_getobject(heap, value);
        if (object && object->type == OBJECTTYPE_STRING)
        {
            string = heap_getdata(heap, value);
        }
    }

    return string;
}

NomValue string_newinterned(
    StringId    id
)
{
    NomValue string = nom_nil();
    SET_TYPE(string, VALUETYPE_INTERNED_STRING);
    SET_ID(string, id);
    return string;
}
