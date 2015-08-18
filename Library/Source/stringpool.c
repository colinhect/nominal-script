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
#include "stringpool.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

StringPool* stringpool_new(
    size_t  stringcount
)
{
    StringPool* stringpool = (StringPool*)malloc(sizeof(StringPool));
    assert(stringpool);
    stringpool->hashtable = hashtable_new(hashstring, comparestring, 0, stringcount * 2);

    stringpool->strings = (char**)malloc(sizeof(char*)* stringcount);
    assert(stringpool->strings);
    memset(stringpool->strings, 0, sizeof(char*) * stringcount);

    stringpool->hashes = (Hash*)malloc(sizeof(Hash) * stringcount);
    assert(stringpool->hashes);
    memset(stringpool->hashes, 0, sizeof(Hash)* stringcount);

    stringpool->stringcount = stringcount;
    stringpool->nextid = 0;
    return stringpool;
}

void stringpool_free(
    StringPool* stringpool
)
{
    assert(stringpool);

    // Free the string hashes
    if (stringpool->hashes)
    {
        free(stringpool->hashes);
    }

    // Free the individual strings
    if (stringpool->strings)
    {
        StringId i;
        for (i = 0; i < stringpool->stringcount; ++i)
        {
            if (stringpool->strings[i])
            {
                free(stringpool->strings[i]);
            }
        }
    }

    // Free the strings array
    if (stringpool->strings)
    {
        free(stringpool->strings);
    }

    // Free the hash table of strings
    if (stringpool->hashtable)
    {
        hashtable_free(stringpool->hashtable, NULL, NULL);
    }

    free(stringpool);
}

StringId stringpool_getid(
    StringPool* stringpool,
    const char* string
)
{
    return stringpool_getidsubstring(stringpool, string, strlen(string));
}

StringId stringpool_getidsubstring(
    StringPool* stringpool,
    const char* string,
    size_t      length)
{
    char* newstring = (char*)malloc(sizeof(char) * (length + 1));
    assert(newstring);

    memcpy(newstring, string, length);
    newstring[length] = '\0';

    StringId id;
    UserData outputId = 0;
    if (!hashtable_insertorget(stringpool->hashtable, (UserData)newstring, (UserData)stringpool->nextid, &outputId))
    {
        id = stringpool->nextid;
        stringpool->strings[id] = newstring;
        stringpool->hashes[id] = hashstring((UserData)newstring, 0);
        ++stringpool->nextid;
    }
    else
    {
        id = (StringId)outputId;
        free(newstring);
    }
    return id;
}

const char* stringpool_find(
    StringPool* stringpool,
    StringId    id
)
{
    return stringpool->strings[id];
}

Hash stringpool_hash(
    StringPool* stringpool,
    StringId    id
)
{
    return stringpool->hashes[id];
}
