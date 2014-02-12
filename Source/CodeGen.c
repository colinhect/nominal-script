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
#include "Nominal.h"

#include "CodeGen.h"
#include "ByteCode.h"
#include "Node.h"
#include "String.h"
#include "StringPool.h"

#define OPCODE(op)  byteCode[index++] = (unsigned char)op
#define STRING(s)   *(StringId*)&byteCode[index] = s; index += sizeof(StringId)
#define VALUE(v)    *(NomValue*)&byteCode[index] = v; index += sizeof(NomValue)

size_t GenerateCode(
    Node*           node,
    unsigned char*  byteCode,
    size_t          index
    )
{
    switch (node->type)
    {
    case NODE_NIL:
        OPCODE(OP_PUSH);
        VALUE(NomValue_Nil());
        break;
    case NODE_INTEGER:
        OPCODE(OP_PUSH);
        VALUE(NomInteger_FromUnsignedLongLong(node->data.integer.value));
        break;
    case NODE_REAL:
        OPCODE(OP_PUSH);
        VALUE(NomReal_FromDouble(node->data.real.value));
        break;
    case NODE_STRING:
        OPCODE(OP_PUSH);
        VALUE(NomString_FromId(node->data.string.id));
        break;
    case NODE_MAP:
        {
            size_t itemCount = 0;
            while (node)
            {
                Node* assoc = node->data.map.node;

                // Value on stack
                index = GenerateCode(assoc->data.binary.right, byteCode, index);

                // Key on stack
                index = GenerateCode(assoc->data.binary.left, byteCode, index);

                node = node->data.map.next;
                ++itemCount;
            }
            OPCODE(OP_PUSH);
            VALUE(NomInteger_FromUnsignedLongLong(itemCount));
            OPCODE(OP_NEW_MAP);
        } break;
    case NODE_IDENT:
        OPCODE(OP_GET);
        STRING(node->data.ident.id);
        break;
    case NODE_UNARY:
        index = GenerateCode(node->data.unary.node, byteCode, index);
        OPCODE(node->data.unary.op);
        break;
    case NODE_BINARY:
        {
            OpCode op = node->data.binary.op;

            // Push right hand side on stack
            index = GenerateCode(node->data.binary.right, byteCode, index);

            if (op == OP_LET || op == OP_SET)
            {
                // Perform set
                OPCODE(op);
                STRING(node->data.binary.left->data.ident.id);
            }
            else
            {
                index = GenerateCode(node->data.binary.left, byteCode, index);
                OPCODE(op);
            }
        }
        break;
    case NODE_SEQUENCE:
        while (node)
        {
            index = GenerateCode(node->data.sequence.node, byteCode, index);
            node = node->data.sequence.next;
        }
        break;
    }

    return index;
}