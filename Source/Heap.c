#include "Heap.h"

#include <stdlib.h>
#include <string.h>

typedef struct _HeapObject
{
    void* data;
    void (*free)(void*);
} HeapObject;

typedef struct _Heap
{
    HeapObject* objects;
    size_t objectCapacity;
    ObjectHandle nextHandle;
} Heap;

#define DEFAULT_HEAP_SIZE   (65536) // 2 ^ 16

Heap* Heap_Create()
{
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->objects = (HeapObject*)malloc(sizeof(HeapObject) * DEFAULT_HEAP_SIZE);
    memset(h->objects, 0, sizeof(HeapObject) * DEFAULT_HEAP_SIZE);
    h->objectCapacity = DEFAULT_HEAP_SIZE;
    h->nextHandle = 0;
    return h;
}

void Heap_Free(Heap* h)
{
    ObjectHandle i;
    for (i = 0; i < h->objectCapacity; ++i)
    {
        HeapObject* object = &h->objects[i];
        if (object->data && object->free)
        {
            object->free(object->data);
        }
    }

    free(h->objects);
    free(h);
}

ObjectHandle Heap_Alloc(Heap* h, size_t size, void (*free)(void*))
{
    ObjectHandle handle = h->nextHandle++;
    HeapObject* object = &h->objects[handle];

    object->data = malloc(size);
    object->free = free;

    return handle;
}

void* Heap_GetData(Heap* h, ObjectHandle handle)
{
    HeapObject* object = &h->objects[handle];
    return object->data;
}