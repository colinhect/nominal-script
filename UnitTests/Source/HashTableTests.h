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
#ifndef HASHTABLETESTS_H
#define HASHTABLETESTS_H

#include <HashTable.h>

void Test_HashTable_InsertAndFind(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(HashTable_Insert(hashTable, (UserData)key, (UserData)value));
    CU_ASSERT(HashTable_Find(hashTable, (UserData)key, (UserData*)&result));
    CU_ASSERT_EQUAL(result, value);

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_InsertExisting(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(HashTable_Insert(hashTable, (UserData)key, (UserData)value));
    CU_ASSERT(!HashTable_Insert(hashTable, (UserData)key, (UserData)value));

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_FindNonExisting(void)
{
    HashTable*  hashTable;
    UserData    result;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(!HashTable_Find(hashTable, (UserData)"DoesNotExist", (UserData*)&result));

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_Set(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* newValue = "NewValue";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(HashTable_Insert(hashTable, (UserData)key, (UserData)value));
    CU_ASSERT(HashTable_Set(hashTable, (UserData)key, (UserData)newValue));
    CU_ASSERT(HashTable_Find(hashTable, (UserData)key, (UserData*)&result));
    CU_ASSERT_EQUAL(result, newValue);

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_SetNonExisting(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(!HashTable_Set(hashTable, (UserData)key, (UserData)value));

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_InsertOrFindExisting(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(HashTable_Insert(hashTable, (UserData)key, (UserData)value));
    CU_ASSERT(HashTable_InsertOrFind(hashTable, (UserData)key, (UserData)value, (UserData*)&result));
    CU_ASSERT_EQUAL(result, value);

    HashTable_Free(hashTable, NULL, NULL);
}

void Test_HashTable_InsertOrFindNonExisting(void)
{
    HashTable*  hashTable;
    const char* key = "Key";
    const char* value = "Value";
    const char* result = NULL;

    hashTable = HashTable_Create(HashString, CompareString, 10);

    CU_ASSERT(!HashTable_InsertOrFind(hashTable, (UserData)key, (UserData)value, (UserData*)&result));
    CU_ASSERT(HashTable_Find(hashTable, (UserData)key, (UserData*)&result));
    CU_ASSERT_EQUAL(result, value);

    HashTable_Free(hashTable, NULL, NULL);
}

#endif