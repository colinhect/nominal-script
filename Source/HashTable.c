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
#include "HashTable.h"

#include <stdlib.h>
#include <string.h>

typedef struct _BucketNode
{
    UserData            key;
    UserData            value;
    struct _BucketNode* next;
} BucketNode;

typedef struct _HashTable
{
    HashFunction    hash;
    CompareFunction compare;
    BucketNode**    buckets;
    size_t          bucketCount;
} HashTable;

// Gets a node for a specific key with the option of creating a new node if it
// is not found
int HashTable_FindNode(
    HashTable*      hashTable,
    UserData        key,
    int             createNew,
    BucketNode**    node
    )
{
    BucketNode* prev = NULL;
    BucketNode* curr = NULL;
    Hash        hash;

    // Hash the key
    hash = hashTable->hash(key);
    hash = hash % hashTable->bucketCount;

    // Find the first node of the bucket for the hash
    curr = hashTable->buckets[hash];

    // Iterate through each node in the bucket
    while (curr)
    {
        // Check if this is the right node
        if (hashTable->compare(curr->key, key) == 0)
        {
            // Found the node
            *node = curr;

            // If we were supposed to create a new node then this is a failure
            if (createNew)
            {
                return 0;
            }

            // If we were supposed to set an existing value then this is a
            // success
            else
            {
                return 1;
            }
        }

        // Move to the next node
        prev = curr;
        curr = curr->next;
    }

    // We didn't find an existing node, if we were supposed to create one then
    // create it
    if (createNew)
    {
        // Create the node
        curr = (BucketNode*)malloc(sizeof(BucketNode));
        curr->key = key;
        curr->next = NULL;

        if (prev)
        {
            // Link to the previous node
            prev->next = curr;
        }
        else
        {
            // Set as first node in a new bucket
            hashTable->buckets[hash] = curr;
        }

        // Return the value
        *node = curr;

        return 1;
    }

    // We were not support to create a new node and we never found one, so this
    // is a falure
    return 0;
}

HashTable* HashTable_Create(
    HashFunction    hash,
    CompareFunction compare,
    size_t          bucketCount
    )
{
    HashTable* hashTable;

    hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->hash = hash;
    hashTable->compare = compare;
    hashTable->buckets = (BucketNode**)malloc(sizeof(BucketNode*) * bucketCount);
    memset(hashTable->buckets, 0, sizeof(BucketNode*) * bucketCount);
    hashTable->bucketCount = bucketCount;

    return hashTable;
}

void HashTable_Free(
    HashTable*  hashTable,
    void        (*freeKey)(void*),
    void        (*freeValue)(void*)
    )
{
    size_t i;

    // For each bucket
    for (i = 0; i < hashTable->bucketCount; ++i)
    {
        BucketNode* n;

        // For each node in the bucket
        n = hashTable->buckets[i];
        while (n)
        {
            BucketNode* t;
            t = n;
            n = n->next;

            // Free the key if needed
            if (freeKey && t->key)
            {
                freeKey((void*)t->key);
            }

            // Free the value if needed
            if (freeValue && t->value)
            {
                freeValue((void*)t->value);
            }

            free(t);
        }
    }
}

int HashTable_Insert(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    )
{
    BucketNode* node = NULL;

    if (HashTable_FindNode(hashTable, key, 1, &node))
    {
        node->value = value;
        return 1;
    }

    return 0;
}

int HashTable_Set(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    )
{
    BucketNode* node = NULL;

    if (HashTable_FindNode(hashTable, key, 0, &node))
    {
        node->value = value;
        return 1;
    }

    return 0;
}

bool HashTable_Find(
    HashTable*  hashTable,
    UserData    key,
    UserData*   value
    )
{
    BucketNode* node = NULL;

    if (HashTable_FindNode(hashTable, key, 0, &node))
    {
        *value = node->value;
        return true;
    }

    return false;
}

int HashTable_InsertOrFind(
    HashTable*  hashTable,
    UserData    key,
    UserData    value,
    UserData*   existingValue
    )
{
    BucketNode* node = NULL;

    if (!HashTable_FindNode(hashTable, key, 1, &node))
    {
        *existingValue = node->value;
        return 1;
    }
    node->value = value;
    return 0;
}

Hash HashString(
    UserData    key
    )
{
    char* s = (char*)key;
    Hash hash = 5381;
    int c;

    while (c = *s++)
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int CompareString(
    UserData    left,
    UserData    right
    )
{
    return strcmp((const char*)left, (const char*)right);
}