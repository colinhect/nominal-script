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

#include "Type.h"
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
    UserData    key,
    UserData    context
    )
{
    NomState* state = (NomState*)context;

    NomValue value;
    value.raw = key;

    Hash hash = value.raw;
    switch (value.fields.type)
    {
    case TYPE_INTEGER:
        hash = (Hash)value.fields.data.integerValue;
        break;
    case TYPE_REAL:
        hash = (Hash)value.fields.data.realValue;
        break;
    case TYPE_STRING:
    case TYPE_POOLED_STRING:
        hash = HashString((UserData)NomString_AsString(state, value), 0);
        break;
    case TYPE_NIL:
        hash = 0;
        break;
    }

    return (Hash)key;
}

int CompareValue(
    UserData    left,
    UserData    right,
    UserData    context
    )
{
    NomState* state = (NomState*)context;

    NomValue leftValue;
    leftValue.raw = left;

    NomValue rightValue;
    rightValue.raw = right;

    return NomValue_Compare(state, leftValue, rightValue);
}

bool NomMap_Check(
    NomValue value
    )
{
    return value.fields.type == TYPE_MAP;
}

NomValue NomMap_Create(
    NomState*   state
    )
{
    NomValue map = NomValue_Nil();
    map.fields.type = TYPE_MAP;

    ObjectHandle handle = Heap_Alloc(state->heap, sizeof(MapData), free);
    MapData* data = Heap_GetData(state->heap, handle);
    data->state = state;
    data->hashTable = HashTable_Create(HashValue, CompareValue, (UserData)state, 32);

    map.fields.data.handle = handle;
    return map;
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

    ObjectHandle handle = map.fields.data.handle;
    MapData* data = Heap_GetData(state->heap, handle);
    return HashTable_InsertOrSet(data->hashTable, (UserData)key.raw, (UserData)value.raw);
}

NomValue NomMap_Get(
    NomState*   state,
    NomValue    map,
    NomValue    key
    )
{
    NomValue value = NomValue_Nil();
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

    ObjectHandle handle = map.fields.data.handle;
    MapData* data = Heap_GetData(state->heap, handle);
    return HashTable_Find(data->hashTable, (UserData)key.raw, (UserData*)&value->raw);
}