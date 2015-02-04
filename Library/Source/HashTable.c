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
#include "HashTable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct HashTable
{
    HashFunction    hash;
    CompareFunction compare;
    UserData        context;
    BucketNode**    buckets;
    size_t          bucketCount;
};

struct BucketNode
{
    UserData            key;
    UserData            value;
    struct BucketNode*  next;
};

// Gets a node for a specific key with the option of creating a new node if it
// is not found
bool FindNode(
    HashTable*      hashTable,
    UserData        key,
    bool            createNew,
    BucketNode**    node
    )
{
    // Hash the key
    Hash hash = hashTable->hash(key, hashTable->context);
    hash = hash % hashTable->bucketCount;

    // Find the first node of the bucket for the hash
    BucketNode* curr = hashTable->buckets[hash];
    BucketNode* prev = NULL;

    // Iterate through each node in the bucket
    while (curr)
    {
        // Check if this is the right node
        if (hashTable->compare(curr->key, key, hashTable->context))
        {
            // Found the node
            *node = curr;

            // If we were supposed to create a new node then this is a failure
            // and if we were supposed to set an existing value then this is a
            // success
            return !createNew;
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
        assert(curr);
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

        return true;
    }

    // We were not support to create a new node and we never found one, so this
    // is a falure
    return false;
}

HashTable* HashTable_Create(
    HashFunction    hash,
    CompareFunction compare,
    UserData        context,
    size_t          bucketCount
    )
{
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    assert(hashTable);

    hashTable->hash = hash;
    hashTable->compare = compare;
    hashTable->context = context;
    hashTable->buckets = (BucketNode**)malloc(sizeof(BucketNode*) * bucketCount);
    assert(hashTable->buckets);

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
    assert(hashTable);

    // Free each bucket
    if (hashTable->buckets)
    {
        for (size_t i = 0; i < hashTable->bucketCount; ++i)
        {
            // For each node in the bucket
            BucketNode* n = hashTable->buckets[i];
            while (n)
            {
                BucketNode* t = n;
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
    
    // Free the array of buckets
    free(hashTable->buckets);

    free(hashTable);
}

bool HashTable_MoveNext(
    HashTable*          hashTable,
    HashTableIterator*  iterator
    )
{
    if (!iterator->hashTable)
    {
        // Initialize the iterator
        iterator->hashTable = hashTable;
        iterator->index = 0;
        iterator->bucketNode = hashTable->buckets[0];
    }
    else if (iterator->hashTable != hashTable)
    {
        // Iterator does not correspond to this hash table
        return false;
    }
    else if (iterator->bucketNode)
    {
        // Move to the next node if possible
        iterator->bucketNode = iterator->bucketNode->next;
    }

    // Find the next bucket node
    while (!iterator->bucketNode)
    {
        ++iterator->index;
        if (iterator->index >= hashTable->bucketCount)
        {
            return false;
        }

        iterator->bucketNode = hashTable->buckets[iterator->index];
    }

    iterator->key = iterator->bucketNode->key;
    iterator->value = iterator->bucketNode->value;

    return true;
}

bool HashTable_Insert(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    )
{
    BucketNode* node = NULL;
    if (FindNode(hashTable, key, true, &node))
    {
        node->value = value;
        return true;
    }

    return false;
}

bool HashTable_Set(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    )
{
    BucketNode* node = NULL;
    if (FindNode(hashTable, key, false, &node))
    {
        node->value = value;
        return true;
    }

    return false;
}

bool HashTable_InsertOrSet(
    HashTable*  hashTable,
    UserData    key,
    UserData    value
    )
{
    BucketNode* node = NULL;
    bool result = FindNode(hashTable, key, true, &node);
    node->value = value;
    return result;
}

bool HashTable_Get(
    HashTable*  hashTable,
    UserData    key,
    UserData*   value
    )
{
    BucketNode* node = NULL;
    if (FindNode(hashTable, key, false, &node))
    {
        *value = node->value;
        return true;
    }

    return false;
}

bool HashTable_InsertOrGet(
    HashTable*  hashTable,
    UserData    key,
    UserData    value,
    UserData*   existingValue
    )
{
    BucketNode* node = NULL;
    if (!FindNode(hashTable, key, true, &node))
    {
        *existingValue = node->value;
        return true;
    }
    node->value = value;
    return false;
}

Hash HashString(
    UserData    key,
    UserData    context
    )
{
    (void)context;

    char* s = (char*)key;
    Hash hash = 5381;

    char c;
    while ((c = *s++) != '\0')
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

bool CompareString(
    UserData    left,
    UserData    right,
    UserData    context
    )
{
    (void)context;
    return strcmp((const char*)left, (const char*)right) == 0;
}

Hash HashIdentity(
    UserData    key,
    UserData    context
    )
{
    (void)context;
    return (Hash)key;
}

bool CompareIdentity(
    UserData    left,
    UserData    right,
    UserData    context
    )
{
    (void)context;
    return left == right;
}
