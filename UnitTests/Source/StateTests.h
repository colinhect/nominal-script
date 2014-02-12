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
#ifndef STATETESTS_H
#define STATETESTS_H

#include "Basic.h"

#include <Nominal.h>

#define TEST_EXPR(expr, expected)\
    {\
        NomState* state = NomState_Create(); \
        CU_ASSERT(state != NULL);\
        NomValue value = NomState_Execute(state, expr);\
        CU_ASSERT(!NomState_ErrorOccurred(state));\
        CU_ASSERT(NomValue_Equals(state, value, expected));\
        NomState_Free(state);\
    }

void Test_State_IntegerArithmetic(void)
{
    TEST_EXPR("2 + 3", NomInteger_FromInt(5));
    TEST_EXPR("2 - 3", NomInteger_FromInt(-1));
    TEST_EXPR("2 * 3", NomInteger_FromInt(6));
    TEST_EXPR("6 / 3", NomInteger_FromInt(2));
    TEST_EXPR("6 / 4", NomInteger_FromInt(1));
}

void Test_State_RealArithmetic(void)
{
    TEST_EXPR("2 + 3.0", NomReal_FromDouble(5.0));
    TEST_EXPR("2.0 + 3.0", NomReal_FromDouble(5.0));
    TEST_EXPR("2.0 + 3", NomReal_FromDouble(5.0));
    TEST_EXPR("2 - 3.0", NomReal_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3", NomReal_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3.0", NomReal_FromDouble(-1.0));
    TEST_EXPR("2 * 3.0", NomReal_FromDouble(6.0));
    TEST_EXPR("2.0 * 3", NomReal_FromDouble(6.0));
    TEST_EXPR("2 * 3.0", NomReal_FromDouble(6.0));
    TEST_EXPR("6 / 3.0", NomReal_FromDouble(2.0));
    TEST_EXPR("6.0 / 3", NomReal_FromDouble(2.0));
    TEST_EXPR("6 / 3.0", NomReal_FromDouble(2.0));
    TEST_EXPR("6.0 / 4.0", NomReal_FromDouble(6.0 / 4.0));
    TEST_EXPR("6.53 / 4.23", NomReal_FromDouble(6.53 / 4.23));
}

void Test_State_GlobalVariables(void)
{
    TEST_EXPR("a := 1, b := 2, a + b", NomInteger_FromInt(3));
}

void Test_State_MapWithImplicitKeys(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomState_Execute(state, "{ 0, 1, 2, 3 }");
    CU_ASSERT(!NomState_ErrorOccurred(state));
    CU_ASSERT(NomMap_Check(map));

    NomValue result;

    for (int i = 0; i < 4; ++i)
    {
        result = NomMap_Get(state, map, NomInteger_FromInt(i));
        CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(i)));
    }

    NomState_Free(state);
}

void Test_State_MapWithExplicitKeys(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomState_Execute(state, "{ \"zero\" -> 0, \"one\" -> 1 }");
    CU_ASSERT(!NomState_ErrorOccurred(state));
    CU_ASSERT(NomMap_Check(map));

    NomValue result;
    result = NomMap_Get(state, map, NomString_FromString(state, "zero", false));
    CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(0)));
    result = NomMap_Get(state, map, NomString_FromString(state, "one", false));
    CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(1)));

    NomState_Free(state);
}

#endif