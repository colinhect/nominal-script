#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>

typedef unsigned long (*HashFunction)(const void* key);
typedef int (*CompareFunction)(const void* left, const void* right);

typedef struct BucketNode
{
    void* key;
    void* value;
    struct BucketNode* next;
} BucketNode;

typedef struct
{
    HashFunction hash;
    CompareFunction compare;
    BucketNode** nodes;
    size_t bucketCount;
} HashTable;

HashTable* CreateHashTable(HashFunction hash, CompareFunction compare, size_t bucketCount);
void FreeHashTable(HashTable* hashTable);

void Insert(HashTable* hashTable, void* key, void* value);
int Find(HashTable* hashTable, void* key, void ** value);

int InsertOrFind(HashTable* hashTable, void* key, void* value, void** existingValue);

unsigned long HashString(const void* key);
int CompareString(const void* left, const void* right);

#endif