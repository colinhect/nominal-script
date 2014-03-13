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
#include "catch.hpp"

extern "C"
{
#include <Nominal.h>
}

TEST_CASE("Map_CreateAndCheck")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Check(map));

    NomState_Free(state);
}

TEST_CASE("Map_InsertAndGet")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(5), NomNumber_FromInt(10)));

    NomValue result = NomMap_Get(state, map, NomNumber_FromInt(5));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(10)));

    NomState_Free(state);
}

TEST_CASE("Map_SetExisting")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(5), NomNumber_FromInt(10)));
    REQUIRE(NomMap_Set(state, map, NomNumber_FromInt(5), NomNumber_FromInt(25)));

    NomValue result = NomMap_Get(state, map, NomNumber_FromInt(5));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(25)));

    NomState_Free(state);
}

TEST_CASE("Map_TryGet")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(5), NomNumber_FromInt(10)));

    NomValue result;
    REQUIRE(NomMap_TryGet(state, map, NomNumber_FromInt(5), &result));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(10)));

    NomState_Free(state);
}

TEST_CASE("Map_TryGetNonExisting")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(5), NomNumber_FromInt(10)));

    NomValue result;
    REQUIRE(!NomMap_TryGet(state, map, NomNumber_FromInt(10), &result));

    NomState_Free(state);
}

TEST_CASE("Map_StringKeys")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", false);
    NomValue b = NomString_FromString(state, "b", false);
    NomValue c = NomString_FromString(state, "c", false);
    NomValue d = NomString_FromString(state, "d", false);
    NomValue e = NomString_FromString(state, "e", false);

    REQUIRE(NomMap_Insert(state, map, a, b));
    REQUIRE(NomMap_Insert(state, map, b, c));
    REQUIRE(NomMap_Insert(state, map, c, d));
    REQUIRE(NomMap_Insert(state, map, d, e));
    REQUIRE(NomMap_Insert(state, map, e, a));

    NomValue result;
    result = NomMap_Get(state, map, a);
    REQUIRE(NomValue_Equals(state, result, b));
    result = NomMap_Get(state, map, b);
    REQUIRE(NomValue_Equals(state, result, c));
    result = NomMap_Get(state, map, c);
    REQUIRE(NomValue_Equals(state, result, d));
    result = NomMap_Get(state, map, e);
    REQUIRE(NomValue_Equals(state, result, a));

    NomState_Free(state);
}

TEST_CASE("Map_PooledStringKeys")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);
    NomValue c = NomString_FromString(state, "c", true);
    NomValue d = NomString_FromString(state, "d", true);
    NomValue e = NomString_FromString(state, "e", true);

    REQUIRE(NomMap_Insert(state, map, a, b));
    REQUIRE(NomMap_Insert(state, map, b, c));
    REQUIRE(NomMap_Insert(state, map, c, d));
    REQUIRE(NomMap_Insert(state, map, d, e));
    REQUIRE(NomMap_Insert(state, map, e, a));

    NomValue result;
    result = NomMap_Get(state, map, a);
    REQUIRE(NomValue_Equals(state, result, b));
    result = NomMap_Get(state, map, b);
    REQUIRE(NomValue_Equals(state, result, c));
    result = NomMap_Get(state, map, c);
    REQUIRE(NomValue_Equals(state, result, d));
    result = NomMap_Get(state, map, e);
    REQUIRE(NomValue_Equals(state, result, a));

    NomState_Free(state);
}

TEST_CASE("Map_MixedStringKeys")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);

    REQUIRE(NomMap_Insert(state, map, a, b));

    NomValue a2 = NomString_FromString(state, "a", false);

    NomValue result;
    result = NomMap_Get(state, map, a2);
    REQUIRE(NomValue_Equals(state, result, b));

    NomState_Free(state);
}

TEST_CASE("Map_Iterate")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(0), NomNumber_FromInt(0)));
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(1), NomNumber_FromInt(2)));
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(2), NomNumber_FromInt(4)));
    REQUIRE(NomMap_Insert(state, map, NomNumber_FromInt(3), NomNumber_FromInt(6)));

    NomMapIterator iterator = { 0 };
    REQUIRE(NomMap_MoveNext(state, map, &iterator));
    REQUIRE(NomValue_Equals(state, iterator.key, NomNumber_FromInt(0)));
    REQUIRE(NomValue_Equals(state, iterator.value, NomNumber_FromInt(0)));
    REQUIRE(NomMap_MoveNext(state, map, &iterator));
    REQUIRE(NomValue_Equals(state, iterator.key, NomNumber_FromInt(1)));
    REQUIRE(NomValue_Equals(state, iterator.value, NomNumber_FromInt(2)));
    REQUIRE(NomMap_MoveNext(state, map, &iterator));
    REQUIRE(NomValue_Equals(state, iterator.key, NomNumber_FromInt(2)));
    REQUIRE(NomValue_Equals(state, iterator.value, NomNumber_FromInt(4)));
    REQUIRE(NomMap_MoveNext(state, map, &iterator));
    REQUIRE(NomValue_Equals(state, iterator.key, NomNumber_FromInt(3)));
    REQUIRE(NomValue_Equals(state, iterator.value, NomNumber_FromInt(6)));
    REQUIRE(!NomMap_MoveNext(state, map, &iterator));

    NomState_Free(state);
}

