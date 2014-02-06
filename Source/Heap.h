#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct _Heap Heap;

typedef uint32_t ObjectHandle;

Heap* Heap_Create();
void Heap_Free(Heap* h);

ObjectHandle Heap_Alloc(Heap* h, size_t size, void (*free)(void*));

void* Heap_GetData(Heap* h, ObjectHandle handle);

#endif