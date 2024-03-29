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

TEST_FILE("Tests/Positive/Arithmetic.ns")
TEST_FILE("Tests/Positive/ClassCreation.ns")
TEST_FILE("Tests/Positive/ClassGetAndSet.ns")
TEST_FILE("Tests/Positive/Comments.ns")
TEST_FILE("Tests/Positive/Fibonacci.ns")
TEST_FILE("Tests/Positive/Functions.ns")
TEST_FILE("Tests/Positive/GetIntrinsicClass.ns")
TEST_FILE("Tests/Positive/If.ns")
TEST_FILE("Tests/Positive/Map.ns")
TEST_FILE("Tests/Positive/Objects.ns")
TEST_FILE("Tests/Positive/ObjectConstructors.ns")
TEST_FILE("Tests/Positive/OverloadArithmetic.ns")
TEST_FILE("Tests/Positive/ShortCircuitAnd.ns")
TEST_FILE("Tests/Positive/ShortCircuitOr.ns")
TEST_FILE("Tests/Positive/ToString.ns")
TEST_FILE("Tests/Positive/While.ns")
