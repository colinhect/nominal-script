#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include <string.h>

typedef size_t StringId;

typedef struct _StringPool StringPool;

StringPool* StringPool_Create(size_t stringCount);
void StringPool_Free(StringPool* s);

StringId StringPool_InsertOrFind(StringPool* s, const char* string);
const char* StringPool_Find(StringPool* s, StringId id);

#endif