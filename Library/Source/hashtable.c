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
#include "hashtable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Gets a node for a specific key with the option of creating a new node if it
// is not found
static bool findnode(
    HashTable*      hashtable,
    UserData        key,
    bool            createNew,
    BucketNode**    node
);

HashTable* hashtable_new(
    HashFunction    hash,
    CompareFunction compare,
    UserData        context,
    size_t          bucketcount
)
{
    assert(hash);
    assert(compare);

    HashTable* hashtable = (HashTable*)malloc(sizeof(HashTable));
    assert(hashtable);

    hashtable->hash = hash;
    hashtable->compare = compare;
    hashtable->context = context;
    hashtable->buckets = (BucketNode**)malloc(sizeof(BucketNode*) * bucketcount);
    assert(hashtable->buckets);

    memset(hashtable->buckets, 0, sizeof(BucketNode*) * bucketcount);
    hashtable->bucketcount = bucketcount;

    return hashtable;
}

void hashtable_free(
    HashTable*  hashtable,
    void        (*freekey)(void*),
    void        (*freevalue)(void*)
)
{
    assert(hashtable);

    // Free each bucket
    if (hashtable->buckets)
    {
        for (size_t i = 0; i < hashtable->bucketcount; ++i)
        {
            // For each node in the bucket
            BucketNode* n = hashtable->buckets[i];
            while (n)
            {
                BucketNode* t = n;
                n = n->next;

                // Free the key if needed
                if (freekey && t->key)
                {
                    freekey((void*)t->key);
                }

                // Free the value if needed
                if (freevalue && t->value)
                {
                    freevalue((void*)t->value);
                }

                free(t);
            }
        }
    }

    // Free the array of buckets
    free(hashtable->buckets);

    free(hashtable);
}

bool hashtable_next(
    HashTable*          hashtable,
    HashTableIterator*  iterator
)
{
    assert(hashtable);

    if (!iterator->hashtable)
    {
        // Initialize the iterator
        iterator->hashtable = hashtable;
        iterator->index = 0;
        iterator->bucketnode = hashtable->buckets[0];
    }
    else if (iterator->hashtable != hashtable)
    {
        // Iterator does not correspond to this hash table
        return false;
    }
    else if (iterator->bucketnode)
    {
        // Move to the next node if possible
        iterator->bucketnode = iterator->bucketnode->next;
    }

    // Find the next bucket node
    while (!iterator->bucketnode)
    {
        ++iterator->index;
        if (iterator->index >= hashtable->bucketcount)
        {
            return false;
        }

        iterator->bucketnode = hashtable->buckets[iterator->index];
    }

    iterator->key = iterator->bucketnode->key;
    iterator->value = iterator->bucketnode->value;

    return true;
}

bool hashtable_insert(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
)
{
    assert(hashtable);

    BucketNode* node = NULL;
    bool result = findnode(hashtable, key, true, &node);
    if (result)
    {
        node->value = value;
    }

    return result;
}

bool hashtable_set(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
)
{
    assert(hashtable);

    BucketNode* node = NULL;
    bool result = findnode(hashtable, key, false, &node);
    if (result)
    {
        node->value = value;
    }

    return result;
}

bool hashtable_insertorset(
    HashTable*  hashtable,
    UserData    key,
    UserData    value
)
{
    assert(hashtable);

    BucketNode* node = NULL;
    bool result = findnode(hashtable, key, true, &node);

    node->value = value;
    return result;
}

bool hashtable_get(
    HashTable*  hashtable,
    UserData    key,
    UserData*   value
)
{
    BucketNode* node = NULL;
    bool result = findnode(hashtable, key, false, &node);
    if (result)
    {
        *value = node->value;
    }

    return result;
}

bool hashtable_insertorget(
    HashTable*  hashtable,
    UserData    key,
    UserData    value,
    UserData*   existingvalue
)
{
    BucketNode* node = NULL;

    bool result = !findnode(hashtable, key, true, &node);
    if (result)
    {
        *existingvalue = node->value;
    }
    else
    {
        node->value = value;
    }

    return result;
}

Hash hashstring(
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

bool comparestring(
    UserData    left,
    UserData    right,
    UserData    context
)
{
    (void)context;
    return strcmp((const char*)left, (const char*)right) == 0;
}

Hash hashidentity(
    UserData    key,
    UserData    context
)
{
    (void)context;
    return (Hash)key;
}

bool compareidentity(
    UserData    left,
    UserData    right,
    UserData    context
)
{
    (void)context;
    return left == right;
}

static bool findnode(
    HashTable*      hashtable,
    UserData        key,
    bool            createNew,
    BucketNode**    node
)
{
    // Hash the key
    Hash hash = hashtable->hash(key, hashtable->context);
    hash = hash % hashtable->bucketcount;

    // Find the first node of the bucket for the hash
    BucketNode* curr = hashtable->buckets[hash];
    BucketNode* prev = NULL;

    // Iterate through each node in the bucket
    while (curr)
    {
        // Check if this is the right node
        if (hashtable->compare(curr->key, key, hashtable->context))
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
            hashtable->buckets[hash] = curr;
        }

        // Return the value
        *node = curr;

        return true;
    }

    // We were not support to create a new node and we never found one, so this
    // is a falure
    return false;
}
