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
#include "Basic.h"

#include "HashTableTests.h"
#include "MapTests.h"
#include "NumberTests.h"
#include "StateTests.h"

#define BEGIN_TEST_SUITE(name)  suite = CU_add_suite("HashTable", NULL, NULL)
#define ADD_TEST(test)          CU_add_test(suite, #test, test)

int main()
{
    CU_pSuite suite = NULL;

    // Initialize CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    // HashTable test suite
    BEGIN_TEST_SUITE("HashTable");
    ADD_TEST(Test_HashTable_InsertAndFind);
    ADD_TEST(Test_HashTable_InsertExisting);
    ADD_TEST(Test_HashTable_GetNonExisting);
    ADD_TEST(Test_HashTable_Set);
    ADD_TEST(Test_HashTable_SetNonExisting);
    ADD_TEST(Test_HashTable_InsertOrGetExisting);
    ADD_TEST(Test_HashTable_InsertOrGetNonExisting);
    ADD_TEST(Test_HashTable_Iterate);

    // Map test suite
    BEGIN_TEST_SUITE("Map");
    ADD_TEST(Test_Map_CreateAndCheck);
    ADD_TEST(Test_Map_InsertAndGet);
    ADD_TEST(Test_Map_SetExisting);
    ADD_TEST(Test_Map_TryGet);
    ADD_TEST(Test_Map_TryGetNonExisting);
    ADD_TEST(Test_Map_StringKeys);
    ADD_TEST(Test_Map_PooledStringKeys);
    ADD_TEST(Test_Map_MixedStringKeys);
    ADD_TEST(Test_Map_Iterate);

    // Number test suite
    BEGIN_TEST_SUITE("Number");
    ADD_TEST(Test_Number_CheckPositive);
    ADD_TEST(Test_Number_CheckNegative);
    ADD_TEST(Test_Number_ToAndFromDouble);

    // State test suite
    BEGIN_TEST_SUITE("State");
    ADD_TEST(Test_State_Arithmetic);
    ADD_TEST(Test_State_GlobalVariables);
    ADD_TEST(Test_State_MapWithImplicitKeys);
    ADD_TEST(Test_State_MapWithExplicitKeys);
    ADD_TEST(Test_State_Indexing);
    
    // Run the tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Clean up and exit
    CU_cleanup_registry();
    return CU_get_error();
}