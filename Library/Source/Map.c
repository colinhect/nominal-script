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

#include <stdlib.h>

typedef struct
{
    HashTable*  hashTable;
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
    ObjectId id = Heap_Alloc(heap, sizeof(MapData), free);
    MapData* data = Heap_GetData(heap, id);
    data->hashTable = HashTable_Create(HashValue, CompareValue, (UserData)state, 32);

    SET_ID(map, id);
    return map;
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

    // Initialize a hash table iterator at the same location as the map
    // iterator
    HashTableIterator hashTableIterator = { 0 };
    if (iterator->source.raw == 0)
    {
        iterator->source = map;
    }
    else
    {
        hashTableIterator.hashTable = data->hashTable;
    }
    hashTableIterator.index = iterator->data.map.index;
    hashTableIterator.bucketNode = iterator->data.map.bucketNode;

    // Move to the next pair in the hash table
    bool result = HashTable_MoveNext(data->hashTable, &hashTableIterator);

    // Sync the hash table iterator with the map iterator
    iterator->data.map.index = hashTableIterator.index;
    iterator->data.map.bucketNode = hashTableIterator.bucketNode;
    iterator->key.raw = hashTableIterator.key;
    iterator->value.raw = hashTableIterator.value;

    return result;
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
    return HashTable_Insert(data->hashTable, (UserData)key.raw, (UserData)value.raw);
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
    return HashTable_InsertOrSet(data->hashTable, (UserData)key.raw, (UserData)value.raw);
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
