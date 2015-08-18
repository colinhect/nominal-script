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
#ifndef HEAP_H
#define HEAP_H

#include "value.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define INITIAL_HEAP_SIZE   (65536) // 2 ^ 16

// A handle to a GC object (specific to a certain heap)
typedef uint32_t HeapObjectId;

// A GC object on the heap
typedef struct HeapObject
{
    ObjectType  type;
    void*       data;
    void(*free)(void*);
    int32_t     refcount;
    bool        marked;
} HeapObject;

// A heap of GC objects
typedef struct Heap
{
    HeapObject*     objects;
    uint32_t        capacity;
    HeapObjectId    nextid;
    HeapObjectId    maxid;
} Heap;

// Creates a new heap
Heap* heap_new(
    void
);

// Frees a heap
void heap_free(
    Heap*   heap
);

// Allocates a new object in the heap given the size (in bytes) of the data to
// allocate and the function used to free the object when it is collected
NomValue heap_alloc(
    Heap*       heap,
    ObjectType  type,
    size_t      size,
    void        (*free)(void*)
);

// Deallocates an object in the heap
void heap_dealloc(
    Heap*       heap,
    NomValue    value
);

// Returns a pointer to an object on the heap
HeapObject* heap_getobject(
    Heap*       heap,
    NomValue    value
);

// Returns a pointer to the data of an object on the heap
void* heap_getdata(
    Heap*       heap,
    NomValue    value
);

// Marks an object to be deallocated on the next sweep
void heap_mark(
    Heap*       heap,
    NomValue    value
);

// Deallocates all unmarked objects in the heap, returning the number of
// objects collected
unsigned heap_sweep(
    Heap*   heap
);

#endif
