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
#include "Value.h"
#include "Closure.h"
#include "Map.h"
#include "State.h"
#include "Heap.h"

#include <assert.h>
#include <stdlib.h>

typedef struct
{
    uint32_t    ip;
    NomValue    scope;
} ClosureData;

bool NomClosure_Check(
    NomValue value
    )
{
    return GET_TYPE(value) == TYPE_CLOSURE;
}

NomValue NomClosure_Create(
    NomState*   state,
    uint32_t    ip
    )
{
    assert(state);

    NomValue closure = NomValue_Nil();
    SET_TYPE(closure, TYPE_CLOSURE);

    Heap* heap = NomState_GetHeap(state);
    ObjectId id = Heap_Alloc(heap, sizeof(ClosureData), free);
    ClosureData* data = Heap_GetData(heap, id);
    data->ip = ip;
    data->scope = NomMap_Create(state);

    SET_ID(closure, id);
    return closure;
}

NomValue NomClosure_GetScope(
    NomState*   state,
    NomValue    closure
    )
{
    assert(state);

    if (!NomClosure_Check(closure))
    {
        return NomValue_Nil();
    }

    ObjectId id = GET_ID(closure);
    Heap* heap = NomState_GetHeap(state);
    ClosureData* data = Heap_GetData(heap, id);

    return data->scope;
}

uint32_t NomClosure_GetInstructionPointer(
    NomState*   state,
    NomValue    closure
    )
{
    assert(state);

    if (!NomClosure_Check(closure))
    {
        return (uint32_t)-1;
    }

    ObjectId id = GET_ID(closure);
    Heap* heap = NomState_GetHeap(state);
    ClosureData* data = Heap_GetData(heap, id);

    return data->ip;
}
