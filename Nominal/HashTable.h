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
///
/// \returns The hash value.
typedef Hash (*HashFunction)(
    UserData    key
    );

///
/// \brief A comparison function.
///
/// \param left
///     The left operand.
/// \param right
///     The right operand.
///
/// \returns True if the values are equal; false otherwise.
typedef bool (*CompareFunction)(
    UserData    left,
    UserData    right
    );

///
/// \brief A hash table.
typedef struct _HashTable HashTable;
typedef struct _BucketNode BucketNode;

///
/// \brief An iterator of a hash table.
typedef struct _HashTableIterator
{
    HashTable*  hashTable;
    size_t      index;
    UserData    key;
    UserData    value;
    BucketNode* bucketNode;
} HashTableIterator;

///
/// \brief Creates a new hash table.
///
/// \param hash
///     The hash function to use.
/// \param compare
///     The compare function to use.
/// \param bucketCount
///     The number of buckets to use.
///
/// \returns The new hash table.
HashTable* HashTable_Create(
    HashFunction    hash,
    CompareFunction compare,
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
/// \brief Moves to the next pair in the hash table.
///
/// \param hashTable
///     The hash table.
/// \param iterator
///     The iterator.  If initialized to zero then it will be intialized and
///     moved to the first pair in the table.
///
/// \returns True if the next pair was moved to; false if there were no more
/// pairs in the table.
bool HashTable_MoveNext(
    HashTable*          hashTable,
    HashTableIterator*  iterator
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
/// \returns True if the set was successful; false if no value exists for the
/// given key.
bool HashTable_Set(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    );

///
/// \brief Inserts a new value or sets an existing value in the hash table.
///
/// \param hashTable
///     The hash table.
/// \param key
///     The key.
/// \param value
///     The value.
///
/// \returns True if a new value was inserted; false if an existing value was
/// changed.
bool HashTable_InsertOrSet(
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
bool HashTable_Get(
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
bool HashTable_InsertOrGet(
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
///
/// \returns The hash value.
Hash HashString(
    UserData    key
    );

///
/// \brief Compares strings.
///
/// \param left
///     The left string (casted to const char*).
/// \param right
///     The right string (casted to const char*).
///
/// \returns True if the strings are equal; false otherwise.
bool CompareString(
    UserData    left,
    UserData    right
    );

///
/// \brief Returns the literal value of the key as the hash.
///
/// \param key
///     The key to hash.
/// 
/// \returns The hash value.
Hash HashIdentity(
    UserData    key
    );

///
/// \brief Compares the literal value of the keys
///
/// \param left
///     The left key.
/// \param right
///     The right key.
///
/// \returns True if the keys are equal; false otherwise.
bool CompareIdentity(
    UserData    left,
    UserData    right
    );

#endif