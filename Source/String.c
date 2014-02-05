#include "NomString.h"

#include "State.h"
#include "Heap.h"

#include <stdlib.h>
#include <string.h>

NomValue NomString_FromString(NomState* s, const char* string)
{
    NomValue value = NOM_NIL;
    value.type = NOM_TYPE_STRING;
    value.data.handle = Heap_Alloc(s->heap, strlen(string) + 1, free);
    strcpy((char*)Heap_GetData(s->heap, value.data.handle), string);
    return value;
}

const char* NomString_AsString(NomState* s, NomValue value)
{
    if (value.type != NOM_TYPE_STRING)
    {
        NomState_SetError(s, "Value is not a string");
        return NULL;
    }

    return (const char*)Heap_GetData(s->heap, value.data.handle);
}