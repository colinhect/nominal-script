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

typedef struct _StringPool StringPool;

///
/// \brief Creates a new string pool.
///
/// \param stringCount
///     The maximum number of string the pool can contain.
/// 
/// \returns The new string pool.
StringPool* StringPool_Create(
    size_t  stringCount
    );

///
/// \brief Frees a string pool.
///
/// \param stringPool
///     The string pool to free.
void StringPool_Free(
    StringPool* stringPool
    );

///
/// \brief Inserts a new string or gets the ID of an existing string.
///
/// \param stringPool
///     The string pool.
/// \param string
///     The string.
///
/// \returns The string ID associated with the given string.
StringId StringPool_InsertOrFind(
    StringPool* stringPool,
    const char* string
    );

///
/// \brief Inserts a new string or gets the ID of an existing string.
///
/// \param stringPool
///     The string pool.
/// \param string
///     The string
/// \param length
///     The number of characters to include from the string.
///
/// \returns The string ID associated with the given string.
StringId StringPool_InsertOrFindSubString(
    StringPool* stringPool,
    const char* string,
    size_t      length
    );

///
/// \brief Gets the string value from a string ID.
///
/// \param stringPool
///     The string pool.
/// \param id
///     The ID of the string to get the value for.
///
/// \returns The string value; NULL if no string exists of the given ID.
const char* StringPool_Find(
    StringPool* stringPool,
    StringId    id
    );

///
/// \brief Returns the hash from a string ID.
///
/// \param stringPool
///     The string pool.
/// \param id
///     The ID of the string to get the hash for.
///
/// \returns The hash.
Hash StringPool_Hash(
    StringPool* stringPool,
    StringId    id
    );

#endif