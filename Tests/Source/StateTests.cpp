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
    REQUIRE(state != NULL); \
    NomValue value = NomState_Execute(state, expr); \
    REQUIRE(NomState_ErrorOccurred(state) == false); \
    REQUIRE(NomValue_Equals(state, value, expected) == true); \
}

#define TEST_EXPR_ERROR(expr)\
{\
    REQUIRE(state != NULL); \
    NomState_Execute(state, expr); \
    REQUIRE(NomState_ErrorOccurred(state) == true); \
}

TEST_CASE("Performing arithmetic operations", "[State]")
{
    NomState* state = NomState_Create();
    TEST_EXPR("2 + 3", NomNumber_FromInt(5));
    TEST_EXPR("2 - 3", NomNumber_FromInt(-1));
    TEST_EXPR("2 * 3", NomNumber_FromInt(6));
    TEST_EXPR("2 * 3 + 1", NomNumber_FromInt(7));
    TEST_EXPR("2 * (3 + 1)", NomNumber_FromInt(8));
    TEST_EXPR("6 / 3", NomNumber_FromInt(2));
    TEST_EXPR("2 + 3.0", NomNumber_FromDouble(5.0));
    TEST_EXPR("2.0 + 3.0", NomNumber_FromDouble(5.0));
    TEST_EXPR("2.0 + 3", NomNumber_FromDouble(5.0));
    TEST_EXPR("2 - 3.0", NomNumber_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3", NomNumber_FromDouble(-1.0));
    TEST_EXPR("2.0 - 3.0", NomNumber_FromDouble(-1.0));
    TEST_EXPR("2 * 3.0", NomNumber_FromDouble(6.0));
    TEST_EXPR("2.0 * 3", NomNumber_FromDouble(6.0));
    TEST_EXPR("2 * 3.0", NomNumber_FromDouble(6.0));
    TEST_EXPR("6 / 3.0", NomNumber_FromDouble(2.0));
    TEST_EXPR("6.0 / 3", NomNumber_FromDouble(2.0));
    TEST_EXPR("6 / 3.0", NomNumber_FromDouble(2.0));
    TEST_EXPR("6.0 / 4.0", NomNumber_FromDouble(6.0 / 4.0));
    TEST_EXPR("6.53 / 4.23", NomNumber_FromDouble(6.53 / 4.23));
    NomState_Free(state);
}

TEST_CASE("Setting global variables", "[State]")
{
    NomState* state = NomState_Create();
    TEST_EXPR("a := 1, b := 2, a + b", NomNumber_FromInt(3));
    NomState_Free(state);
}

TEST_CASE("Creating a map with implicit keys", "[State]")
{
    NomState* state = NomState_Create();

    NomValue map = NomState_Execute(state, "{ 0, 1, 2, 3 }");
    REQUIRE(NomState_ErrorOccurred(state) == false);
    REQUIRE(NomMap_Check(map) == true);

    NomValue result;

    for (int i = 0; i < 4; ++i)
    {
        result = NomMap_Get(state, map, NomNumber_FromInt(i));
        REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(i)));
    }

    NomState_Free(state);
}

TEST_CASE("Creating a map with explicit keys", "[State]")
{
    NomState* state = NomState_Create();

    NomValue map = NomState_Execute(state, "{ \"zero\" -> 0, \"one\" -> 1, two := 2 }");
    REQUIRE(NomState_ErrorOccurred(state) == false);
    REQUIRE(NomMap_Check(map));

    NomValue result;
    result = NomMap_Get(state, map, NomString_FromString(state, "zero", false));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(0)) == true);
    result = NomMap_Get(state, map, NomString_FromString(state, "one", false));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(1)) == true);
    result = NomMap_Get(state, map, NomString_FromString(state, "two", false));
    REQUIRE(NomValue_Equals(state, result, NomNumber_FromInt(2)) == true);

    NomState_Free(state);
}

TEST_CASE("Indexing maps", "[State]")
{
    NomState* state = NomState_Create();
    TEST_EXPR("{ 5 }[0]", NomNumber_FromInt(5));
    TEST_EXPR("{ 2, 3, 4, 5 }[2]", NomNumber_FromInt(4));
    TEST_EXPR("{ one := 1 }.one", NomNumber_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }[\"two\"][\"one\"]", NomNumber_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }.two.one", NomNumber_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } }.two).one", NomNumber_FromInt(1));
    TEST_EXPR("{ two := { one := 1 } }[{ }]", NomValue_Nil());
    TEST_EXPR("{ two := { one := 1 } }[{ one := \"two\" }.one].one", NomNumber_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } })[{ one := \"two\" }.one].one", NomNumber_FromInt(1));
    TEST_EXPR("({ two := { one := 1 } })[({ one := \"two\" }.one)].one", NomNumber_FromInt(1));
    TEST_EXPR("one := { 0 }, two := { one -> 1 }, two[one]", NomNumber_FromInt(1));
    TEST_EXPR("a := { }, a.b := 1, a.b", NomNumber_FromInt(1));
    TEST_EXPR("b := { }, b[\"c\"] = 1, b.c", NomNumber_FromInt(1));
    TEST_EXPR("d := { }, d.e := 1, d.e", NomNumber_FromInt(1));

    TEST_EXPR_ERROR("f := { }, f.g");
    TEST_EXPR_ERROR("f.g = 1");

    NomState_Free(state);
}
