///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2014 Colin Hill
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
    return StringPool_InsertOrFindSubString(s, string, strlen(string));
}

StringId StringPool_InsertOrFindSubString(StringPool* s, const char* string, size_t length)
{
    UserData outputId;
    StringId id;
    char* newString = (char*)malloc(sizeof(char) * (length + 1));
    memcpy(newString, string, length);
    newString[length] = '\0';

    if (!HashTable_InsertOrFind(s->hashTable, (UserData)newString, (UserData)s->nextStringId, &outputId))
    {
        id = s->nextStringId;
        s->strings[id] = newString;
        ++s->nextStringId;
    }
    else
    {
        id = (StringId)outputId;
        free(newString);
    }
    return id;
}

const char* StringPool_Find(StringPool* s, StringId id)
{
    return s->strings[id];
}