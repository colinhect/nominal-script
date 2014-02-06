#include "HashTable.h"

#include <stdlib.h>
#include <string.h>

typedef struct _BucketNode
{
    void* key;
    void* value;
    struct _BucketNode* next;
} BucketNode;

typedef struct _HashTable
{
    HashFunction hash;
    CompareFunction compare;
    BucketNode** nodes;
    size_t bucketCount;
} HashTable;

HashTable* HashTable_Create(HashFunction hash, CompareFunction compare, size_t bucketCount)
{
    HashTable* h = (HashTable*)malloc(sizeof(HashTable));
    h->hash = hash;
    h->compare = compare;
    h->nodes = (BucketNode**)malloc(sizeof(BucketNode*) * bucketCount);
    memset(h->nodes, 0, sizeof(BucketNode*) * bucketCount);
    h->bucketCount = bucketCount;
    return h;
}

void HashTable_Free(HashTable* h, void (*freeKey)(void*), void (*freeValue)(void*))
{
    size_t i;
    for (i = 0; i < h->bucketCount; ++i)
    {
        BucketNode* n = h->nodes[i];
        while (n)
        {
            BucketNode* t = n;
            n = n->next;

            // Free the key if needed
            if (freeKey && t->key)
            {
                freeKey(t->key);
            }

            // Free the value if needed
            if (freeValue && t->value)
            {
                freeValue(t->value);
            }

            free(t);
        }
    }
}

void HashTable_Insert(HashTable* h, void* key, void* value)
{
    BucketNode* prev = NULL;
    BucketNode* curr = NULL;
    unsigned long hash = h->hash(key);
    hash = hash % h->bucketCount;

    curr = h->nodes[hash];

    while (curr)
    {
        if (h->compare(curr->key, key) == 0)
        {
            curr->value = value;
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    curr = (BucketNode*)malloc(sizeof(BucketNode));
    curr->key = key;
    curr->value = value;
    curr->next = NULL;

    if (prev)
    {
        prev->next = curr;
    }
    else
    {
        h->nodes[hash] = curr;
    }
}

int HashTable_Find(HashTable* h, void* key, void** value)
{
    BucketNode* n = NULL;
    unsigned long hash = h->hash(key);
    hash = hash % h->bucketCount;
    n = h->nodes[hash];
    while (n)
    {
        if (h->compare(n->key, key) == 0)
        {
            *value = n->value;
            return 1;
        }
        n = n->next;
    }

    return 0;
}

int HashTable_InsertOrFind(HashTable* h, void* key, void* value, void** existingValue)
{
    BucketNode* prev = NULL;
    BucketNode* curr = NULL;
    unsigned long hash = h->hash(key);
    hash = hash % h->bucketCount;

    curr = h->nodes[hash];

    while (curr)
    {
        if (h->compare(curr->key, key) == 0)
        {
            *existingValue = curr->value;
            return 1;
        }

        prev = curr;
        curr = curr->next;
    }

    curr = (BucketNode*)malloc(sizeof(BucketNode));
    curr->key = key;
    curr->value = value;
    curr->next = NULL;

    if (prev)
    {
        prev->next = curr;
    }
    else
    {
        h->nodes[hash] = curr;
    }

    return 0;
}

unsigned long HashString(const void* key)
{
    char* s = (char*)key;
    unsigned long hash = 5381;
    int c;

    while (c = *s++)
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int CompareString(const void* left, const void* right)
{
    return strcmp((const char*)left, (const char*)right);
}