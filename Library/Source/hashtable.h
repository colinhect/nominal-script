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
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// The data type used for keys/values/contexts in a hash table
typedef uint64_t UserData;

// The result of a hash
typedef uint64_t Hash;

// A hash function
typedef Hash (*HashFunction)(
    UserData    key,
    UserData    context
    );

// A comparison function
typedef bool (*CompareFunction)(
    UserData    left,
    UserData    right,
    UserData    context
    );

// A hash table
typedef struct HashTable HashTable;

// A hash table bucket node
typedef struct BucketNode BucketNode;

// An iterator of a hash table
typedef struct
{
    HashTable*  hashtable;
    size_t      index;
    UserData    key;
    UserData    value;
    BucketNode* bucketnode;
} HashTableIterator;

// Creates a new hash table given the hash/compare functions and the context
// used for those functions
HashTable* hashtable_new(
    HashFunction    hash,
    CompareFunction compare,
    UserData        context,
    size_t          bucketcount
    );

// Frees a hash table using the specified functions to free each key and value
// (both functions can be NULL)
void hashtable_free(
    HashTable*  hashtable,
    void        (*freekey)(void*),
    void        (*freevalue)(void*)
    );

// Moves to the next pair in the hash table
bool hashtable_next(
    HashTable*          hashtable,
    HashTableIterator*  iterator
    );

// Inserts a new value in the hash table, returning true if the insertion was
// successful or false if a value already exists for the specified key
bool hashtable_insert(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
    );

// Sets an existing value in the hash table, returning true if the set was
// successful or false if no value exists for the specified key
bool hashtable_set(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
    );

// Inserts a new value or sets an existing value in the hash table, returning
// true if a new value was inserted or false if an existing value was changed
bool HashTable_InsertOrSet(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
    );

// Gets the value for a key, returning true if a value was found for the
// specified key and false if no value was found for the specified key
bool hashtable_get(
    HashTable*  hashtable,
    UserData    key,
    UserData*   value
    );

// Either inserts a new value or get an existing value, returning true if a
// value was found for the given key or false if no value was found for the
// specified and a new value was inserted
bool hashtable_insertorget(
    HashTable*  hashtable,
    UserData    key,
    UserData    value,
    UserData*   existingvalue
    );

// Hashes a string
Hash hashstring(
    UserData    key,
    UserData    context
    );

// Compares strings, returning true if the strings are equal or false otherwise
bool comparestring(
    UserData    left,
    UserData    right,
    UserData    context
    );

// Returns the literal value of the key as the hash
Hash hashidentity(
    UserData    key,
    UserData    context
    );

// Compares the literal value of the key, returning true if the keys are equal
// or false otherwise
bool compareidentity(
    UserData    left,
    UserData    right,
    UserData    context
    );

#endif
