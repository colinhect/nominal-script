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
#include "map.h"

#include "value.h"
#include "state.h"
#include "heap.h"

#include <assert.h>
#include <stdlib.h>

Hash hashvalue(
    UserData    key,
    UserData    context
);

void freemapdata(
    void*   data
);

bool comparevalue(
    UserData    left,
    UserData    right,
    UserData    context
);

void insertkey(
    MapData*    data,
    NomValue    key
);

bool nom_ismap(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, value);
    if (object)
    {
        result = object->type == OBJECTTYPE_MAP;
    }

    return result;
}

NomValue nom_newmap(
    NomState*   state
)
{
    assert(state);

    NomValue map = heap_alloc(state->heap, OBJECTTYPE_MAP, sizeof(MapData), freemapdata);

    MapData* data = heap_getdata(state->heap, map);
    data->hashtable = hashtable_new(hashvalue, comparevalue, (UserData)state, 32);
    data->capacity = 32;
    data->count = 0;
    data->keys = (NomValue*)malloc(sizeof(NomValue) * data->capacity);
    data->contiguous = true;
    data->class = nom_nil();

    return map;
}

bool map_iscontiguous(
    NomState*   state,
    NomValue    map
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        if (data)
        {
            result = data->contiguous;
        }
    }

    return result;
}

bool map_next(
    NomState*       state,
    NomValue        map,
    NomIterator*    iterator
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        if (data)
        {
            // If this is a new iterator
            if (iterator->source.raw == 0)
            {
                iterator->source = map;
                iterator->data.map.index = 0;
            }
            else
            {
                iterator->data.map.index += 1;
            }

            size_t index = iterator->data.map.index;

            // If the index is within the range of the keys in the map
            if (index < data->count)
            {
                // Get the key at this index
                NomValue key = data->keys[index];

                // Get the value with the key
                NomValue value;
                hashtable_find(data->hashtable, (UserData)key.raw, (UserData*)&value.data);

                // Update the iterator
                iterator->key = key;
                iterator->value = value;

                result = true;
            }

            // The end of the map has been reached
            else
            {
                iterator->source = nom_nil();
                iterator->key = nom_nil();
                iterator->value = nom_nil();

                result = false;
            }
        }
    }

    return result;
}

bool map_insert(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        if (data)
        {
            result = hashtable_insert(data->hashtable, (UserData)key.raw, (UserData)value.raw);
            if (result)
            {
                insertkey(data, key);
            }
        }
    }

    return result;
}

bool map_set(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        if (data)
        {
            result = hashtable_set(data->hashtable, (UserData)key.raw, (UserData)value.raw);
        }
    }

    return result;
}

bool map_update(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        if (data)
        {
            result = hashtable_update(data->hashtable, (UserData)key.raw, (UserData)value.raw);
            if (result)
            {
                insertkey(data, key);
            }
        }
    }

    return result;
}

NomValue map_find(
    NomState*   state,
    NomValue    map,
    NomValue    key
)
{
    NomValue value = { 0 };
    map_get(state, map, key, &value);
    return value;
}

bool map_get(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue*   value
)
{
    assert(state);

    bool result = false;

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        result = hashtable_find(data->hashtable, (UserData)key.raw, (UserData*)&value->data);
    }

    return result;
}

void map_setclass(
    NomState*   state,
    NomValue    map,
    NomValue    class
)
{
    assert(state);

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        data->class = class;
    }
}

NomValue map_getclass(
    NomState*   state,
    NomValue    map
)
{
    assert(state);

    NomValue class = nom_nil();

    HeapObject* object = heap_getobject(state->heap, map);
    if (object && object->type == OBJECTTYPE_MAP && object->data)
    {
        MapData* data = (MapData*)object->data;
        class = data->class;
    }

    return class;
}

Hash hashvalue(
    UserData    key,
    UserData    context
)
{
    NomState* state = (NomState*)context;
    NomValue value = { key };
    return nom_hash(state, value);
}

void freemapdata(
    void*   data
)
{
    assert(data);

    MapData* mapdata = (MapData*)data;

    // Free the hash table
    if (mapdata->hashtable)
    {
        hashtable_free(mapdata->hashtable, NULL, NULL);
    }

    // Free the array of keys
    if (mapdata->keys)
    {
        free(mapdata->keys);
    }

    free(mapdata);
}

bool comparevalue(
    UserData    left,
    UserData    right,
    UserData    context
)
{
    NomState* state = (NomState*)context;
    NomValue leftvalue = { left };
    NomValue rightvalue = { right };

    return nom_equals(state, leftvalue, rightvalue);
}

void insertkey(
    MapData*    data,
    NomValue    key
)
{
    assert(data);

    // If there is not enough capacity
    if (data->count >= data->capacity)
    {
        // Double the capacity
        data->capacity *= 2;

        // Allocate a new array of keys
        NomValue* newkeys = (NomValue*)malloc(sizeof(NomValue) * data->capacity);

        // Copy the keys from the old array of keys
        for (size_t i = 0; i < data->count; ++i)
        {
            newkeys[i] = data->keys[i];
        }

        // Free the old keys and use the new array
        free(data->keys);
        data->keys = newkeys;
    }

    // Insert the key at the end
    data->keys[data->count] = key;
    data->count += 1;

    if (data->contiguous)
    {
        // If the key matches the index then the map remains contiguous
        data->contiguous = IS_NUMBER(key) && (data->count - 1) == (size_t)key.number;
    }
}
