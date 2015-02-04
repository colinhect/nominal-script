///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2015 Colin Hill
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

TEST_CASE("Creating and identifying an empty Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CHECK(NomMap_Check(map) == true);

    NomState_Free(state);
}

TEST_CASE("Calling NomMap_Check() on non-map Nominal values", "[Map]")
{
    NomState* state = NomState_Create();

    SECTION("Checking nil")
    {
        NomValue value = NomValue_Nil();
        CHECK(NomMap_Check(value) == false);
    }

    SECTION("Checking number")
    {
        NomValue value = NomNumber_FromDouble(1.0);
        CHECK(NomMap_Check(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = NomString_FromString(state, "Testing...", false);
        CHECK(NomMap_Check(value) == false);
    }

    SECTION("Checking interned string")
    {
        NomValue value = NomString_FromString(state, "Testing...", true);
        CHECK(NomMap_Check(value) == false);
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
        CHECK(NomValue_Insert(state, map, key, value) == true);
        NomValue result = NomValue_Get(state, map, key);
        CHECK(NomValue_Equals(state, result, value) == true);
    }

    SECTION("Using string keys")
    {
        NomValue key = NomString_FromString(state, "Key", false);
        NomValue value = NomNumber_FromInt(10);
        CHECK(NomValue_Insert(state, map, key, value) == true);
        NomValue result = NomValue_Get(state, map, key);
        CHECK(NomValue_Equals(state, result, value) == true);
    }

    SECTION("Using interned string keys")
    {
        NomValue key = NomString_FromString(state, "Key", true);
        NomValue value = NomNumber_FromInt(10);
        CHECK(NomValue_Insert(state, map, key, value) == true);
        NomValue result = NomValue_Get(state, map, key);
        CHECK(NomValue_Equals(state, result, value) == true);
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
    CHECK(NomValue_Insert(state, map, key, value) == true);
    CHECK(NomValue_Set(state, map, key, newValue) == true);

    NomValue result = NomValue_Get(state, map, key);
    CHECK(NomValue_Equals(state, result, newValue) == true);

    NomState_Free(state);
}

TEST_CASE("Calling NomValue_TryGet() for a key that exists", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue key = NomNumber_FromInt(5);
    NomValue value = NomNumber_FromInt(10);
    CHECK(NomValue_Insert(state, map, key, value) == true);

    NomValue result;
    CHECK(NomValue_TryGet(state, map, key, &result) == true);
    CHECK(NomValue_Equals(state, result, value) == true);

    NomState_Free(state);
}

TEST_CASE("Calling NomValue_TryGet() for a key that does not exist in a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue key = NomNumber_FromInt(5);
    NomValue value = NomNumber_FromInt(10);
    CHECK(NomValue_Insert(state, map, key, value) == true);

    NomValue result;
    CHECK(NomValue_TryGet(state, map, value, &result) == false);

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

    CHECK(NomValue_Insert(state, map, a, b) == true);
    CHECK(NomValue_Insert(state, map, b, c) == true);
    CHECK(NomValue_Insert(state, map, c, d) == true);
    CHECK(NomValue_Insert(state, map, d, e) == true);
    CHECK(NomValue_Insert(state, map, e, a) == true);

    NomValue result;
    result = NomValue_Get(state, map, a);
    CHECK(NomValue_Equals(state, result, b) == true);
    result = NomValue_Get(state, map, b);
    CHECK(NomValue_Equals(state, result, c) == true);
    result = NomValue_Get(state, map, c);
    CHECK(NomValue_Equals(state, result, d) == true);
    result = NomValue_Get(state, map, e);
    CHECK(NomValue_Equals(state, result, a) == true);

    NomState_Free(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from interned strings in a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);
    NomValue c = NomString_FromString(state, "c", true);
    NomValue d = NomString_FromString(state, "d", true);
    NomValue e = NomString_FromString(state, "e", true);

    CHECK(NomValue_Insert(state, map, a, b) == true);
    CHECK(NomValue_Insert(state, map, b, c) == true);
    CHECK(NomValue_Insert(state, map, c, d) == true);
    CHECK(NomValue_Insert(state, map, d, e) == true);
    CHECK(NomValue_Insert(state, map, e, a) == true);

    NomValue result;
    result = NomValue_Get(state, map, a);
    CHECK(NomValue_Equals(state, result, b) == true);
    result = NomValue_Get(state, map, b);
    CHECK(NomValue_Equals(state, result, c) == true);
    result = NomValue_Get(state, map, c);
    CHECK(NomValue_Equals(state, result, d) == true);
    result = NomValue_Get(state, map, e);
    CHECK(NomValue_Equals(state, result, a) == true);

    NomState_Free(state);
}

TEST_CASE("Inserting values keyed from mixed interned/non-interned strings into a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);

    NomValue a = NomString_FromString(state, "a", true);
    NomValue b = NomString_FromString(state, "b", true);

    CHECK(NomValue_Insert(state, map, a, b) == true);

    NomValue a2 = NomString_FromString(state, "a", false);

    NomValue result;
    result = NomValue_Get(state, map, a2);
    CHECK(NomValue_Equals(state, result, b) == true);

    NomState_Free(state);
}

TEST_CASE("Calling NomValue_IsIterable() a Nominal map", "[Map]")
{
    NomState* state = NomState_Create();

    NomValue map = NomMap_Create(state);
    CHECK(NomValue_IsIterable(state, map) == true);

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

    CHECK(NomValue_Insert(state, map, a, a) == true);
    CHECK(NomValue_Insert(state, map, b, c) == true);
    CHECK(NomValue_Insert(state, map, c, e) == true);
    CHECK(NomValue_Insert(state, map, d, f) == true);

    NomIterator iterator = { 0 };
    CHECK(NomValue_MoveNext(state, map, &iterator) == true);
    CHECK(NomValue_Equals(state, iterator.key, a) == true);
    CHECK(NomValue_Equals(state, iterator.value, a) == true);
    CHECK(NomValue_MoveNext(state, map, &iterator) == true);
    CHECK(NomValue_Equals(state, iterator.key, b) == true);
    CHECK(NomValue_Equals(state, iterator.value, c) == true);
    CHECK(NomValue_MoveNext(state, map, &iterator) == true);
    CHECK(NomValue_Equals(state, iterator.key, c) == true);
    CHECK(NomValue_Equals(state, iterator.value, e) == true);
    CHECK(NomValue_MoveNext(state, map, &iterator) == true);
    CHECK(NomValue_Equals(state, iterator.key, d) == true);
    CHECK(NomValue_Equals(state, iterator.value, f) == true);
    CHECK(NomValue_MoveNext(state, map, &iterator) == false);

    NomState_Free(state);
}
