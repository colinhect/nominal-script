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
#include "function.h"

#include "value.h"
#include "map.h"
#include "state.h"
#include "heap.h"

#include <assert.h>
#include <stdlib.h>

#define MAX_FUNCTION_PARAMS (16)

typedef struct
{
    uint32_t    ip;
    NomFunction nativefunction;
    StringId    params[MAX_FUNCTION_PARAMS];
    size_t      paramcount;
} FunctionData;

static NomValue allocfunction(
        NomState*       state,
        FunctionData**  data
    )
{
    assert(state);
    assert(data);

    NomValue value = nom_nil();
    SET_TYPE(value, TYPE_FUNCTION);

    Heap* heap = state_getheap(state);
    HeapObjectId id = heap_alloc(heap, sizeof(FunctionData), free);
    *data = heap_getdata(heap, id);

    SET_ID(value, id);
    return value;
}

bool nom_isfunction(
    NomValue value
    )
{
    return GET_TYPE(value) == TYPE_FUNCTION;
}

NomValue nom_newfunction(
    NomState*   state,
    NomFunction function
    )
{
    assert(state);

    FunctionData* data;
    NomValue value = allocfunction(state, &data);
    data->ip = 0;
    data->nativefunction = function;
    data->paramcount = 0;

    return value;
}

NomValue function_new(
    NomState*   state,
    uint32_t    ip
    )
{
    assert(state);

    FunctionData* data;
    NomValue value = allocfunction(state, &data);
    data->ip = ip;
    data->nativefunction = NULL;
    data->paramcount = 0;

    return value;
}

void function_addparam(
    NomState*   state,
    NomValue    function,
    StringId    param
    )
{
    assert(state);

    if (!nom_isfunction(function))
    {
        return;
    }

    HeapObjectId id = GET_ID(function);
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

    if (!nom_isfunction(function))
    {
        return 0;
    }

    HeapObjectId id = GET_ID(function);
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

    if (!nom_isfunction(function))
    {
        return (StringId)-1;
    }

    HeapObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->params[index];
}

bool function_isnative(
    NomState*   state,
    NomValue    function
    )
{
    return function_getnative(state, function) != NULL;
}

NomFunction function_getnative(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!nom_isfunction(function))
    {
        return NULL;
    }

    HeapObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->nativefunction;
}

uint32_t function_getip(
    NomState*   state,
    NomValue    function
    )
{
    assert(state);

    if (!nom_isfunction(function))
    {
        return (uint32_t)-1;
    }

    HeapObjectId id = GET_ID(function);
    Heap* heap = state_getheap(state);
    FunctionData* data = heap_getdata(heap, id);

    return data->ip;
}
