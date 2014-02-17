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
#include "Nominal/Map.h"
#include "Nominal/String.h"
#include "Nominal/Real.h"

#include "Value.h"
#include "State.h"
#include "HashTable.h"
#include "Heap.h"

#include <stdlib.h>

typedef struct _MapData
{
    NomState*   state;
    HashTable*  hashTable;
} MapData;

Hash HashValue(
    UserData    key
    )
{
    NomValue value;
    value.data = key;

    return NomValue_Hash(value);
}

bool CompareValue(
    UserData    left,
    UserData    right
    )
{
    NomValue leftValue;
    leftValue.data = left;

    NomValue rightValue;
    rightValue.data = right;

    return NomValue_Equals(leftValue, rightValue);
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
    NomValue map;
    INIT_VALUE(map, TYPE_MAP, state);

    ObjectId id = Heap_Alloc(state->heap, sizeof(MapData), free);
    MapData* data = Heap_GetData(state->heap, id);
    data->state = state;
    data->hashTable = HashTable_Create(HashValue, CompareValue, 32);

    SET_ID_BITS(map, id);
    return map;
}

bool NomMap_MoveNext(
    NomValue        map,
    NomMapIterator* iterator
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    NomState* state = NomValue_GetState(map);

    ObjectId id = GET_ID_BITS(map);
    MapData* data = Heap_GetData(state->heap, id);

    // Initialize a hash table iterator at the same location as the map
    // iterator
    HashTableIterator hashTableIterator = { 0 };
    if (iterator->map.data == 0)
    {
        iterator->map = map;
    }
    else
    {
        hashTableIterator.hashTable = data->hashTable;
    }
    hashTableIterator.index = iterator->index;
    hashTableIterator.bucketNode = iterator->bucketNode;

    // Move to the next pair in the hash table
    bool result = HashTable_MoveNext(data->hashTable, &hashTableIterator);

    // Sync the hash table iterator with the map iterator
    iterator->index = hashTableIterator.index;
    iterator->bucketNode = hashTableIterator.bucketNode;
    iterator->key.data = hashTableIterator.key;
    iterator->value.data = hashTableIterator.value;

    return result;
}

bool NomMap_Insert(
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    NomState* state = NomValue_GetState(map);

    ObjectId id = GET_ID_BITS(map);
    MapData* data = Heap_GetData(state->heap, id);
    return HashTable_Insert(data->hashTable, (UserData)key.data, (UserData)value.data);
}

bool NomMap_Set(
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    NomState* state = NomValue_GetState(map);

    ObjectId id = GET_ID_BITS(map);
    MapData* data = Heap_GetData(state->heap, id);
    return HashTable_Set(data->hashTable, (UserData)key.data, (UserData)value.data);
}

bool NomMap_InsertOrSet(
    NomValue    map,
    NomValue    key,
    NomValue    value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    NomState* state = NomValue_GetState(map);

    ObjectId id = GET_ID_BITS(map);
    MapData* data = Heap_GetData(state->heap, id);
    return HashTable_InsertOrSet(data->hashTable, (UserData)key.data, (UserData)value.data);
}

NomValue NomMap_Get(
    NomValue    map,
    NomValue    key
    )
{
    NomValue value = { 0 };
    NomMap_TryGet(map, key, &value);
    return value;
}

bool NomMap_TryGet(
    NomValue    map,
    NomValue    key,
    NomValue*   value
    )
{
    if (!NomMap_Check(map))
    {
        return false;
    }

    NomState* state = NomValue_GetState(map);

    ObjectId id = GET_ID_BITS(map);
    MapData* data = Heap_GetData(state->heap, id);
    return HashTable_Get(data->hashTable, (UserData)key.data, (UserData*)&value->data);
}