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

TEST_CASE("Creating and invoking functions with parameters", "[Function]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("[ a b | a + b ]: 2 3", nom_fromint(5));
    TEST_EXPR("id := [ a | a ], id: 2", nom_fromint(2));
    TEST_EXPR("id:", nom_nil());

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

TEST_CASE("Creating and invoking native functions", "[Function]")
{
    NomState* state = nom_newstate();

    NomValue function = nom_newfunction(state, test);
    CHECK(nom_isfunction(function));

    nom_letvar(state, "test", function);
    nom_release(state, function);

    CHECK(!nom_error(state));

    TEST_EXPR("test: 2 3", nom_fromint(5));

    nom_freestate(state);
}

TEST_CASE("Invoking the 'assertEqual' function", "[Function]")
{
    NomState* state = nom_newstate();

    TEST_EXPR("assertEqual: \"A\" \"A\"", nom_nil());
    TEST_EXPR_ERROR("assertEqual: \"A\" \"B\"");

    nom_freestate(state);
}
