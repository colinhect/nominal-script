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
#include "String.h"
#include "StringPool.h"
#include "State.h"
#include "Heap.h"
#include "Type.h"
#include "Value.h"

#include <stdlib.h>
#include <string.h>

bool NomString_Check(
    NomValue    value
    )
{
    Type type = GET_TYPE_BITS(value);
    return type == TYPE_STRING
        || type == TYPE_POOLED_STRING;
}

NomValue NomString_FromString(
    NomState*   state,
    const char* string,
    bool        pooled
    )
{
    NomValue value = NomValue_Nil();

    if (pooled)
    {
        StringId id = StringPool_InsertOrFind(state->stringPool, string);
        SET_TYPE_BITS(value, TYPE_POOLED_STRING);
        SET_ID_BITS(value, id);
    }
    else
    {
        ObjectId id = Heap_Alloc(state->heap, strlen(string) + 1, free);
        strcpy((char*)Heap_GetData(state->heap, id), string);
        SET_TYPE_BITS(value, TYPE_STRING);
        SET_ID_BITS(value, id);
    }

    return value;
}

const char* NomString_AsString(
    NomState*   state,
    NomValue    value
    )
{
    switch ((Type)GET_TYPE_BITS(value))
    {
    case TYPE_STRING:
        return (const char*)Heap_GetData(state->heap, GET_ID_BITS(value));
        break;
    case TYPE_POOLED_STRING:
        return StringPool_Find(state->stringPool, GET_ID_BITS(value));
        break;
    }
    return NULL;
}

NomValue NomString_FromId(
    StringId    id
    )
{
    NomValue value = NomValue_Nil();
    SET_TYPE_BITS(value, TYPE_POOLED_STRING);
    SET_ID_BITS(value, id);
    return value;
}