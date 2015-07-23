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
#include <nominal.h>
}

TEST_CASE("Creating and identifying an empty Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);
    CHECK(nom_ismap(map) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Calling nom_ismap() on non-map Nominal values", "[Map]")
{
    NomState* state = nom_newstate();

    SECTION("Checking nil")
    {
        NomValue value = nom_nil();
        CHECK(nom_ismap(value) == false);
    }

    SECTION("Checking number")
    {
        NomValue value = nom_fromdouble(1.0);
        CHECK(nom_ismap(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = nom_newstring(state, "Testing...");
        CHECK(nom_ismap(value) == false);
        nom_release(state, value);
    }

    SECTION("Checking interned string")
    {
        NomValue value = nom_newinternedstring(state, "Testing...");
        CHECK(nom_ismap(value) == false);
        nom_release(state, value);
    }

    nom_freestate(state);
}

TEST_CASE("Inserting and retrieving values to/from a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();
    
    NomValue map = nom_newmap(state);

    SECTION("Using number keys")
    {
        NomValue key = nom_fromint(5);
        NomValue value = nom_fromint(10);
        CHECK(nom_insert(state, map, key, value) == true);
        NomValue result = nom_get(state, map, key);
        CHECK(nom_equals(state, result, value) == true);
    }

    SECTION("Using string keys")
    {
        NomValue key = nom_newstring(state, "Key");
        NomValue value = nom_fromint(10);
        CHECK(nom_insert(state, map, key, value) == true);
        NomValue result = nom_get(state, map, key);
        CHECK(nom_equals(state, result, value) == true);
    }

    SECTION("Using interned string keys")
    {
        NomValue key = nom_newinternedstring(state, "Key");
        NomValue value = nom_fromint(10);
        CHECK(nom_insert(state, map, key, value) == true);
        NomValue result = nom_get(state, map, key);
        CHECK(nom_equals(state, result, value) == true);
    }

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Setting a value in a Nominal map after it is inserted", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue key = nom_fromint(5);
    NomValue value = nom_fromint(10);
    NomValue newvalue = nom_fromint(25);
    CHECK(nom_insert(state, map, key, value) == true);
    CHECK(nom_set(state, map, key, newvalue) == true);

    NomValue result = nom_get(state, map, key);
    CHECK(nom_equals(state, result, newvalue) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Calling nom_tryget() for a key that exists", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue key = nom_fromint(5);
    NomValue value = nom_fromint(10);
    CHECK(nom_insert(state, map, key, value) == true);

    NomValue result;
    CHECK(nom_tryget(state, map, key, &result) == true);
    CHECK(nom_equals(state, result, value) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Calling nom_tryget() for a key that does not exist in a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue key = nom_fromint(5);
    NomValue value = nom_fromint(10);
    CHECK(nom_insert(state, map, key, value) == true);

    NomValue result;
    CHECK(nom_tryget(state, map, value, &result) == false);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from strings in a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue a = nom_newstring(state, "a");
    NomValue b = nom_newstring(state, "b");
    NomValue c = nom_newstring(state, "c");
    NomValue d = nom_newstring(state, "d");
    NomValue e = nom_newstring(state, "e");

    CHECK(nom_insert(state, map, a, b) == true);
    CHECK(nom_insert(state, map, b, c) == true);
    CHECK(nom_insert(state, map, c, d) == true);
    CHECK(nom_insert(state, map, d, e) == true);
    CHECK(nom_insert(state, map, e, a) == true);

    NomValue result;
    result = nom_get(state, map, a);
    CHECK(nom_equals(state, result, b) == true);
    result = nom_get(state, map, b);
    CHECK(nom_equals(state, result, c) == true);
    result = nom_get(state, map, c);
    CHECK(nom_equals(state, result, d) == true);
    result = nom_get(state, map, e);
    CHECK(nom_equals(state, result, a) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Inserting and retrieving multiple values keyed from interned strings in a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue a = nom_newinternedstring(state, "a");
    NomValue b = nom_newinternedstring(state, "b");
    NomValue c = nom_newinternedstring(state, "c");
    NomValue d = nom_newinternedstring(state, "d");
    NomValue e = nom_newinternedstring(state, "e");

    CHECK(nom_insert(state, map, a, b) == true);
    CHECK(nom_insert(state, map, b, c) == true);
    CHECK(nom_insert(state, map, c, d) == true);
    CHECK(nom_insert(state, map, d, e) == true);
    CHECK(nom_insert(state, map, e, a) == true);

    NomValue result;
    result = nom_get(state, map, a);
    CHECK(nom_equals(state, result, b) == true);
    result = nom_get(state, map, b);
    CHECK(nom_equals(state, result, c) == true);
    result = nom_get(state, map, c);
    CHECK(nom_equals(state, result, d) == true);
    result = nom_get(state, map, e);
    CHECK(nom_equals(state, result, a) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Inserting values keyed from mixed interned/non-interned strings into a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue a = nom_newinternedstring(state, "a");
    NomValue b = nom_newinternedstring(state, "b");

    CHECK(nom_insert(state, map, a, b) == true);

    NomValue a2 = nom_newstring(state, "a");

    NomValue result;
    result = nom_get(state, map, a2);
    CHECK(nom_equals(state, result, b) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Calling nom_isiterable() a Nominal map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);
    CHECK(nom_isiterable(state, map) == true);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Iterating over a map", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_newmap(state);

    NomValue a = nom_fromint(0);
    NomValue b = nom_fromint(1);
    NomValue c = nom_fromint(2);
    NomValue d = nom_fromint(3);
    NomValue e = nom_fromint(4);
    NomValue f = nom_fromint(5);

    CHECK(nom_insert(state, map, a, a) == true);
    CHECK(nom_insert(state, map, b, c) == true);
    CHECK(nom_insert(state, map, c, e) == true);
    CHECK(nom_insert(state, map, d, f) == true);

    NomIterator iterator = { 0 };
    CHECK(nom_next(state, map, &iterator) == true);
    CHECK(nom_equals(state, iterator.key, a) == true);
    CHECK(nom_equals(state, iterator.value, a) == true);
    CHECK(nom_next(state, map, &iterator) == true);
    CHECK(nom_equals(state, iterator.key, b) == true);
    CHECK(nom_equals(state, iterator.value, c) == true);
    CHECK(nom_next(state, map, &iterator) == true);
    CHECK(nom_equals(state, iterator.key, c) == true);
    CHECK(nom_equals(state, iterator.value, e) == true);
    CHECK(nom_next(state, map, &iterator) == true);
    CHECK(nom_equals(state, iterator.key, d) == true);
    CHECK(nom_equals(state, iterator.value, f) == true);
    CHECK(nom_next(state, map, &iterator) == false);

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Creating a map with implicit keys", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_execute(state, "{ 0, 1, 2, 3 }");
    CHECK(nom_error(state) == false);
    CHECK(nom_ismap(map) == true);

    NomValue result;

    for (int i = 0; i < 4; ++i)
    {
        result = nom_get(state, map, nom_fromint(i));
        CHECK(nom_equals(state, result, nom_fromint(i)));
    }

    nom_release(state, map);
    nom_freestate(state);
}

TEST_CASE("Creating a map with explicit keys", "[Map]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_execute(state, "{ \"zero\" -> 0, \"one\" -> 1, two := 2 }");
    CHECK(nom_error(state) == false);
    CHECK(nom_ismap(map));

    NomValue result;
    result = nom_get(state, map, nom_newstring(state, "zero"));
    CHECK(nom_equals(state, result, nom_fromint(0)) == true);
    result = nom_get(state, map, nom_newstring(state, "one"));
    CHECK(nom_equals(state, result, nom_fromint(1)) == true);
    result = nom_get(state, map, nom_newstring(state, "two"));
    CHECK(nom_equals(state, result, nom_fromint(2)) == true);

    nom_freestate(state);
}

TEST_CASE("Indexing maps", "[Map]")
{
    NomState* state = nom_newstate();

    nom_execute(state, "f := { }, f.g");
    CHECK(nom_error(state) == true);

    nom_execute(state, "f.g = 1");
    CHECK(nom_error(state) == true);

    nom_freestate(state);
}
