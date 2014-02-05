#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct _Heap Heap;

typedef uint32_t ObjectHandle;

typedef void (*FreeFunction)(void* data);

Heap* Heap_Create();
void Heap_Free(Heap* h);

ObjectHandle Heap_Alloc(Heap* h, size_t size, FreeFunction free);

void* Heap_GetData(Heap* h, ObjectHandle handle);

#endif