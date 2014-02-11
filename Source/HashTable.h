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
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

///
/// \brief The data type used for keys/values/contexts in a hash table.
typedef uint64_t UserData;

///
/// \brief The result of a hash.
typedef uint64_t Hash;

///
/// \brief A hash function.
///
/// \param key
///     The key to hash.
/// \param context
///     The context.
///
/// \returns The hash value.
typedef Hash (*HashFunction)(
    UserData    key,
    UserData    context
    );

///
/// \brief A comparison function.
///
/// \param left
///     The left operand.
/// \param right
///     The right operand.
/// \param context
///     The context.
///
/// \returns 0 if the values are equal; non-zero otherwise.
typedef int (*CompareFunction)(
    UserData    left,
    UserData    right,
    UserData    context
    );

///
/// \brief A hash table.
typedef struct _HashTable HashTable;

///
/// \brief Creates a new hash table.
///
/// \param hash
///     The hash function to use.
/// \param compare
///     The compare function to use.
/// \param context
///        The context passed to the hash/compare functions.
/// \param bucketCount
///     The number of buckets to use.
///
/// \returns The new hash table.
HashTable* HashTable_Create(
    HashFunction    hash,
    CompareFunction compare,
    UserData        context,
    size_t          bucketCount
    );

///
/// \brief Frees a hash table.
///
/// \param hashTable
///     The hash table to free.
/// \param freeKey
///     The function to use to free a key (can be NULL).
/// \param freeValue
///     The function to use to free a value (can be NULL).
void HashTable_Free(
    HashTable*  hashTable,
    void        (*freeKey)(void*),
    void        (*freeValue)(void*)
    );

///
/// \brief Inserts a new value in the hash table.
///
/// \param hashTable
///     The hash table.
/// \param key
///     The key.
/// \param value
///     The value.
///
/// \returns True if the insertion was successful; false if a value already
/// exists for the given key.
bool HashTable_Insert(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    );

///
/// \brief Sets an existing value in the hash table.
///
/// \param hashTable
///     The hash table.
/// \param key
///     The key.
/// \param value
///     The value.
///
/// \returns True if the insertion was successful; false if a value already
/// exists for the given key.
bool HashTable_Set(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    );

///
/// \brief Gets the value for a key.
///
/// \param hashTable
///     The hash table.
/// \param key
///     The key.
/// \param value
///     A pointer to where to store the found value.
///
/// \returns True if a value was found for the given key; false if no value
/// was found for the given key.
bool HashTable_Find(
    HashTable*  hashTable,
    UserData    key,
    UserData*   value
    );

///
/// \brief Either instert a new value or get an existing value.
///
/// \param hashTable
///     The hash table.
/// \param key
///     The key.
/// \param value
///     The value.
/// \param existingValue
///     A pointer to where to store the found value (if an existing value is
///     found).
///
/// \returns True if a value was found for the given key; false if no value
/// was found for the given and a new value was inserted.
bool HashTable_InsertOrFind(
    HashTable*  hashTable,
    UserData    key,
    UserData    value,
    UserData*   existingValue
    );

///
/// \brief Hashes a string.
///
/// \param key
///     The string to hash (casted to const char*).
/// \param context
///     The context (ignored).
///
/// \returns The hash value.
Hash HashString(
    UserData    key,
    UserData    context
    );

///
/// \brief Compares strings.
///
/// \param left
///     The left string (casted to const char*).
/// \param right
///     The right string (casted to const char*).
/// \param context
///     The context (ignored).
///
/// \returns 0 if the string are equal; non-zero otherwise.
int CompareString(
    UserData    left,
    UserData    right,
    UserData    context
    );

///
/// \brief Returns the literal value of the key as the hash.
///
/// \param key
///     The key to hash.
/// \param context
///     The context (ignored).
/// 
/// \returns The hash value.
Hash HashIdentity(
    UserData    key,
    UserData    context
    );

///
/// \brief Compares the literal value of the keys
///
/// \param left
///     The left key.
/// \param right
///     The right key.
/// \param context
///     The context (ignored).
///
/// \returns 0 if the keys are equal; 1 if the left value is greater than the
/// right; -1 if the left value is greater than the left.
int CompareIdentity(
    UserData    left,
    UserData    right,
    UserData    context
    );

#endif