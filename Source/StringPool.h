#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include <string.h>

#include "HashTable.h"

typedef size_t StringId;

typedef struct
{
    HashTable* hashTable;
    char** strings;
    size_t stringCount;
    StringId nextStringId;
} StringPool;

StringPool* CreateStringPool(size_t stringCount);
void FreeStringPool(StringPool* stringPool);

StringId AddOrGetString(StringPool* stringPool, const char* string);
const char* GetString(StringPool* stringPool, StringId id);

#endif