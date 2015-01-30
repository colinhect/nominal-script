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
#include "Nominal.h"

#include "CodeGen.h"
#include "Node.h"
#include "State.h"
#include "String.h"
#include "StringPool.h"

// Maps operators to their op code
const OpCode OP_OPCODE[] =
{
    OPCODE_LET,   // OP_LET
    OPCODE_SET,   // OP_SET
    OPCODE_ADD,   // OP_ADD
    OPCODE_SUB,   // OP_SUB
    OPCODE_MUL,   // OP_MUL
    OPCODE_DIV,   // OP_DIV
    OPCODE_NEG,   // OP_NEG
    OPCODE_EQ,    // OP_EQ
    OPCODE_NE,    // OP_NE
    OPCODE_GT,    // OP_GT
    OPCODE_GTE,   // OP_GTE
    OPCODE_LT,    // OP_LT
    OPCODE_LTE,   // OP_LTE
    OPCODE_AND,   // OP_AND
    OPCODE_OR,    // OP_OR
    OPCODE_NOT,   // OP_NOT
    OPCODE_ASSOC, // OP_ASSOC
    OPCODE_RET    // OP_RET
};

#define OPCODE(op)      byteCode[index++] = (unsigned char)op
#define WRITEAS(t, v)   *(t*)&byteCode[index] = v; index += sizeof(t)

size_t GenerateCode(
    NomState*       state,
    Node*           node,
    unsigned char*  byteCode,
    size_t          index
    )
{
    switch (node->type)
    {
    case NODE_NIL:
        OPCODE(OPCODE_PUSH);
        WRITEAS(NomValue, NomValue_Nil());
        break;
    case NODE_NUMBER:
        OPCODE(OPCODE_PUSH);
        WRITEAS(NomValue, NomNumber_FromDouble(node->data.number.value));
        break;
    case NODE_STRING:
        OPCODE(OPCODE_PUSH);
        WRITEAS(NomValue, NomString_FromId(node->data.string.id));
        break;
    case NODE_MAP:
    {
        // Push all map items on the stack
        size_t itemCount = 0;
        while (node)
        {
            Node* assoc = node->data.map.assoc;

            if (assoc)
            {
                // Value on stack
                Node* rightExpr = assoc->data.binary.rightExpr;
                index = GenerateCode(state, rightExpr, byteCode, index);

                // Key on stack
                Node* leftExpr = assoc->data.binary.leftExpr;
                index = GenerateCode(state, leftExpr, byteCode, index);

                node = node->data.map.next;
                ++itemCount;
            }
            else
            {
                // No more key/value pairs
                break;
            }
        }

        // Create the map
        OPCODE(OPCODE_NEW_MAP);
        WRITEAS(size_t, itemCount);
    } break;
    case NODE_IDENT:
        OPCODE(OPCODE_GET);
        WRITEAS(StringId, node->data.ident.id);
        break;
    case NODE_UNARY:
        index = GenerateCode(state, node->data.unary.expr, byteCode, index);
        OPCODE(OP_OPCODE[node->data.unary.op]);
        break;
    case NODE_INDEX:
        index = GenerateCode(state, node->data.index.expr, byteCode, index);
        index = GenerateCode(state, node->data.index.key, byteCode, index);
        if (node->data.index.bracket)
        {
            OPCODE(OPCODE_BRACKET_GET);
        }
        else
        {
            OPCODE(OPCODE_VALUE_GET);
        }
        break;
    case NODE_BINARY:
    {
        OpCode op = (OpCode)node->data.binary.op;

        Node* leftExpr = node->data.binary.leftExpr;
        Node* rightExpr = node->data.binary.rightExpr;

        // Push right hand side on stack
        index = GenerateCode(state, rightExpr, byteCode, index);

        if (op == OPCODE_LET || op == OPCODE_SET)
        {
            if (leftExpr->type == NODE_INDEX)
            {
                index = GenerateCode(state, leftExpr->data.index.expr, byteCode, index);
                index = GenerateCode(state, leftExpr->data.index.key, byteCode, index);

                if (op == OPCODE_SET)
                {
                    bool bracket = leftExpr->data.index.bracket;
                    if (bracket)
                    {
                        OPCODE(OPCODE_BRACKET_SET);
                    }
                    else
                    {
                        OPCODE(OPCODE_VALUE_SET);
                    }
                }
                else
                {
                    OPCODE(OPCODE_VALUE_LET);
                }
            }
            else
            {
                // Perform set
                OPCODE(OP_OPCODE[op]);
                WRITEAS(StringId, leftExpr->data.ident.id);
            }
        }
        else
        {
            index = GenerateCode(state, leftExpr, byteCode, index);
            OPCODE(OP_OPCODE[op]);
        }
    } break;
    case NODE_SEQUENCE:
    {
        while (node)
        {
            index = GenerateCode(state, node->data.sequence.expr, byteCode, index);
            node = node->data.sequence.next;

            // Pop the result of that expression off of the stack if there is
            // another expression in the sequence
            if (node)
            {
                OPCODE(OPCODE_POP);
            }
        }
    } break;
    case NODE_CLOSURE:
    {
        // Go to the end of the closure body
        OPCODE(OPCODE_GOTO);
        size_t gotoIndex = index;
        WRITEAS(size_t, 0); // This will be known once the closure code is generator

        // Generate the code for the closure body
        size_t closureIndex = index;
        size_t endIndex = GenerateCode(state, node->data.closure.exprs, byteCode, index);

        // Go back to the goto
        index = gotoIndex;
        WRITEAS(size_t, endIndex); // Update the idnex with the write index
        index = endIndex;

        // Create the closure given the index where it starts
        OPCODE(OPCODE_NEW_CLOSURE);
        WRITEAS(size_t, closureIndex);
    } break;
    case NODE_INVOCATION:
        break;
    }

    return index;
}
