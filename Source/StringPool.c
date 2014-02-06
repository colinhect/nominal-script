#include "StringPool.h"
#include "HashTable.h"

#include <stdlib.h>
#include <string.h>

typedef struct _StringPool
{
    HashTable* hashTable;
    char** strings;
    size_t stringCount;
    StringId nextStringId;
} StringPool;

StringPool* StringPool_Create(size_t stringCount)
{
    StringPool* s = (StringPool*)malloc(sizeof(StringPool));
    s->hashTable = HashTable_Create(HashString, CompareString, stringCount);
    s->strings = (char**)malloc(sizeof(char*) * stringCount);
    memset(s->strings, 0, sizeof(char*) * stringCount);
    s->stringCount = stringCount;
    s->nextStringId = 0;
    return s;
}

void StringPool_Free(StringPool* s)
{
    StringId i;
    for (i = 0; i < s->stringCount; ++i)
    {
        if (s->strings[i])
        {
            free(s->strings[i]);
        }
    }

    HashTable_Free(s->hashTable, NULL, NULL);
    free(s);
}

StringId StringPool_InsertOrFind(StringPool* s, const char* string)
{
    StringId id;
    char* newString = (char*)malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(newString, string);

    if (!HashTable_InsertOrFind(s->hashTable, (void*)string, (void*)s->nextStringId, (void**)&id))
    {
        id = s->nextStringId;
        s->strings[id] = newString;
        ++s->nextStringId;
    }
    else
    {
        free(newString);
    }
    return id;
}

const char* StringPool_Find(StringPool* s, StringId id)
{
    return s->strings[id];
}