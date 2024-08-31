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

#define TEST_FILE(path) \
    TEST_CASE(#path, "[Positive]")\
    {\
        NomState* state = nom_newstate();\
        nom_dofile(state, path);\
        bool error = nom_error(state);\
        if (error)\
        {\
            INFO(nom_geterror(state));\
            CHECK(!error);\
        }\
        else\
        {\
            NomValue completed = nom_getvar(state, "completed");\
            CHECK(nom_istrue(state, completed));\
        }\
        nom_freestate(state);\
    }

TEST_FILE("tests/positive/arithmetic.ns")
TEST_FILE("tests/positive/class_creation.ns")
TEST_FILE("tests/positive/class_get_and_set.ns")
TEST_FILE("tests/positive/comments.ns")
TEST_FILE("tests/positive/fibonacci.ns")
TEST_FILE("tests/positive/functions.ns")
TEST_FILE("tests/positive/get_intrinsic_class.ns")
TEST_FILE("tests/positive/if.ns")
TEST_FILE("tests/positive/import.ns")
TEST_FILE("tests/positive/map.ns")
TEST_FILE("tests/positive/objects.ns")
TEST_FILE("tests/positive/object_constructors.ns")
TEST_FILE("tests/positive/overload_arithmetic.ns")
TEST_FILE("tests/positive/short_circuit_and.ns")
TEST_FILE("tests/positive/short_circuit_or.ns")
TEST_FILE("tests/positive/to_string.ns")
TEST_FILE("tests/positive/while.ns")
