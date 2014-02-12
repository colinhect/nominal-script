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
#ifndef MAPTESTS_H
#define MAPTESTS_H

#include "Basic.h"

#include <Nominal.h>

void Test_Map_CreateAndCheck(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CU_ASSERT(NomMap_Check(map));

    NomState_Free(state);
}

void Test_Map_SetAndGet(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CU_ASSERT(NomMap_Set(state, map, NomInteger_FromInt(5), NomInteger_FromInt(10)));

    NomValue result = NomMap_Get(state, map, NomInteger_FromInt(5));
    CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(10)));

    NomState_Free(state);
}

void Test_Map_SetExisting(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CU_ASSERT(NomMap_Set(state, map, NomInteger_FromInt(5), NomInteger_FromInt(10)));
    CU_ASSERT(!NomMap_Set(state, map, NomInteger_FromInt(5), NomInteger_FromInt(25)));

    NomValue result = NomMap_Get(state, map, NomInteger_FromInt(5));
    CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(25)));

    NomState_Free(state);
}

void Test_Map_TryGet(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CU_ASSERT(NomMap_Set(state, map, NomInteger_FromInt(5), NomInteger_FromInt(10)));

    NomValue result;
    CU_ASSERT(NomMap_TryGet(state, map, NomInteger_FromInt(5), &result));
    CU_ASSERT(NomValue_Equals(state, result, NomInteger_FromInt(10)));

    NomState_Free(state);
}

void Test_Map_TryGetNonExisting(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CU_ASSERT(NomMap_Set(state, map, NomInteger_FromInt(5), NomInteger_FromInt(10)));

    NomValue result;
    CU_ASSERT(!NomMap_TryGet(state, map, NomInteger_FromInt(10), &result));

    NomState_Free(state);
}

void Test_Map_StringKeys(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", false);
    NomValue b = NomString_FromString(state, "b", false);
    NomValue c = NomString_FromString(state, "c", false);
    NomValue d = NomString_FromString(state, "d", false);
    NomValue e = NomString_FromString(state, "e", false);

    CU_ASSERT(NomMap_Set(state, map, a, b));
    CU_ASSERT(NomMap_Set(state, map, b, c));
    CU_ASSERT(NomMap_Set(state, map, c, d));
    CU_ASSERT(NomMap_Set(state, map, d, e));
    CU_ASSERT(NomMap_Set(state, map, e, a));

    NomValue result;
    result = NomMap_Get(state, map, a);
    CU_ASSERT(NomValue_Equals(state, result, b));
    result = NomMap_Get(state, map, b);
    CU_ASSERT(NomValue_Equals(state, result, c));
    result = NomMap_Get(state, map, c);
    CU_ASSERT(NomValue_Equals(state, result, d));
    result = NomMap_Get(state, map, e);
    CU_ASSERT(NomValue_Equals(state, result, a));

    NomState_Free(state);
}

void Test_Map_PooledStringKeys(void)
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);
    NomValue c = NomString_FromString(state, "c", true);
    NomValue d = NomString_FromString(state, "d", true);
    NomValue e = NomString_FromString(state, "e", true);

    CU_ASSERT(NomMap_Set(state, map, a, b));
    CU_ASSERT(NomMap_Set(state, map, b, c));
    CU_ASSERT(NomMap_Set(state, map, c, d));
    CU_ASSERT(NomMap_Set(state, map, d, e));
    CU_ASSERT(NomMap_Set(state, map, e, a));

    NomValue result;
    result = NomMap_Get(state, map, a);
    CU_ASSERT(NomValue_Equals(state, result, b));
    result = NomMap_Get(state, map, b);
    CU_ASSERT(NomValue_Equals(state, result, c));
    result = NomMap_Get(state, map, c);
    CU_ASSERT(NomValue_Equals(state, result, d));
    result = NomMap_Get(state, map, e);
    CU_ASSERT(NomValue_Equals(state, result, a));

    NomState_Free(state);
}

#endif