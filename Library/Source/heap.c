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
#include "heap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Heap* heap_new(
    void
)
{
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    assert(heap);

    memset(heap, 0, sizeof(Heap));

    return heap;
}

void heap_free(
    Heap*   heap
)
{
    assert(heap);

    // Free all objects
    if (heap->objects)
    {
        NomValue value = nom_nil();
        SET_TYPE(value, VALUETYPE_OBJECT);

        for (HeapObjectId id = 0; id <= heap->maxid; ++id)
        {
            SET_ID(value, id);
            heap_dealloc(heap, value);
        }
    }

    // Free the array of objects
    free(heap->objects);

    free(heap);
}

NomValue heap_alloc(
    Heap*       heap,
    ObjectType  type,
    size_t      size,
    void        (*free)(void*)
)
{
    assert(heap);

    HeapObjectId id = heap->nextid++;

    // If the id exceeds the capacity
    if (id >= heap->capacity)
    {
        assert(heap->capacity < (1u << (sizeof(HeapObjectId) * 8u - 1u)));

        // Compute the new capacity
        heap->capacity = heap->capacity == 0 ? INITIAL_HEAP_SIZE : heap->capacity * 2;

        // Allocate the new array of objects
        size_t size = sizeof(HeapObject) * heap->capacity;
        HeapObject* objects = (HeapObject*)malloc(size);
        memset(objects, 0, size);
        assert(objects);

        // If the heap already had objects
        if (heap->objects)
        {
            // Copy over the first half of the objects
            memcpy(objects, heap->objects, size / 2);

            extern void free(void*);
            free(heap->objects);
        }

        heap->objects = objects;
    }

    heap->maxid = id > heap->maxid ? id : heap->maxid;

    HeapObject* object = &heap->objects[id];
    memset(object, 0, sizeof(HeapObject));

    object->type = type;
    object->data = malloc(size);
    assert(object->data);

    object->free = free;

    NomValue value = nom_nil();
    SET_TYPE(value, VALUETYPE_OBJECT);
    SET_ID(value, id);

    return value;
}

void heap_dealloc(
    Heap*       heap,
    NomValue    value
)
{
    assert(heap);

    HeapObject* object = heap_getobject(heap, value);

    // If the object has data and a free function then free it
    if (object && object->data && object->free)
    {
        object->free(object->data);
        object->data = NULL;
        object->refcount = 0;
    }
}

HeapObject* heap_getobject(
    Heap*       heap,
    NomValue    value
)
{
    assert(heap);

    HeapObject* object = NULL;

    ValueType type = GET_TYPE(value);
    if (type == VALUETYPE_OBJECT)
    {
        HeapObjectId id = GET_ID(value);
        assert(id < heap->capacity);

        object = &heap->objects[id];
    }

    return object;
}

void* heap_getdata(
    Heap*       heap,
    NomValue    value
)
{
    assert(heap);

    void* data = NULL;

    HeapObject* object = heap_getobject(heap, value);
    if (object)
    {
        data = object->data;
    }

    return data;
}

void heap_mark(
    Heap*       heap,
    NomValue    value
)
{
    assert(heap);

    HeapObject* object = heap_getobject(heap, value);
    if (object && object->data)
    {
        object->marked = true;
    }
}

unsigned int heap_sweep(
    Heap*   heap
)
{
    assert(heap);

    unsigned int count = 0;
    uint32_t newmaxid = 0;

    // For each potential heap object
    for (HeapObjectId id = 0; id <= heap->maxid; ++id)
    {
        HeapObject* object = &heap->objects[id];

        // If the object is allocated
        if (object->data)
        {
            newmaxid = id > newmaxid ? id : newmaxid;

            // Either unmark the object or free it
            if (object->marked)
            {
                object->marked = false;
            }
            else if (object->refcount <= 0)
            {
                NomValue value = nom_nil();
                SET_TYPE(value, VALUETYPE_OBJECT);
                SET_ID(value, id);

                heap_dealloc(heap, value);
                ++count;
            }
        }
    }

    // Establish a new upper bound of the largest object ID
    heap->maxid = newmaxid;

    return count;
}
