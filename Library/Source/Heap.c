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
#include "Heap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void*   data;
    void    (*free)(void*);
} HeapObject;

struct Heap
{
    HeapObject* objects;
    size_t      objectCapacity;
    ObjectId    nextId;
};

#define INITIAL_HEAP_SIZE   (65536) // 2 ^ 16

Heap* Heap_Create(
    void
    )
{
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    assert(heap);

    size_t  objectsSize = sizeof(HeapObject) * INITIAL_HEAP_SIZE;
    heap->objects = (HeapObject*)malloc(objectsSize);
    assert(heap->objects);

    memset(heap->objects, 0, objectsSize);

    heap->objectCapacity = INITIAL_HEAP_SIZE;
    heap->nextId = 0;

    return heap;
}

void Heap_Free(
    Heap*   heap
    )
{
    assert(heap);
    assert(heap->objects);

    // For each (potential) object
    for (ObjectId i = 0; i < heap->objectCapacity; ++i)
    {
        HeapObject* object = &heap->objects[i];

        // If the object has data and a free function then free it
        if (object->data && object->free)
        {
            object->free(object->data);
        }
    }

    free(heap->objects);
    free(heap);
}

ObjectId Heap_Alloc(
    Heap*   heap,
    size_t  size,
    void    (*free)(void*)
    )
{
    ObjectId id = heap->nextId++;

    HeapObject* object = &heap->objects[id];
    object->data = malloc(size);
    object->free = free;

    return id;
}

void* Heap_GetData(
    Heap*       heap,
    ObjectId    id
    )
{
    HeapObject* object = &heap->objects[id];
    return object->data;
}
