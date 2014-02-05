#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include <string.h>

typedef size_t StringId;

typedef struct _StringPool StringPool;

StringPool* StringPool_Create(size_t stringCount);
void StringPool_Free(StringPool* stringPool);

StringId StringPool_AddOrGet(StringPool* stringPool, const char* string);
const char* StringPool_Get(StringPool* stringPool, StringId id);

#endif