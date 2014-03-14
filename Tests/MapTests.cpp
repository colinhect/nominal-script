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

TEST_CASE("A Nominal map can be created", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    REQUIRE(NomMap_Check(map) == true);

    NomState_Free(state);
}

TEST_CASE("Calling NomMap_Check on non-map Nominal values", "[Map]")
{
    NomState* state = NomState_Create();

    SECTION("Checking nil")
    {
        NomValue value = NOM_NIL;
        REQUIRE(NomMap_Check(value) == false);
    }

    SECTION("Checking number")
    {
        NomValue value = NomNumber_FromDouble(1.0);
        REQUIRE(NomMap_Check(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = NomString_FromString(state, "Testing...", false);
        REQUIRE(NomMap_Check(value) == false);
    }

    SECTION("Checking pooled string")
    {
        NomValue value = NomString_FromString(state, "Testing...", true);
        REQUIRE(NomMap_Check(value) == false);
    }

    NomState_Free(state);
}

TEST_CASE("Inserting and retrieving values to/from a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();
    
    NomValue map = NomMap_Create(state);

    SECTION("Using number keys")
    {
        NomValue key = NomNumber_FromInt(5);
        NomValue value = NomNumber_FromInt(10);
        REQUIRE(NomMap_Insert(state, map, key, value) == true);
        NomValue result = NomMap_Get(state, map, key);
        REQUIRE(NomValue_Equals(state, result, value) == true);
    }

    SECTION("Using string keys")
    {
        NomValue key = NomString_FromString(state, "Key", false);
        NomValue value = NomNumber_FromInt(10);
        REQUIRE(NomMap_Insert(state, map, key, value) == true);
        NomValue result = NomMap_Get(state, map, key);
        REQUIRE(NomValue_Equals(state, result, value) == true);
    }

    SECTION("Using pooled string keys")
    {
        NomValue key = NomString_FromString(state, "Key", true);
        NomValue value = NomNumber_FromInt(10);
        REQUIRE(NomMap_Insert(state, map, key, value) == true);
        NomValue result = NomMap_Get(state, map, key);
        REQUIRE(NomValue_Equals(state, result, value) == true);
    }

    NomState_Free(state);
}

TEST_CASE("Setting a value in a Nominal map after it is inserted", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue key = NomNumber_FromInt(5);
    NomValue value = NomNumber_FromInt(10);
    NomValue newValue = NomNumber_FromInt(25);
    REQUIRE(NomMap_Insert(state, map, key, value) == true);
    REQUIRE(NomMap_Set(state, map, key, newValue) == true);

    NomValue result = NomMap_Get(state, map, key);
    REQUIRE(NomValue_Equals(state, result, newValue) == true);

    NomState_Free(state);
}

TEST_CASE("Calling Map_TryGet for a key that exists", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue key = NomNumber_FromInt(5);
    NomValue value = NomNumber_FromInt(10);
    REQUIRE(NomMap_Insert(state, map, key, value) == true);

    NomValue result;
    REQUIRE(NomMap_TryGet(state, map, key, &result) == true);
    REQUIRE(NomValue_Equals(state, result, value) == true);

    NomState_Free(state);
}

TEST_CASE("Calling Map_TryGet for a key that does not exist", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue key = NomNumber_FromInt(5);
    NomValue value = NomNumber_FromInt(10);
    REQUIRE(NomMap_Insert(state, map, key, value) == true);

    NomValue result;
    REQUIRE(NomMap_TryGet(state, map, value, &result) == false);

    NomState_Free(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from strings in a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", false);
    NomValue b = NomString_FromString(state, "b", false);
    NomValue c = NomString_FromString(state, "c", false);
    NomValue d = NomString_FromString(state, "d", false);
    NomValue e = NomString_FromString(state, "e", false);

    REQUIRE(NomMap_Insert(state, map, a, b) == true);
    REQUIRE(NomMap_Insert(state, map, b, c) == true);
    REQUIRE(NomMap_Insert(state, map, c, d) == true);
    REQUIRE(NomMap_Insert(state, map, d, e) == true);
    REQUIRE(NomMap_Insert(state, map, e, a) == true);

    NomValue result;
    result = NomMap_Get(state, map, a);
    REQUIRE(NomValue_Equals(state, result, b) == true);
    result = NomMap_Get(state, map, b);
    REQUIRE(NomValue_Equals(state, result, c) == true);
    result = NomMap_Get(state, map, c);
    REQUIRE(NomValue_Equals(state, result, d) == true);
    result = NomMap_Get(state, map, e);
    REQUIRE(NomValue_Equals(state, result, a) == true);

    NomState_Free(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from pooled strings in a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);
    NomValue c = NomString_FromString(state, "c", true);
    NomValue d = NomString_FromString(state, "d", true);
    NomValue e = NomString_FromString(state, "e", true);

    REQUIRE(NomMap_Insert(state, map, a, b) == true);
    REQUIRE(NomMap_Insert(state, map, b, c) == true);
    REQUIRE(NomMap_Insert(state, map, c, d) == true);
    REQUIRE(NomMap_Insert(state, map, d, e) == true);
    REQUIRE(NomMap_Insert(state, map, e, a) == true);

    NomValue result;
    result = NomMap_Get(state, map, a);
    REQUIRE(NomValue_Equals(state, result, b) == true);
    result = NomMap_Get(state, map, b);
    REQUIRE(NomValue_Equals(state, result, c) == true);
    result = NomMap_Get(state, map, c);
    REQUIRE(NomValue_Equals(state, result, d) == true);
    result = NomMap_Get(state, map, e);
    REQUIRE(NomValue_Equals(state, result, a) == true);

    NomState_Free(state);
}

TEST_CASE("Inserting values keyed from mixed pooled/non-pooled strings into a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);

    REQUIRE(NomMap_Insert(state, map, a, b) == true);

    NomValue a2 = NomString_FromString(state, "a", false);

    NomValue result;
    result = NomMap_Get(state, map, a2);
    REQUIRE(NomValue_Equals(state, result, b) == true);

    NomState_Free(state);
}

TEST_CASE("Iterating over a map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomNumber_FromInt(0);
    NomValue b = NomNumber_FromInt(1);
    NomValue c = NomNumber_FromInt(2);
    NomValue d = NomNumber_FromInt(3);
    NomValue e = NomNumber_FromInt(4);
    NomValue f = NomNumber_FromInt(5);
    NomValue g = NomNumber_FromInt(6);

    REQUIRE(NomMap_Insert(state, map, a, a) == true);
    REQUIRE(NomMap_Insert(state, map, b, c) == true);
    REQUIRE(NomMap_Insert(state, map, c, e) == true);
    REQUIRE(NomMap_Insert(state, map, d, g) == true);

    NomMapIterator iterator = { 0 };
    REQUIRE(NomMap_MoveNext(state, map, &iterator) == true);
    REQUIRE(NomValue_Equals(state, iterator.key, a) == true);
    REQUIRE(NomValue_Equals(state, iterator.value, a) == true);
    REQUIRE(NomMap_MoveNext(state, map, &iterator) == true);
    REQUIRE(NomValue_Equals(state, iterator.key, b) == true);
    REQUIRE(NomValue_Equals(state, iterator.value, c) == true);
    REQUIRE(NomMap_MoveNext(state, map, &iterator) == true);
    REQUIRE(NomValue_Equals(state, iterator.key, c) == true);
    REQUIRE(NomValue_Equals(state, iterator.value, e) == true);
    REQUIRE(NomMap_MoveNext(state, map, &iterator) == true);
    REQUIRE(NomValue_Equals(state, iterator.key, d) == true);
    REQUIRE(NomValue_Equals(state, iterator.value, g) == true);
    REQUIRE(NomMap_MoveNext(state, map, &iterator) == false);

    NomState_Free(state);
}

