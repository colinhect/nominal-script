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
#include <catch.hpp>

extern "C"
{
#include <nominal.h>
}

template <typename T>
void check_number(
    NomValue    value,
    T           expected
)
{
    SECTION("Converting to int")
    {
        CHECK(nom_toint(value) == (int)expected);
    }

    SECTION("Converting to unsigned int")
    {
        CHECK(nom_tounsignedint(value) == (unsigned int)expected);
    }

    SECTION("Converting to long")
    {
        CHECK(nom_tolong(value) == (long)expected);
    }

    SECTION("Converting to unsigned long")
    {
        CHECK(nom_tounsignedlong(value) == (unsigned long)expected);
    }

    SECTION("Converting to long long")
    {
        CHECK(nom_tolonglong(value) == (long long)expected);
    }

    SECTION("Converting to unsigned long long")
    {
        CHECK(nom_tounsignedlonglong(value) == (unsigned long long)expected);
    }

    SECTION("Converting to size_t")
    {
        CHECK(nom_tosize(value) == (size_t)expected);
    }

    SECTION("Converting to float")
    {
        CHECK(nom_tofloat(value) == (float)expected);
    }

    SECTION("Converting to double")
    {
        CHECK(nom_todouble(value) == (double)expected);
    }
}

TEST_CASE("Creating/converting a Nominal number from/to all supported numeric types", "[Number]")
{
    SECTION("Creating from int")
    {
        NomValue value = nom_fromint(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from unsigned int")
    {
        NomValue value = nom_fromunsignedint(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from long")
    {
        NomValue value = nom_fromlong(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from unsigned long")
    {
        NomValue value = nom_fromunsignedlong(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from long long")
    {
        NomValue value = nom_fromlonglong(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from unsigned long long")
    {
        NomValue value = nom_fromunsignedlonglong(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from size")
    {
        NomValue value = nom_fromsize(4);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4);
    }

    SECTION("Creating from float")
    {
        NomValue value = nom_fromfloat(4.321f);

        CHECK(nom_isnumber(value) == true);

        check_number(value, 4.321f);
    }

    SECTION("Creating from double")
    {
        NomValue value = nom_fromdouble(4.321);

        check_number(value, 4.321);
    }
}

TEST_CASE("Calling nom_isnumber() on non-number Nominal values", "[Number]")
{
    NomState* state = nom_newstate();

    SECTION("Checking nil")
    {
        NomValue value = nom_nil();
        CHECK(nom_isnumber(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = nom_newstring(state, "Testing...");
        CHECK(nom_isnumber(value) == false);
    }

    SECTION("Checking interned string")
    {
        NomValue value = nom_newinternedstring(state, "Testing...");
        CHECK(nom_isnumber(value) == false);
    }

    SECTION("Checking map")
    {
        NomValue value = nom_newmap(state);
        CHECK(nom_isnumber(value) == false);
    }

    nom_freestate(state);
}

TEST_CASE("Performing arithmetic operations on Nominal number values", "[Number]")
{
    NomState* state = nom_newstate();

    SECTION("Addition")
    {
        NomValue a = nom_fromint(3);
        NomValue b = nom_fromint(2);
        NomValue c = nom_add(state, a, b);
        CHECK(nom_isnumber(c) == true);
        CHECK(nom_todouble(c) == 5.0);
    }

    SECTION("Subtraction")
    {
        NomValue a = nom_fromint(3);
        NomValue b = nom_fromint(2);
        NomValue c = nom_sub(state, a, b);
        CHECK(nom_isnumber(c) == true);
        CHECK(nom_todouble(c) == 1.0);
    }

    SECTION("Multiplication")
    {
        NomValue a = nom_fromint(3);
        NomValue b = nom_fromint(2);
        NomValue c = nom_mul(state, a, b);
        CHECK(nom_isnumber(c) == true);
        CHECK(nom_todouble(c) == 6.0);
    }

    SECTION("Division")
    {
        NomValue a = nom_fromint(6);
        NomValue b = nom_fromint(2);
        NomValue c = nom_div(state, a, b);
        CHECK(nom_isnumber(c) == true);
        CHECK(nom_todouble(c) == 3.0);
    }

    nom_freestate(state);
}

TEST_CASE("Calling nom_isiterable() a Nominal number", "[Number]")
{
    NomState* state = nom_newstate();

    NomValue number = nom_fromdouble(1.0);
    CHECK(nom_isiterable(state, number) == false);

    nom_freestate(state);
}
