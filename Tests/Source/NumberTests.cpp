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

TEST_CASE("Creating/converting a Nominal number from/to all supported numeric types", "[Number]")
{
    SECTION("Creating from int")
    {
        NomValue value = Nom_FromInt(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned int")
    {
        NomValue value = Nom_FromUnsignedInt(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from long")
    {
        NomValue value = Nom_FromLong(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned long")
    {
        NomValue value = Nom_FromUnsignedLong(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from long long")
    {
        NomValue value = Nom_FromLongLong(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned long long")
    {
        NomValue value = Nom_FromUnsignedLongLong(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from size")
    {
        NomValue value = Nom_FromSize(4);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from float")
    {
        NomValue value = Nom_FromFloat(4.321f);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.321f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == (double)4.321f);
        }
    }

    SECTION("Creating from double")
    {
        NomValue value = Nom_FromDouble(4.321);

        CHECK(Nom_IsNumber(value) == true);

        SECTION("Converting to int")
        {
            CHECK(Nom_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            CHECK(Nom_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            CHECK(Nom_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            CHECK(Nom_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            CHECK(Nom_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            CHECK(Nom_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            CHECK(Nom_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            CHECK(Nom_AsFloat(value) == 4.321f);
        }

        SECTION("Converting to double")
        {
            CHECK(Nom_AsDouble(value) == 4.321);
        }
    }
}

TEST_CASE("Calling Nom_IsNumber() on non-number Nominal values", "[Number]")
{
    NomState* state = Nom_NewState();

    SECTION("Checking nil")
    {
        NomValue value = Nom_Nil();
        CHECK(Nom_IsNumber(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = Nom_NewString(state, "Testing...", false);
        CHECK(Nom_IsNumber(value) == false);
    }

    SECTION("Checking interned string")
    {
        NomValue value = Nom_NewString(state, "Testing...", true);
        CHECK(Nom_IsNumber(value) == false);
    }

    SECTION("Checking map")
    {
        NomValue value = Nom_NewMap(state);
        CHECK(Nom_IsNumber(value) == false);
    }

    Nom_FreeState(state);
}

TEST_CASE("Performing arithmetic operations on Nominal number values", "[Number]")
{
    NomState* state = Nom_NewState();

    SECTION("Addition")
    {
        NomValue a = Nom_FromInt(3);
        NomValue b = Nom_FromInt(2);
        NomValue c = Nom_Add(state, a, b);
        CHECK(Nom_IsNumber(c) == true);
        CHECK(Nom_AsDouble(c) == 5.0);
    }

    SECTION("Subtraction")
    {
        NomValue a = Nom_FromInt(3);
        NomValue b = Nom_FromInt(2);
        NomValue c = Nom_Subtract(state, a, b);
        CHECK(Nom_IsNumber(c) == true);
        CHECK(Nom_AsDouble(c) == 1.0);
    }

    SECTION("Multiplication")
    {
        NomValue a = Nom_FromInt(3);
        NomValue b = Nom_FromInt(2);
        NomValue c = Nom_Multiply(state, a, b);
        CHECK(Nom_IsNumber(c) == true);
        CHECK(Nom_AsDouble(c) == 6.0);
    }

    SECTION("Division")
    {
        NomValue a = Nom_FromInt(6);
        NomValue b = Nom_FromInt(2);
        NomValue c = Nom_Divide(state, a, b);
        CHECK(Nom_IsNumber(c) == true);
        CHECK(Nom_AsDouble(c) == 3.0);
    }

    Nom_FreeState(state);
}

TEST_CASE("Calling Nom_IsIterable() a Nominal number", "[Number]")
{
    NomState* state = Nom_NewState();

    NomValue number = Nom_FromDouble(1.0);
    CHECK(Nom_IsIterable(state, number) == false);

    Nom_FreeState(state);
}