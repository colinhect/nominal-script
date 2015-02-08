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

#include "HashTable.h"

#include <string.h>

typedef uint32_t StringId;

typedef struct StringPool StringPool;

// Creates a new string pool
StringPool* StringPool_New(
    size_t  stringCount
    );

// Frees a string pool
void StringPool_Free(
    StringPool* stringPool
    );

// Inserts a new string or gets the ID of an existing string, returning the ID
// associated with the specified string
StringId StringPool_InsertOrFind(
    StringPool* stringPool,
    const char* string
    );

// Inserts a new string or gets the ID of an existing string, returning the ID
// associated with the specified string
StringId StringPool_InsertOrFindSubString(
    StringPool* stringPool,
    const char* string,
    size_t      length
    );

// Returns the string value from a string ID (NULL if no string exists of the
// given ID)
const char* StringPool_Find(
    StringPool* stringPool,
    StringId    id
    );

// Returns the hash from a string ID
Hash StringPool_Hash(
    StringPool* stringPool,
    StringId    id
    );

#endif
