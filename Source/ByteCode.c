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
#include "ByteCode.h"

const int OP_PREC[] =
{
    1,  // OP_LET
    2,  // OP_SET
    0,  // OP_GET
    40, // OP_ADD
    40, // OP_SUB
    80, // OP_MUL
    80, // OP_DIV
    6,  // OP_NEG
    10, // OP_EQ
    10, // OP_NE
    20, // OP_GT
    20, // OP_GTE
    20, // OP_LT
    20, // OP_LTE
    5,  // OP_AND
    5,  // OP_OR
    6,  // OP_NOT
    1,  // OP_ASSOC
    1,  // OP_RET
    0   // OP_PUSH
};

const char* const OP_STR[] =
{
    ":=", // OP_LET
    "=",  // OP_SET
    " ",  // OP_GET
    "+",  // OP_ADD
    "-",  // OP_SUB
    "*",  // OP_MUL
    "/",  // OP_DIV
    "-",  // OP_NEG
    "==", // OP_EQUAL
    "!=", // OP_NE
    ">",  // OP_GT
    ">=", // OP_GTE
    "<",  // OP_LT
    "<=", // OP_LTE
    "&&", // OP_AND
    "||", // OP_OR
    "!",  // OP_NOT
    "->", // OP_ASSOC
    "<-", // OP_RET
    " ",  // OP_PUSH
};