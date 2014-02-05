#include "Heap.h"

#include <stdlib.h>
#include <string.h>

typedef struct _HeapObject
{
    void* data;
    FreeFunction free;
} HeapObject;

typedef struct _Heap
{
    HeapObject* objects;
    size_t objectCount;
    ObjectHandle nextHandle;
} Heap;

Heap* Heap_Create()
{
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->objects = (HeapObject*)malloc(sizeof(HeapObject) * 2048);
    memset(h->objects, 0, sizeof(HeapObject) * 2048);
    h->objectCount = 0;
    h->nextHandle = 0;
    return h;
}

void Heap_Free(Heap* h)
{
    ObjectHandle i;
    for (i = 0; i < h->objectCount; ++i)
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

ObjectHandle Heap_Alloc(Heap* h, size_t size, FreeFunction free)
{
    ObjectHandle handle = h->nextHandle++;
    HeapObject* object = &h->objects[handle];

    object->data = malloc(size);
    object->free = free;

    ++h->objectCount;

    return handle;
}

void* Heap_GetData(Heap* h, ObjectHandle handle)
{
    HeapObject* object = &h->objects[handle];
    return object->data;
}