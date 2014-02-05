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
    StringPool* stringPool = (StringPool*)malloc(sizeof(StringPool));
    stringPool->hashTable = HashTable_Create(HashString, CompareString, stringCount);
    stringPool->strings = (char**)malloc(sizeof(char*) * stringCount);
    stringPool->stringCount = stringCount;
    stringPool->nextStringId = 0;
    memset(stringPool->strings, 0, sizeof(char*) * stringCount);
    return stringPool;
}

void StringPool_Free(StringPool* stringPool)
{
    StringId i;
    for (i = 0; i < stringPool->stringCount; ++i)
    {
        if (stringPool->strings[i])
        {
            free(stringPool->strings[i]);
        }
    }

    HashTable_Free(stringPool->hashTable);
    free(stringPool);
}

StringId StringPool_AddOrGet(StringPool* stringPool, const char* string)
{
    StringId id;
    char* newString = (char*)malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(newString, string);

    if (!HashTable_InsertOrFind(stringPool->hashTable, (void*)string, (void*)stringPool->nextStringId, (void**)&id))
    {
        id = stringPool->nextStringId;
        stringPool->strings[id] = newString;
        ++stringPool->nextStringId;
    }
    else
    {
        free(newString);
    }
    return id;
}

const char* StringPool_Get(StringPool* stringPool, StringId id)
{
    return stringPool->strings[id];
}