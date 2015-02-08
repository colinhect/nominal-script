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
#include "String.h"
#include "StringPool.h"
#include "State.h"
#include "Heap.h"
#include "Value.h"

#include <stdlib.h>
#include <string.h>

bool Nom_IsString(
    NomValue    value
    )
{
    Type type = GET_TYPE(value);
    return type == TYPE_STRING
        || type == TYPE_INTERNED_STRING;
}

NomValue Nom_NewString(
    NomState*   state,
    const char* value,
    bool        interned
    )
{
    NomValue string = Nom_Nil();


    if (interned)
    {
        StringPool* stringPool = State_GetStringPool(state);
        StringId id = StringPool_InsertOrFind(stringPool, value);

        SET_TYPE(string, TYPE_INTERNED_STRING);
        SET_ID(string, id);
    }
    else
    {
        Heap* heap = State_GetHeap(state);
        ObjectId id = Heap_Alloc(heap, strlen(value) + 1, free);
        strcpy((char*)Heap_GetData(heap, id), value);

        SET_TYPE(string, TYPE_STRING);
        SET_ID(string, id);
    }

    return string;
}

const char* Nom_AsRawString(
    NomState*   state,
    NomValue    value
    )
{
    Heap* heap = State_GetHeap(state);
    StringPool* stringPool = State_GetStringPool(state);

    switch (GET_TYPE(value))
    {
    case TYPE_STRING:
        return (const char*)Heap_GetData(heap, GET_ID(value));
    case TYPE_INTERNED_STRING:
        return StringPool_Find(stringPool, GET_ID(value));
    }

    return NULL;
}

NomValue String_NewInterned(
    StringId    id
    )
{
    NomValue string = Nom_Nil();
    SET_TYPE(string, TYPE_INTERNED_STRING);
    SET_ID(string, id);
    return string;
}
