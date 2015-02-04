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
#include "Nominal/Map.h"
#include "Nominal/String.h"
#include "Nominal/Number.h"

#include "Value.h"
#include "Map.h"
#include "State.h"
#include "HashTable.h"
#include "Heap.h"

#include <assert.h>
#include <stdlib.h>

typedef struct
{
    HashTable*  hashTable;
    size_t      capacity;
    size_t      count;
    NomValue*   keys;
    bool        contiguous;
} MapData;

Hash HashValue(
    UserData    key,
    UserData    context
    )
{
    NomValue value;
    value.raw = key;

    return NomValue_Hash((NomState*)context, value);
}

void FreeMapData(
    void*   data
    )
{
    assert(data);

    MapData* mapData = (MapData*)data;

    // Free the hash table
    if (mapData->hashTable)
    {
        HashTable_Free(mapData->hashTable, NULL, NULL);
    }

    // Free the array of keys
    if (mapData->keys)
    {
        free(mapData->keys);
    }

    free(mapData);
}

bool CompareValue(
    UserData    left,
    UserData    right,
    UserData    context
    )
{
    NomValue leftValue;
    leftValue.raw = left;

    NomValue rightValue;
    rightValue.raw = right;

    return NomValue_Equals((NomState*)context, leftValue, rightValue);
}

void InsertKey(
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
        NomValue* newKeys = (NomValue*)malloc(sizeof(NomValue) * data->capacity);

        // Copy the keys from the old array of keys
        for (size_t i = 0; i < data->count; ++i)
        {
            newKeys[i] = data->keys[i];
        }

        // Free the old keys and use the new array
        free(data->keys);
        data->keys = newKeys;
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

bool NomMap_Check(
    NomValue value
    )
{
    return GET_TYPE(value) == TYPE_MAP;
}

NomValue NomMap_Create(
    NomState*   state
    )
{
    NomValue map = NomValue_Nil();
    SET_TYPE(map, TYPE_MAP);

    Heap* heap = NomState_GetHeap(state);
    ObjectId id = Heap_Alloc(heap, sizeof(MapData), FreeMapData);
    MapData* data = Heap_GetData(heap, id);
    data->hashTable = HashTable_Create(HashValue, CompareValue, (UserData)state, 32);
    data->capacity = 32;
    data->count = 0;
    data->keys = (NomValue*)malloc(sizeof(NomValue) * data->capacity);
    data->contiguous = true;
    
    SET_ID(map, id);
    return map;
}

bool NomMap_IsContiguous(
    NomState*   state,
    NomValue    map
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);
    
    return data->contiguous;
}

bool NomMap_MoveNext(
    NomState*       state,
    NomValue        map,
    NomIterator*    iterator
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);

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
        HashTable_Get(data->hashTable, (UserData)key.raw, (UserData*)&value.data);

        // Update the iterator
        iterator->key = key;
        iterator->value = value;

        return true;
    }

    // The end of the map has been reached
    else
    {
        iterator->source = NomValue_Nil();
        iterator->key = NomValue_Nil();
        iterator->value = NomValue_Nil();

        return false;
    }
}

bool NomMap_Insert(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);

    bool valueInserted = HashTable_Insert(data->hashTable, (UserData)key.raw, (UserData)value.raw);
    if (valueInserted)
    {
        InsertKey(data, key);
    }

    return valueInserted;
}

bool NomMap_Set(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);
    return HashTable_Set(data->hashTable, (UserData)key.raw, (UserData)value.raw);
}

bool NomMap_InsertOrSet(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);

    bool valueInserted = HashTable_InsertOrSet(data->hashTable, (UserData)key.raw, (UserData)value.raw);
    if (valueInserted)
    {
        InsertKey(data, key);
    }
    
    return valueInserted;
}

NomValue NomMap_Get(
    NomState*   state,
    NomValue    map,
    NomValue    key
    )
{
    NomValue value = { 0 };
    NomMap_TryGet(state, map, key, &value);
    return value;
}

bool NomMap_TryGet(
    NomState*   state,
    NomValue    map,
    NomValue    key,
    NomValue*   value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    ObjectId id = GET_ID(map);
    Heap* heap = NomState_GetHeap(state);
    MapData* data = Heap_GetData(heap, id);
    return HashTable_Get(data->hashTable, (UserData)key.raw, (UserData*)&value->data);
}
