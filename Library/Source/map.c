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
#include "hashtable.h"
#include "heap.h"

#include <assert.h>
#include <stdlib.h>

typedef struct
{
    HashTable*  hashtable;
    size_t      capacity;
    size_t      count;
    NomValue*   keys;
    bool        contiguous;
} MapData;

Hash hashvalue(
    UserData    key,
    UserData    context
)
{
    NomValue value = { key };

    return nom_hash((NomState*)context, value);
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
    NomValue leftvalue = { left };
    NomValue rightvalue = { right };

    return nom_equals((NomState*)context, leftvalue, rightvalue);
}

void insertkey(
    MapData*    data,
    NomValue    key
)
{
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

bool nom_ismap(
    NomState*   state,
    NomValue    value
)
{
    assert(state);

    bool result = false;

    if (GET_TYPE(value) == VALUETYPE_OBJECT)
    {
        Heap* heap = state_getheap(state);
        HeapObjectId id = GET_ID(value);
        HeapObject* object = heap_getobject(heap, id);
        result = object && object->type == OBJECTTYPE_MAP;
    }

    return result;
}

NomValue nom_newmap(
    NomState*   state
)
{
    assert(state);

    NomValue map = nom_nil();
    SET_TYPE(map, VALUETYPE_OBJECT);

    Heap* heap = state_getheap(state);
    HeapObjectId id = heap_alloc(heap, OBJECTTYPE_MAP, sizeof(MapData), freemapdata);
    MapData* data = heap_getdata(heap, id);
    data->hashtable = hashtable_new(hashvalue, comparevalue, (UserData)state, 32);
    data->capacity = 32;
    data->count = 0;
    data->keys = (NomValue*)malloc(sizeof(NomValue) * data->capacity);
    data->contiguous = true;

    SET_ID(map, id);
    return map;
}

bool map_iscontiguous(
    NomState*   state,
    NomValue    map
)
{
    assert(state);

    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);

    return data->contiguous;
}

bool map_next(
    NomState*       state,
    NomValue        map,
    NomIterator*    iterator
)
{
    assert(state);

    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);

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
        hashtable_get(data->hashtable, (UserData)key.raw, (UserData*)&value.data);

        // Update the iterator
        iterator->key = key;
        iterator->value = value;

        return true;
    }

    // The end of the map has been reached
    else
    {
        iterator->source = nom_nil();
        iterator->key = nom_nil();
        iterator->value = nom_nil();

        return false;
    }
}

bool map_insert(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    assert(state);

    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);

    bool inserted = hashtable_insert(data->hashtable, (UserData)key.raw, (UserData)value.raw);
    if (inserted)
    {
        insertkey(data, key);
    }

    return inserted;
}

bool map_set(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    assert(state);

    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);

    bool result = hashtable_set(data->hashtable, (UserData)key.raw, (UserData)value.raw);
    return result;
}

bool map_insertorset(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
)
{
    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);

    bool inserted = hashtable_insertorset(data->hashtable, (UserData)key.raw, (UserData)value.raw);
    if (inserted)
    {
        insertkey(data, key);
    }

    return inserted;
}

NomValue map_get(
    NomState*   state,
    NomValue    map,
    NomValue    key
)
{
    NomValue value = { 0 };
    map_tryget(state, map, key, &value);
    return value;
}

bool map_tryget(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue*   value
)
{
    if (!nom_ismap(state, map))
    {
        return false;
    }

    HeapObjectId id = GET_ID(map);
    Heap* heap = state_getheap(state);
    MapData* data = heap_getdata(heap, id);
    return hashtable_get(data->hashtable, (UserData)key.raw, (UserData*)&value->data);
}
