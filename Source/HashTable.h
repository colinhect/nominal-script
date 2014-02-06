#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>

typedef unsigned long (*HashFunction)(const void* key);
typedef int (*CompareFunction)(const void* left, const void* right);

typedef struct _HashTable HashTable;

HashTable* HashTable_Create(HashFunction hash, CompareFunction compare, size_t bucketCount);
void HashTable_Free(HashTable* h, void (*freeKey)(void*), void (*freeValue)(void*));

void HashTable_Insert(HashTable* h, void* key, void* value);
int HashTable_Find(HashTable* h, void* key, void ** value);

int HashTable_InsertOrFind(HashTable* h, void* key, void* value, void** existingValue);

unsigned long HashString(const void* key);
int CompareString(const void* left, const void* right);

#endif