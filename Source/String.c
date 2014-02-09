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
#include "State.h"
#include "Heap.h"

#include <stdlib.h>
#include <string.h>

bool NomString_Check(
    NomValue    value
    )
{
    return value.fields.type == NOM_TYPE_STRING
        || value.fields.type == NOM_TYPE_STATIC_STRING;
}

NomValue NomString_FromString(
    NomState*   state,
    const char* string
    )
{
    NomValue value = { 0 };
    value.fields.type = NOM_TYPE_STRING;
    value.fields.data.handle = Heap_Alloc(state->heap, strlen(string) + 1, free);
    strcpy((char*)Heap_GetData(state->heap, value.fields.data.handle), string);
    return value;
}

const char* NomString_AsString(
    NomState*   state,
    NomValue    value
    )
{
    switch (value.fields.type)
    {
    case NOM_TYPE_STRING:
        return (const char*)Heap_GetData(state->heap, value.fields.data.handle);
        break;
    case NOM_TYPE_STATIC_STRING:
        return StringPool_Find(state->stringPool, value.fields.data.handle);
        break;
    default:
        NomState_SetError(state, "Value is not a string");
    }
    return NULL;
}

NomValue NomString_FromId(
    StringId    id
    )
{
    NomValue value = { 0 };
    value.fields.type = NOM_TYPE_STATIC_STRING;
    value.fields.data.handle = (unsigned)id;
    return value;
}