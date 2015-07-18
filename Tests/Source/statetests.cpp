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

#define TEST_EXPR(expr, expected)\
{\
    CHECK(state != NULL); \
    NomValue value = nom_execute(state, expr); \
    CHECK(nom_error(state) == false); \
    CHECK(nom_equals(state, value, expected) == true); \
}

#define TEST_EXPR_ERROR(expr)\
{\
    CHECK(state != NULL); \
    nom_execute(state, expr); \
    CHECK(nom_error(state) == true); \
}

TEST_CASE("Performing arithmetic operations", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("2 + 3", nom_fromint(5));
    TEST_EXPR("2 - 3", nom_fromint(-1));
    TEST_EXPR("2 * 3", nom_fromint(6));
    TEST_EXPR("2 * 3 + 1", nom_fromint(7));
    TEST_EXPR("2 * (3 + 1)", nom_fromint(8));
    TEST_EXPR("6 / 3", nom_fromint(2));
    TEST_EXPR("2 + 3.0", nom_fromdouble(5.0));
    TEST_EXPR("2.0 + 3.0", nom_fromdouble(5.0));
    TEST_EXPR("2.0 + 3", nom_fromdouble(5.0));
    TEST_EXPR("2 - 3.0", nom_fromdouble(-1.0));
    TEST_EXPR("2.0 - 3", nom_fromdouble(-1.0));
    TEST_EXPR("2.0 - 3.0", nom_fromdouble(-1.0));
    TEST_EXPR("2 * 3.0", nom_fromdouble(6.0));
    TEST_EXPR("2.0 * 3", nom_fromdouble(6.0));
    TEST_EXPR("2 * 3.0", nom_fromdouble(6.0));
    TEST_EXPR("6 / 3.0", nom_fromdouble(2.0));
    TEST_EXPR("6.0 / 3", nom_fromdouble(2.0));
    TEST_EXPR("6 / 3.0", nom_fromdouble(2.0));
    TEST_EXPR("6.0 / 4.0", nom_fromdouble(6.0 / 4.0));
    TEST_EXPR("6.53 / 4.23", nom_fromdouble(6.53 / 4.23));

    nom_freestate(state);
}

TEST_CASE("Setting global variables", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("a := 1, b := 2, a + b", nom_fromint(3));

    nom_freestate(state);
}

TEST_CASE("Creating a map with implicit keys", "[State]")
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

    nom_freestate(state);
}

TEST_CASE("Creating a map with explicit keys", "[State]")
{
    NomState* state = nom_newstate();

    NomValue map = nom_execute(state, "{ \"zero\" -> 0, \"one\" -> 1, two := 2 }");
    CHECK(nom_error(state) == false);
    CHECK(nom_ismap(map));

    NomValue result;
    result = nom_get(state, map, nom_newstring(state, "zero", false));
    CHECK(nom_equals(state, result, nom_fromint(0)) == true);
    result = nom_get(state, map, nom_newstring(state, "one", false));
    CHECK(nom_equals(state, result, nom_fromint(1)) == true);
    result = nom_get(state, map, nom_newstring(state, "two", false));
    CHECK(nom_equals(state, result, nom_fromint(2)) == true);

    nom_freestate(state);
}

TEST_CASE("Indexing maps", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR_ERROR("f := { }, f.g");
    TEST_EXPR_ERROR("f.g = 1");

    nom_freestate(state);
}

TEST_CASE("Creating and invoking trivial functions", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR_ERROR("z := { }, z:");

    nom_freestate(state);
}

TEST_CASE("Creating and invoking functions with parameters", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("[ a b | a + b ]: 2 3", nom_fromint(5));
    TEST_EXPR("id := [ a | a ], id: 2", nom_fromint(2));
    TEST_EXPR("id:", nom_nil());

    TEST_EXPR_ERROR("z := [ a b c | a + b + c ], z: 1 2 3 4");

    nom_freestate(state);
}

NomValue test(NomState* state)
{
    CHECK(nom_getargcount(state) == 2);

    NomValue left = nom_getarg(state, 0);
    NomValue right = nom_getarg(state, 1);
    NomValue over = nom_getarg(state, 2);

    CHECK(nom_equals(state, left, nom_fromint(2)));
    CHECK(nom_equals(state, right, nom_fromint(3)));
    CHECK(nom_equals(state, over, nom_nil()));

    return nom_add(state, left, right);
}

TEST_CASE("Creating and invoking native functions", "[State]")
{
    NomState* state = nom_newstate();

    NomValue function = nom_newfunction(state, test);
    CHECK(nom_isfunction(function));

    nom_letvar(state, "test", function);
    CHECK(!nom_error(state));

    TEST_EXPR("test: 2 3", nom_fromint(5));

    nom_freestate(state);
}

TEST_CASE("Invoking the 'assertEqual' function", "[State]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("assertEqual: \"A\" \"A\"", nom_nil());
    TEST_EXPR_ERROR("assertEqual: \"A\" \"B\"");

    nom_freestate(state);
}
