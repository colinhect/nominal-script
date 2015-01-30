///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2015 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "StringPool.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct _StringPool
{
    HashTable*  hashTable;
    char**      strings;
    Hash*       hashes;
    size_t      stringCount;
    StringId    nextStringId;
} StringPool;

StringPool* StringPool_Create(
    size_t  stringCount
    )
{
    StringPool* stringPool = (StringPool*)malloc(sizeof(StringPool));
    assert(stringPool);
    stringPool->hashTable = HashTable_Create(HashString, CompareString, 0, stringCount * 2);

    stringPool->strings = (char**)malloc(sizeof(char*)* stringCount);
    assert(stringPool->strings);
    memset(stringPool->strings, 0, sizeof(char*) * stringCount);

    stringPool->hashes = (Hash*)malloc(sizeof(Hash) * stringCount);
    assert(stringPool->hashes);
    memset(stringPool->hashes, 0, sizeof(Hash)* stringCount);

    stringPool->stringCount = stringCount;
    stringPool->nextStringId = 0;
    return stringPool;
}

void StringPool_Free(
    StringPool* stringPool
    )
{
    assert(stringPool);
    assert(stringPool->strings);

    StringId i;
    for (i = 0; i < stringPool->stringCount; ++i)
    {
        if (stringPool->strings[i])
        {
            free(stringPool->strings[i]);
        }
    }

    HashTable_Free(stringPool->hashTable, NULL, NULL);
    free(stringPool);
}

StringId StringPool_InsertOrFind(
    StringPool* stringPool,
    const char* string
    )
{
    return StringPool_InsertOrFindSubString(stringPool, string, strlen(string));
}

StringId StringPool_InsertOrFindSubString(
    StringPool* stringPool,
    const char* string,
    size_t      length)
{
    char* newString = (char*)malloc(sizeof(char) * (length + 1));
    assert(newString);

    memcpy(newString, string, length);
    newString[length] = '\0';

    StringId id;
    UserData outputId = 0;
    if (!HashTable_InsertOrGet(stringPool->hashTable, (UserData)newString, (UserData)stringPool->nextStringId, &outputId))
    {
        id = stringPool->nextStringId;
        stringPool->strings[id] = newString;
        stringPool->hashes[id] = HashString((UserData)newString, 0);
        ++stringPool->nextStringId;
    }
    else
    {
        id = (StringId)outputId;
        free(newString);
    }
    return id;
}

const char* StringPool_Find(
    StringPool* stringPool,
    StringId    id
    )
{
    return stringPool->strings[id];
}

Hash StringPool_Hash(
    StringPool* stringPool,
    StringId    id
    )
{
    return stringPool->hashes[id];
}