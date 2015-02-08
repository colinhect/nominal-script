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
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);
    CHECK(Nom_IsMap(map) == true);

    Nom_FreeState(state);
}

TEST_CASE("Calling Nom_IsMap() on non-map Nominal values", "[Map]")
{
    NomState* state = Nom_NewState();

    SECTION("Checking nil")
    {
        NomValue value = Nom_Nil();
        CHECK(Nom_IsMap(value) == false);
    }

    SECTION("Checking number")
    {
        NomValue value = Nom_FromDouble(1.0);
        CHECK(Nom_IsMap(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = Nom_NewString(state, "Testing...", false);
        CHECK(Nom_IsMap(value) == false);
    }

    SECTION("Checking interned string")
    {
        NomValue value = Nom_NewString(state, "Testing...", true);
        CHECK(Nom_IsMap(value) == false);
    }

    Nom_FreeState(state);
}

TEST_CASE("Inserting and retrieving values to/from a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();
    
    NomValue map = Nom_NewMap(state);

    SECTION("Using number keys")
    {
        NomValue key = Nom_FromInt(5);
        NomValue value = Nom_FromInt(10);
        CHECK(Nom_Insert(state, map, key, value) == true);
        NomValue result = Nom_Get(state, map, key);
        CHECK(Nom_Equals(state, result, value) == true);
    }

    SECTION("Using string keys")
    {
        NomValue key = Nom_NewString(state, "Key", false);
        NomValue value = Nom_FromInt(10);
        CHECK(Nom_Insert(state, map, key, value) == true);
        NomValue result = Nom_Get(state, map, key);
        CHECK(Nom_Equals(state, result, value) == true);
    }

    SECTION("Using interned string keys")
    {
        NomValue key = Nom_NewString(state, "Key", true);
        NomValue value = Nom_FromInt(10);
        CHECK(Nom_Insert(state, map, key, value) == true);
        NomValue result = Nom_Get(state, map, key);
        CHECK(Nom_Equals(state, result, value) == true);
    }

    Nom_FreeState(state);
}

TEST_CASE("Setting a value in a Nominal map after it is inserted", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue key = Nom_FromInt(5);
    NomValue value = Nom_FromInt(10);
    NomValue newValue = Nom_FromInt(25);
    CHECK(Nom_Insert(state, map, key, value) == true);
    CHECK(Nom_Set(state, map, key, newValue) == true);

    NomValue result = Nom_Get(state, map, key);
    CHECK(Nom_Equals(state, result, newValue) == true);

    Nom_FreeState(state);
}

TEST_CASE("Calling Nom_TryGet() for a key that exists", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue key = Nom_FromInt(5);
    NomValue value = Nom_FromInt(10);
    CHECK(Nom_Insert(state, map, key, value) == true);

    NomValue result;
    CHECK(Nom_TryGet(state, map, key, &result) == true);
    CHECK(Nom_Equals(state, result, value) == true);

    Nom_FreeState(state);
}

TEST_CASE("Calling Nom_TryGet() for a key that does not exist in a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue key = Nom_FromInt(5);
    NomValue value = Nom_FromInt(10);
    CHECK(Nom_Insert(state, map, key, value) == true);

    NomValue result;
    CHECK(Nom_TryGet(state, map, value, &result) == false);

    Nom_FreeState(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from strings in a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue a = Nom_NewString(state, "a", false);
    NomValue b = Nom_NewString(state, "b", false);
    NomValue c = Nom_NewString(state, "c", false);
    NomValue d = Nom_NewString(state, "d", false);
    NomValue e = Nom_NewString(state, "e", false);

    CHECK(Nom_Insert(state, map, a, b) == true);
    CHECK(Nom_Insert(state, map, b, c) == true);
    CHECK(Nom_Insert(state, map, c, d) == true);
    CHECK(Nom_Insert(state, map, d, e) == true);
    CHECK(Nom_Insert(state, map, e, a) == true);

    NomValue result;
    result = Nom_Get(state, map, a);
    CHECK(Nom_Equals(state, result, b) == true);
    result = Nom_Get(state, map, b);
    CHECK(Nom_Equals(state, result, c) == true);
    result = Nom_Get(state, map, c);
    CHECK(Nom_Equals(state, result, d) == true);
    result = Nom_Get(state, map, e);
    CHECK(Nom_Equals(state, result, a) == true);

    Nom_FreeState(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from interned strings in a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue a = Nom_NewString(state, "a", true);
    NomValue b = Nom_NewString(state, "b", true);
    NomValue c = Nom_NewString(state, "c", true);
    NomValue d = Nom_NewString(state, "d", true);
    NomValue e = Nom_NewString(state, "e", true);

    CHECK(Nom_Insert(state, map, a, b) == true);
    CHECK(Nom_Insert(state, map, b, c) == true);
    CHECK(Nom_Insert(state, map, c, d) == true);
    CHECK(Nom_Insert(state, map, d, e) == true);
    CHECK(Nom_Insert(state, map, e, a) == true);

    NomValue result;
    result = Nom_Get(state, map, a);
    CHECK(Nom_Equals(state, result, b) == true);
    result = Nom_Get(state, map, b);
    CHECK(Nom_Equals(state, result, c) == true);
    result = Nom_Get(state, map, c);
    CHECK(Nom_Equals(state, result, d) == true);
    result = Nom_Get(state, map, e);
    CHECK(Nom_Equals(state, result, a) == true);

    Nom_FreeState(state);
}

TEST_CASE("Inserting values keyed from mixed interned/non-interned strings into a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue a = Nom_NewString(state, "a", true);
    NomValue b = Nom_NewString(state, "b", true);

    CHECK(Nom_Insert(state, map, a, b) == true);

    NomValue a2 = Nom_NewString(state, "a", false);

    NomValue result;
    result = Nom_Get(state, map, a2);
    CHECK(Nom_Equals(state, result, b) == true);

    Nom_FreeState(state);
}

TEST_CASE("Calling Nom_IsIterable() a Nominal map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);
    CHECK(Nom_IsIterable(state, map) == true);

    Nom_FreeState(state);
}

TEST_CASE("Iterating over a map", "[Map]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_NewMap(state);

    NomValue a = Nom_FromInt(0);
    NomValue b = Nom_FromInt(1);
    NomValue c = Nom_FromInt(2);
    NomValue d = Nom_FromInt(3);
    NomValue e = Nom_FromInt(4);
    NomValue f = Nom_FromInt(5);

    CHECK(Nom_Insert(state, map, a, a) == true);
    CHECK(Nom_Insert(state, map, b, c) == true);
    CHECK(Nom_Insert(state, map, c, e) == true);
    CHECK(Nom_Insert(state, map, d, f) == true);

    NomIterator iterator = { 0 };
    CHECK(Nom_MoveNext(state, map, &iterator) == true);
    CHECK(Nom_Equals(state, iterator.key, a) == true);
    CHECK(Nom_Equals(state, iterator.value, a) == true);
    CHECK(Nom_MoveNext(state, map, &iterator) == true);
    CHECK(Nom_Equals(state, iterator.key, b) == true);
    CHECK(Nom_Equals(state, iterator.value, c) == true);
    CHECK(Nom_MoveNext(state, map, &iterator) == true);
    CHECK(Nom_Equals(state, iterator.key, c) == true);
    CHECK(Nom_Equals(state, iterator.value, e) == true);
    CHECK(Nom_MoveNext(state, map, &iterator) == true);
    CHECK(Nom_Equals(state, iterator.key, d) == true);
    CHECK(Nom_Equals(state, iterator.value, f) == true);
    CHECK(Nom_MoveNext(state, map, &iterator) == false);

    Nom_FreeState(state);
}
