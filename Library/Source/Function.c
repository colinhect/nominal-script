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
#include "Function.h"
#include "Map.h"
#include "State.h"
#include "Heap.h"

#include <assert.h>
#include <stdlib.h>

#define MAX_FUNCTION_PARAMS (16)

typedef struct
{
    uint32_t    ip;
    StringId    parameters[MAX_FUNCTION_PARAMS];
    size_t      parameterCount;
} FunctionData;

bool Function_Check(
    NomValue value
    )
{
    return GET_TYPE(value) == TYPE_FUNCTION;
}

NomValue Function_New(
    NomState*   state,
    uint32_t    ip
    )
{
    assert(state);

    NomValue function = Nom_Nil();
    SET_TYPE(function, TYPE_FUNCTION);

    Heap* heap = State_GetHeap(state);
    ObjectId id = Heap_Alloc(heap, sizeof(FunctionData), free);
    FunctionData* data = Heap_GetData(heap, id);
    data->ip = ip;
    data->parameterCount = 0;

    SET_ID(function, id);
    return function;
}

void Function_AddParameter(
    NomState*   state,
    NomValue    function,
    StringId    parameter
    )
{
    assert(state);

    if (!Function_Check(function))
    {
        return;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = State_GetHeap(state);
    FunctionData* data = Heap_GetData(heap, id);

    data->parameters[data->parameterCount++] = parameter;
}

size_t Function_GetParameterCount(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!Function_Check(function))
    {
        return 0;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = State_GetHeap(state);
    FunctionData* data = Heap_GetData(heap, id);

    return data->parameterCount;
}

StringId Function_GetParameter(
    NomState*   state,
    NomValue    function,
    size_t      index
    )
{
    assert(state);

    if (!Function_Check(function))
    {
        return (StringId)-1;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = State_GetHeap(state);
    FunctionData* data = Heap_GetData(heap, id);

    return data->parameters[index];
}

uint32_t Function_GetInstructionPointer(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!Function_Check(function))
    {
        return (uint32_t)-1;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = State_GetHeap(state);
    FunctionData* data = Heap_GetData(heap, id);

    return data->ip;
}
