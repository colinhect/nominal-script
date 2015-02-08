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

#define TEST_EXPR(expr, expected)\
{\
    CHECK(state != NULL); \
    NomValue value = Nom_Execute(state, expr); \
    CHECK(Nom_ErrorOccurred(state) == false); \
    CHECK(Nom_Equals(state, value, expected) == true); \
}

#define TEST_EXPR_ERROR(expr)\
{\
    CHECK(state != NULL); \
    Nom_Execute(state, expr); \
    CHECK(Nom_ErrorOccurred(state) == true); \
}

TEST_CASE("Performing arithmetic operations", "[State]")
{
    NomState* state = Nom_NewState();
    TEST_EXPR("2 + 3", Nom_FromInt(5));
    TEST_EXPR("2 - 3", Nom_FromInt(-1));
    TEST_EXPR("2 * 3", Nom_FromInt(6));
    TEST_EXPR("2 * 3 + 1", Nom_FromInt(7));
    TEST_EXPR("2 * (3 + 1)", Nom_FromInt(8));
    TEST_EXPR("6 / 3", Nom_FromInt(2));
    TEST_EXPR("2 + 3.0", Nom_FromDouble(5.0));
    TEST_EXPR("2.0 + 3.0", Nom_FromDouble(5.0));
    TEST_EXPR("2.0 + 3", Nom_FromDouble(5.0));
    TEST_EXPR("2 - 3.0", Nom_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3", Nom_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3.0", Nom_FromDouble(-1.0));
    TEST_EXPR("2 * 3.0", Nom_FromDouble(6.0));
    TEST_EXPR("2.0 * 3", Nom_FromDouble(6.0));
    TEST_EXPR("2 * 3.0", Nom_FromDouble(6.0));
    TEST_EXPR("6 / 3.0", Nom_FromDouble(2.0));
    TEST_EXPR("6.0 / 3", Nom_FromDouble(2.0));
    TEST_EXPR("6 / 3.0", Nom_FromDouble(2.0));
    TEST_EXPR("6.0 / 4.0", Nom_FromDouble(6.0 / 4.0));
    TEST_EXPR("6.53 / 4.23", Nom_FromDouble(6.53 / 4.23));
    Nom_FreeState(state);
}

TEST_CASE("Setting global variables", "[State]")
{
    NomState* state = Nom_NewState();
    TEST_EXPR("a := 1, b := 2, a + b", Nom_FromInt(3));
    Nom_FreeState(state);
}

TEST_CASE("Creating a map with implicit keys", "[State]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_Execute(state, "{ 0, 1, 2, 3 }");
    CHECK(Nom_ErrorOccurred(state) == false);
    CHECK(Nom_IsMap(map) == true);

    NomValue result;

    for (int i = 0; i < 4; ++i)
    {
        result = Nom_Get(state, map, Nom_FromInt(i));
        CHECK(Nom_Equals(state, result, Nom_FromInt(i)));
    }

    Nom_FreeState(state);
}

TEST_CASE("Creating a map with explicit keys", "[State]")
{
    NomState* state = Nom_NewState();

    NomValue map = Nom_Execute(state, "{ \"zero\" -> 0, \"one\" -> 1, two := 2 }");
    CHECK(Nom_ErrorOccurred(state) == false);
    CHECK(Nom_IsMap(map));

    NomValue result;
    result = Nom_Get(state, map, Nom_NewString(state, "zero", false));
    CHECK(Nom_Equals(state, result, Nom_FromInt(0)) == true);
    result = Nom_Get(state, map, Nom_NewString(state, "one", false));
    CHECK(Nom_Equals(state, result, Nom_FromInt(1)) == true);
    result = Nom_Get(state, map, Nom_NewString(state, "two", false));
    CHECK(Nom_Equals(state, result, Nom_FromInt(2)) == true);

    Nom_FreeState(state);
}

TEST_CASE("Indexing maps", "[State]")
{
    NomState* state = Nom_NewState();

    TEST_EXPR("{ 5 }[0]", Nom_FromInt(5));
    TEST_EXPR("{ 2, 3, 4, 5 }[2]", Nom_FromInt(4));
    TEST_EXPR("{ one := 1 }.one", Nom_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }[\"two\"][\"one\"]", Nom_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }.two.one", Nom_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } }.two).one", Nom_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }[{ }]", Nom_Nil());
    TEST_EXPR("{ two := { one := 1 } }[{ one := \"two\" }.one].one", Nom_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } })[{ one := \"two\" }.one].one", Nom_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } })[({ one := \"two\" }.one)].one", Nom_FromInt(1));
    TEST_EXPR("one := { 0 }, two := { one -> 1 }, two[one]", Nom_FromInt(1));
    TEST_EXPR("a := { }, a.b := 1, a.b", Nom_FromInt(1));
    TEST_EXPR("b := { }, b[\"c\"] = 1, b.c", Nom_FromInt(1));
    TEST_EXPR("d := { }, d.e := 1, d.e", Nom_FromInt(1));

    TEST_EXPR_ERROR("f := { }, f.g");
    TEST_EXPR_ERROR("f.g = 1");

    Nom_FreeState(state);
}

TEST_CASE("Creating and invoking trivial functions", "[State]")
{
    NomState* state = Nom_NewState();

    TEST_EXPR("[ 1 ]:", Nom_FromInt(1));
    TEST_EXPR("([ 1 ]):", Nom_FromInt(1));
    TEST_EXPR("[ 1, 2, 3 ]:", Nom_FromInt(3));
    TEST_EXPR("[ 1, 2, 3, { 4, 5 } ]:[1]", Nom_FromInt(5));
    TEST_EXPR("([ 1, 2, 3, { 4, 5 } ]:)[1]", Nom_FromInt(5));
    TEST_EXPR("-[[[[42]]]]::::", Nom_FromInt(-42));
    TEST_EXPR("a := [ 0, 1, 2 ], b := a:, b", Nom_FromInt(2));
    TEST_EXPR("f := [ 2 ], g := [ f: + 3 ], g:", Nom_FromInt(5));
    TEST_EXPR("c := { f := [ 23 ], g := [ 19 ] }, c.f: + c.g:", Nom_FromInt(42));
    TEST_EXPR("e := { f := [ [ 23 ] ] }, e.f::", Nom_FromInt(23));
    TEST_EXPR("[ { 0, 1, [ 7 + 3 ] } ]:[2]:", Nom_FromInt(10));
    TEST_EXPR("[ { zero := 0, one := 1, two := 2 } ]:.one", Nom_FromInt(1));
    TEST_EXPR("x := 1, y := 3, w := [ x := 4, x + y ]:, w + x", Nom_FromInt(8));

    TEST_EXPR_ERROR("z := { }, z:");

    Nom_FreeState(state);
}

TEST_CASE("Creating and invoking functions with parameters", "[State]")
{
    NomState* state = Nom_NewState();

    TEST_EXPR("[ a b | a + b ]: 2 3", Nom_FromInt(5));
    TEST_EXPR("id := [ a | a ], id: 2", Nom_FromInt(2));
    TEST_EXPR("id:", Nom_Nil());

    TEST_EXPR_ERROR("z := [ a b c | a + b + c ], z: 1 2 3 4");

    Nom_FreeState(state);
}
