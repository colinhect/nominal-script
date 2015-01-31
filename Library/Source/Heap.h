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

#include <stddef.h>
#include <stdint.h>

///
/// \brief A heap of GC objects.
typedef struct Heap Heap;

///
/// \brief A handle to a GC object.
///
/// \remarks Specific to a certain heap.
typedef uint32_t ObjectId;

///
/// \brief Creates a new heap.
///
/// \returns The new heap.
Heap* Heap_Create(
    void
    );

///
/// \brief Frees a heap.
///
/// \param heap
///     The heap to free.
void Heap_Free(
    Heap*   heap
    );

///
/// \brief Allocates a new object in the heap.
///
/// \param heap
///     The heap.
/// \param size
///     The size of the object to allocate.
/// \param free
///     The function used to free the object when it is collected.
///
/// \returns A ID to the new object.
ObjectId Heap_Alloc(
    Heap*   heap,
    size_t  size,
    void    (*free)(void*)
    );

///
/// \param Returns a pointer to the data of an object on the heap.
///
/// \param heap
///     The heap.
/// \param id
///     The ID of the object to get the data for.
///
/// \returns A pointer to the object's data.
void* Heap_GetData(
    Heap*       heap,
    ObjectId    id
    );

#endif
