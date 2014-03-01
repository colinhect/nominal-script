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
#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>

#include "Node.h"

typedef struct _NomState State;
typedef struct _StringPool StringPool;

///
/// \brief A Nominal byte code operation.
typedef enum
{
    OPCODE_LET,
    OPCODE_SET,
    OPCODE_GET,
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_MUL,
    OPCODE_DIV,
    OPCODE_NEG,
    OPCODE_EQ,
    OPCODE_NE,
    OPCODE_GT,
    OPCODE_GTE,
    OPCODE_LT,
    OPCODE_LTE,
    OPCODE_AND,
    OPCODE_OR,
    OPCODE_NOT,
    OPCODE_ASSOC,
    OPCODE_RET,
    OPCODE_VALUE_LET,
    OPCODE_VALUE_SET,
    OPCODE_VALUE_GET,
    OPCODE_BRACKET_SET,
    OPCODE_BRACKET_GET,
    OPCODE_PUSH,
    OPCODE_POP,
    OPCODE_NEW_MAP
} OpCode;

///
/// \brief Generates Nominal byte code from an AST.
///
/// \param state
///     The state.
/// \param node
///     The root node in the AST.
/// \param byteCode
///     The byte code to write to.
/// \param index
///     The index into the byte code to write to.
///
/// \returns The index in the byte code where the newly generated byte code
/// ends.
size_t GenerateCode(
    NomState*       state,
    Node*           node,
    unsigned char*  byteCode,
    size_t          index
    );

#endif