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
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->hash = hash;
    hashTable->compare = compare;
    hashTable->nodes = (BucketNode**)malloc(sizeof(BucketNode*) * bucketCount);
    hashTable->bucketCount = bucketCount;
    memset(hashTable->nodes, 0, sizeof(BucketNode*) * bucketCount);
    return hashTable;
}

void HashTable_Free(HashTable* hashTable)
{
    size_t i;
    for (i = 0; i < hashTable->bucketCount; ++i)
    {
        BucketNode* n = hashTable->nodes[i];
        while (n)
        {
            BucketNode* t = n;
            n = n->next;
            free(t);
        }
    }
}

void HashTable_Insert(HashTable* hashTable, void* key, void* value)
{
    BucketNode* prev = NULL;
    BucketNode* curr = NULL;
    unsigned long h = hashTable->hash(key);
    h = h % hashTable->bucketCount;

    curr = hashTable->nodes[h];

    while (curr)
    {
        if (hashTable->compare(curr->key, key) == 0)
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
        hashTable->nodes[h] = curr;
    }
}

int HashTable_Find(HashTable* hashTable, void* key, void** value)
{
    BucketNode* n = NULL;
    unsigned long h = hashTable->hash(key);
    h = h % hashTable->bucketCount;
    n = hashTable->nodes[h];
    while (n)
    {
        if (hashTable->compare(n->key, key) == 0)
        {
            *value = n->value;
            return 1;
        }
        n = n->next;
    }

    return 0;
}

int HashTable_InsertOrFind(HashTable* hashTable, void* key, void* value, void** existingValue)
{
    BucketNode* prev = NULL;
    BucketNode* curr = NULL;
    unsigned long h = hashTable->hash(key);
    h = h % hashTable->bucketCount;

    curr = hashTable->nodes[h];

    while (curr)
    {
        if (hashTable->compare(curr->key, key) == 0)
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
        hashTable->nodes[h] = curr;
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