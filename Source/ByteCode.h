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
#ifndef BYTECODE_H
#define BYTECODE_H

///
/// \brief A Nominal byte code operation.
typedef enum
{
    OP_LET,
    OP_SET,
    OP_GET,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
    OP_EQ,
    OP_NE,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_ASSOC,
    OP_RET,
    OP_PUSH,
    OP_MAP
} OpCode;

///
/// \brief The precedence of each operator.
extern const int OP_PREC[];

///
/// \brief The string value of each operator.
extern const char* const OP_STR[];

///
/// \brief The string value of each op code.
extern const char* const OP_CODE_STR[];

#endif