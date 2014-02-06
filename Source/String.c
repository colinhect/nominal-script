#include "String.h"
#include "State.h"
#include "Heap.h"

#include <stdlib.h>
#include <string.h>

NomValue NomString_FromString(NomState* s, const char* string)
{
    NomValue value = { 0 };
    value.type = NOM_TYPE_STRING;
    value.data.handle = Heap_Alloc(s->heap, strlen(string) + 1, free);
    strcpy((char*)Heap_GetData(s->heap, value.data.handle), string);
    return value;
}

const char* NomString_AsString(NomState* s, NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_STRING:
        return (const char*)Heap_GetData(s->heap, value.data.handle);
        break;
    case NOM_TYPE_STATIC_STRING:
        return StringPool_Find(s->stringPool, value.data.handle);
        break;
    default:
        NomState_SetError(s, "Value is not a string");
    }
    return NULL;
}

NomValue NomString_FromId(StringId id)
{
    NomValue value = { 0 };
    value.type = NOM_TYPE_STATIC_STRING;
    value.data.handle = id;
    return value;
}