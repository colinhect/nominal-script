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
    StringId    params[MAX_FUNCTION_PARAMS];
    size_t      paramcount;
} FunctionData;

bool function_check(
    NomValue value
    )
{
    return GET_TYPE(value) == TYPE_FUNCTION;
}

NomValue function_new(
    NomState*   state,
    uint32_t    ip
    )
{
    assert(state);

    NomValue function = nom_nil();
    SET_TYPE(function, TYPE_FUNCTION);

    Heap* heap = state_getheap(state);
    ObjectId id = heap_alloc(heap, sizeof(FunctionData), free);
    FunctionData* data = heap_getdata(heap, id);
    data->ip = ip;
    data->paramcount = 0;

    SET_ID(function, id);
    return function;
}

void function_addparam(
    NomState*   state,
    NomValue    function,
    StringId    param
    )
{
    assert(state);

    if (!function_check(function))
    {
        return;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    data->params[data->paramcount++] = param;
}

size_t function_getparamcount(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!function_check(function))
    {
        return 0;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->paramcount;
}

StringId function_getparam(
    NomState*   state,
    NomValue    function,
    size_t      index
    )
{
    assert(state);

    if (!function_check(function))
    {
        return (StringId)-1;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->params[index];
}

uint32_t function_getip(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!function_check(function))
    {
        return (uint32_t)-1;
    }

    ObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->ip;
}
