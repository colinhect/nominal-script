#include "StringPool.h"

#include <stdlib.h>
#include <string.h>

StringPool* CreateStringPool(size_t stringCount)
{
    StringPool* stringPool = (StringPool*)malloc(sizeof(StringPool));
    stringPool->hashTable = CreateHashTable(HashString, CompareString, stringCount);
    stringPool->strings = (char**)malloc(sizeof(char*) * stringCount);
    stringPool->stringCount = stringCount;
    stringPool->nextStringId = 0;
    memset(stringPool->strings, 0, sizeof(char*) * stringCount);
    return stringPool;
}

void FreeStringPool(StringPool* stringPool)
{
    StringId i;
    for (i = 0; i < stringPool->stringCount; ++i)
    {
        if (stringPool->strings[i])
        {
            free(stringPool->strings[i]);
        }
    }

    FreeHashTable(stringPool->hashTable);
    free(stringPool);
}

StringId AddOrGetString(StringPool* stringPool, const char* string)
{
    StringId id;
    char* newString = (char*)malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(newString, string);

    if (!InsertOrFind(stringPool->hashTable, (void*)string, (void*)stringPool->nextStringId, (void**)&id))
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

const char* GetString(StringPool* stringPool, StringId id)
{
    return stringPool->strings[id];
}