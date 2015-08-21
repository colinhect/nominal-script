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
#include "codegen.h"

// Emits an opcode value to the byte code array
#define OPCODE(op)\
    bytecode[index++] = (unsigned char)op

// Emits a raw value to the byte code array
#define WRITEAS(t, v)\
    *(t*)&bytecode[index] = v; index += sizeof(t)

uint32_t generatecode(
    NomState*       state,
    Node*           node,
    unsigned char*  bytecode,
    uint32_t        index
)
{
    switch (node->type)
    {
    case NODE_NUMBER:
        OPCODE(OPCODE_PUSH);
        WRITEAS(NomValue, nom_fromdouble(node->data.number.value));
        break;
    case NODE_STRING:
        OPCODE(OPCODE_PUSH);
        WRITEAS(NomValue, string_newinterned(node->data.string.id));
        break;
    case NODE_MAP:
    {
        // Move to the end of the sequence of key value pairs
        while (node->data.map.next)
        {
            node = node->data.map.next;
        }

        // Push all map items on the stack in reverse order
        uint32_t itemCount = 0;
        while (node)
        {
            Node* assoc = node->data.map.assoc;

            if (assoc)
            {
                // Value on stack
                Node* rightexpr = assoc->data.binary.rightexpr;
                index = generatecode(state, rightexpr, bytecode, index);

                // Key on stack
                Node* leftexpr = assoc->data.binary.leftexpr;
                index = generatecode(state, leftexpr, bytecode, index);

                node = node->data.map.prev;
                ++itemCount;
            }
            else
            {
                // No more key/value pairs
                break;
            }
        }

        // Create the map
        OPCODE(OPCODE_MAP);
        WRITEAS(uint32_t, itemCount);
    }
    break;
    case NODE_IDENT:
        OPCODE(OPCODE_GET);
        WRITEAS(StringId, node->data.ident.id);
        break;
    case NODE_UNARY:
        index = generatecode(state, node->data.unary.expr, bytecode, index);
        OPCODE(OP_OPCODE[node->data.unary.op]);
        break;
    case NODE_INDEX:
        index = generatecode(state, node->data.index.expr, bytecode, index);
        index = generatecode(state, node->data.index.key, bytecode, index);
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

        Node* leftexpr = node->data.binary.leftexpr;
        Node* rightexpr = node->data.binary.rightexpr;

        // Push right hand side on stack
        index = generatecode(state, rightexpr, bytecode, index);

        if (op == OPCODE_LET || op == OPCODE_SET)
        {
            if (leftexpr->type == NODE_INDEX)
            {
                index = generatecode(state, leftexpr->data.index.expr, bytecode, index);
                index = generatecode(state, leftexpr->data.index.key, bytecode, index);

                if (op == OPCODE_SET)
                {
                    bool bracket = leftexpr->data.index.bracket;
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
                WRITEAS(StringId, leftexpr->data.ident.id);
            }
        }
        else
        {
            index = generatecode(state, leftexpr, bytecode, index);
            OPCODE(OP_OPCODE[op]);
        }
    }
    break;
    case NODE_SEQUENCE:
    {
        while (node)
        {
            index = generatecode(state, node->data.sequence.expr, bytecode, index);
            node = node->data.sequence.next;

            // Pop the result of that expression off of the stack if there is
            // another expression in the sequence
            if (node)
            {
                OPCODE(OPCODE_POP);
            }
        }
    }
    break;
    case NODE_FUNCTION:
    {
        // Go to the end of the function body
        OPCODE(OPCODE_GOTO);
        uint32_t gotoIndex = index;
        WRITEAS(uint32_t, 0); // This will be known once the function code is generated

        // Remember the instruction pointer where the function begins
        uint32_t ip = index;

        // Generate the code for the function body
        index = generatecode(state, node->data.function.exprs, bytecode, index);
        OPCODE(OPCODE_RET);

        // Remember the instruction pointer where the function ends
        uint32_t endIndex = index;

        // Re-write the goto instruction pointer
        index = gotoIndex;
        WRITEAS(uint32_t, endIndex);
        index = endIndex;

        // Create the function
        OPCODE(OPCODE_FUNCTION);
        WRITEAS(uint32_t, ip);
        uint32_t paramCountIndex = index;
        WRITEAS(uint32_t, 0); // This will be known once the parameters are traversed

        uint32_t paramcount = 0;

        // Emit the parameter names
        Node* param = node->data.function.params;
        while (param)
        {
            Node* paramExpr = param->data.sequence.expr;
            if (paramExpr)
            {
                WRITEAS(StringId, paramExpr->data.string.id);
                param = param->data.sequence.next;
                ++paramcount;
            }
            else
            {
                break;
            }
        }

        // Remember the instruction pointer where the parameters end
        endIndex = index;

        // Re-write the parameter count
        index = paramCountIndex;
        WRITEAS(uint32_t, paramcount);
        index = endIndex;
    }
    break;
    case NODE_INVOCATION:
    {
        uint32_t argcount = 0;

        // Push the arguments
        Node* arg = node->data.invocation.args;
        while (arg)
        {
            Node* argExpr = arg->data.sequence.expr;
            if (argExpr)
            {
                // Generate the code to push the argument on the stack
                index = generatecode(state, argExpr, bytecode, index);
                arg = arg->data.sequence.next;
                ++argcount;
            }
            else
            {
                break;
            }
        }

        // Generate the code to push the function on the stack
        index = generatecode(state, node->data.invocation.expr, bytecode, index);

        // Invoke the function
        OPCODE(OPCODE_INVOKE);
        WRITEAS(uint32_t, argcount);
    }
    break;
    }

    return index;
}

const OpCode OP_OPCODE[] =
{
    OPCODE_LET,     // OP_LET
    OPCODE_SET,     // OP_SET
    OPCODE_ADD,     // OP_ADD
    OPCODE_SUB,     // OP_SUB
    OPCODE_MUL,     // OP_MUL
    OPCODE_DIV,     // OP_DIV
    OPCODE_NEG,     // OP_NEG
    OPCODE_EQ,      // OP_EQ
    OPCODE_NE,      // OP_NE
    OPCODE_GT,      // OP_GT
    OPCODE_GTE,     // OP_GTE
    OPCODE_LT,      // OP_LT
    OPCODE_LTE,     // OP_LTE
    OPCODE_AND,     // OP_AND
    OPCODE_OR,      // OP_OR
    OPCODE_NOT,     // OP_NOT
    OPCODE_ASSOC,   // OP_ASSOC
    OPCODE_RET      // OP_RET
};

const char* const OPCODE_NAMES[] =
{
    "LET",          // OPCODE_LET
    "SET",          // OPCODE_SET
    "GET",          // OPCODE_GET
    "ADD",          // OPCODE_ADD
    "SUB",          // OPCODE_SUB
    "MUL",          // OPCODE_MUL
    "DIV",          // OPCODE_DIV
    "NEG",          // OPCODE_NEG
    "EQ",           // OPCODE_EQ
    "NE",           // OPCODE_NE
    "GT",           // OPCODE_GT
    "GTE",          // OPCODE_GTE
    "LT",           // OPCODE_LT
    "LTE",          // OPCODE_LTE
    "AND",          // OPCODE_AND
    "OR",           // OPCODE_OR
    "NOT",          // OPCODE_NOT
    "ASSOC",        // OPCODE_ASSOC
    "RET",          // OPCODE_RET
    "VALUE_LET",    // OPCODE_VALUE_LET
    "VALUE_SET",    // OPCODE_VALUE_SET
    "VALUE_GET",    // OPCODE_VALUE_GET
    "BRACKET_SET",  // OPCODE_BRACKET_SET
    "BRACKET_GET",  // OPCODE_BRACKET_GET
    "PUSH",         // OPCODE_PUSH
    "POP",          // OPCODE_POP
    "MAP",          // OPCODE_MAP
    "FUNCTION",     // OPCODE_FUNCTION
    "GOTO",         // OPCODE_GOTO
    "INVOKE"        // OPCODE_INVOKE
};
