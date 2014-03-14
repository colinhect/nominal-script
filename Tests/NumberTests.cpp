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

TEST_CASE("Creating/converting a Nominal number from/to all supported numeric types", "[Number]")
{
    SECTION("Creating from int")
    {
        NomValue value = NomNumber_FromInt(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned int")
    {
        NomValue value = NomNumber_FromUnsignedInt(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from long")
    {
        NomValue value = NomNumber_FromLong(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned long")
    {
        NomValue value = NomNumber_FromUnsignedLong(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from long long")
    {
        NomValue value = NomNumber_FromLongLong(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from unsigned long long")
    {
        NomValue value = NomNumber_FromUnsignedLongLong(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from size")
    {
        NomValue value = NomNumber_FromSize(4);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.0f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.0);
        }
    }

    SECTION("Creating from float")
    {
        NomValue value = NomNumber_FromFloat(4.321f);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.321f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == (double)4.321f);
        }
    }

    SECTION("Creating from double")
    {
        NomValue value = NomNumber_FromDouble(4.321);

        REQUIRE(NomNumber_Check(value) == true);

        SECTION("Converting to int")
        {
            REQUIRE(NomNumber_AsInt(value) == 4);
        }

        SECTION("Converting to unsigned int")
        {
            REQUIRE(NomNumber_AsUnsignedInt(value) == 4);
        }

        SECTION("Converting to long")
        {
            REQUIRE(NomNumber_AsLong(value) == 4);
        }

        SECTION("Converting to unsigned long")
        {
            REQUIRE(NomNumber_AsUnsignedLong(value) == 4);
        }

        SECTION("Converting to long long")
        {
            REQUIRE(NomNumber_AsLongLong(value) == 4);
        }

        SECTION("Converting to unsigned long long")
        {
            REQUIRE(NomNumber_AsUnsignedLongLong(value) == 4);
        }

        SECTION("Converting to size_t")
        {
            REQUIRE(NomNumber_AsSize(value) == 4);
        }

        SECTION("Converting to float")
        {
            REQUIRE(NomNumber_AsFloat(value) == 4.321f);
        }

        SECTION("Converting to double")
        {
            REQUIRE(NomNumber_AsDouble(value) == 4.321);
        }
    }
}

TEST_CASE("Calling NomNumber_Check on non-number Nominal values", "[Number]")
{
    NomState* state = NomState_Create();

    SECTION("Checking nil")
    {
        NomValue value = NOM_NIL;
        REQUIRE(NomNumber_Check(value) == false);
    }

    SECTION("Checking string")
    {
        NomValue value = NomString_FromString(state, "Testing...", false);
        REQUIRE(NomNumber_Check(value) == false);
    }

    SECTION("Checking pooled string")
    {
        NomValue value = NomString_FromString(state, "Testing...", true);
        REQUIRE(NomNumber_Check(value) == false);
    }

    SECTION("Checking map")
    {
        NomValue value = NomMap_Create(state);
        REQUIRE(NomNumber_Check(value) == false);
    }

    NomState_Free(state);
}

TEST_CASE("Performing arithmetic operations on Nominal number values", "[Number]")
{
    NomState* state = NomState_Create();

    SECTION("Addition")
    {
        NomValue a = NomNumber_FromInt(3);
        NomValue b = NomNumber_FromInt(2);
        NomValue c = NomValue_Add(state, a, b);
        REQUIRE(NomNumber_Check(c) == true);
        REQUIRE(NomNumber_AsDouble(c) == 5.0);
    }

    SECTION("Subtraction")
    {
        NomValue a = NomNumber_FromInt(3);
        NomValue b = NomNumber_FromInt(2);
        NomValue c = NomValue_Subtract(state, a, b);
        REQUIRE(NomNumber_Check(c) == true);
        REQUIRE(NomNumber_AsDouble(c) == 1.0);
    }

    SECTION("Multiplication")
    {
        NomValue a = NomNumber_FromInt(3);
        NomValue b = NomNumber_FromInt(2);
        NomValue c = NomValue_Multiply(state, a, b);
        REQUIRE(NomNumber_Check(c) == true);
        REQUIRE(NomNumber_AsDouble(c) == 6.0);
    }

    SECTION("Division")
    {
        NomValue a = NomNumber_FromInt(6);
        NomValue b = NomNumber_FromInt(2);
        NomValue c = NomValue_Divide(state, a, b);
        REQUIRE(NomNumber_Check(c) == true);
        REQUIRE(NomNumber_AsDouble(c) == 3.0);
    }

    NomState_Free(state);
}
