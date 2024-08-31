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
#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include "hashtable.h"

#include <string.h>

// A numeric identifier for a string in a string pool
typedef uint32_t StringId;

// A pool of strings
typedef struct StringPool
{
    HashTable*  hashtable;
    char**      strings;
    Hash*       hashes;
    size_t      stringcount;
    StringId    nextid;
} StringPool;

// Creates a new string pool
StringPool* stringpool_new(
    size_t  stringcount
);

// Frees a string pool
void stringpool_free(
    StringPool* stringpool
);

// Inserts a new string or gets the ID of an existing string, returning the ID
// associated with the specified string
StringId stringpool_getid(
    StringPool* stringpool,
    const char* string
);

// Inserts a new string or gets the ID of an existing string, returning the ID
// associated with the specified string
StringId stringpool_getidsubstring(
    StringPool* stringpool,
    const char* string,
    size_t      length
);

// Returns the string value from a string ID (NULL if no string exists of the
// given ID)
const char* stringpool_find(
    StringPool* stringpool,
    StringId    id
);

// Returns the hash from a string ID
Hash stringpool_hash(
    StringPool* stringpool,
    StringId    id
);

#endif
