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
#include "Node.h"

#include <assert.h>
#include <stdlib.h>

Node* Node_Create(
    NodeType    type
    )
{
    Node* node = (Node*)malloc(sizeof(Node));
    assert(node);

    memset(node, 0, sizeof(Node));
    node->type = type;
    return node;
}

void Node_Free(
    Node*   node
    )
{
    assert(node);

    switch (node->type)
    {
    case NODE_NIL:
    case NODE_NUMBER:
    case NODE_STRING:
        break;
    case NODE_MAP:
        if (node->data.map.assoc)
        {
            Node_Free(node->data.map.assoc);
        }
        if (node->data.map.next)
        {
            Node_Free(node->data.map.next);
        }
        break;
    case NODE_IDENT:
        break;
    case NODE_BINARY:
        if (node->data.binary.leftExpr)
        {
            Node_Free(node->data.binary.leftExpr);
        }
        if (node->data.binary.rightExpr)
        {
            Node_Free(node->data.binary.rightExpr);
        }
        break;
    case NODE_UNARY:
        if (node->data.unary.expr)
        {
            Node_Free(node->data.unary.expr);
        }
        break;
    case NODE_INDEX:
        if (node->data.index.expr)
        {
            Node_Free(node->data.index.expr);
        }
        if (node->data.index.key)
        {
            Node_Free(node->data.index.key);
        }
        break;
    case NODE_SEQUENCE:
        if (node->data.sequence.expr)
        {
            Node_Free(node->data.sequence.expr);
        }
        if (node->data.sequence.next)
        {
            Node_Free(node->data.sequence.next);
        }
        break;
    case NODE_FUNCTION:
        if (node->data.function.params)
        {
            Node_Free(node->data.function.params);
        }
        if (node->data.function.exprs)
        {
            Node_Free(node->data.function.exprs);
        }
        break;
    case NODE_INVOCATION:
        if (node->data.invocation.expr)
        {
            Node_Free(node->data.invocation.expr);
        }
        if (node->data.invocation.args)
        {
            Node_Free(node->data.invocation.args);
        }
        break;
    }

    free(node);
}

const int OP_PREC[] =
{
    1,  // OP_LET
    2,  // OP_SET
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
    1   // OP_RET
};

const char* const OP_STR[] =
{
    ":=", // OP_LET
    "=",  // OP_SET
    "+",  // OP_ADD
    "-",  // OP_SUB
    "*",  // OP_MUL
    "/",  // OP_DIV
    "-",  // OP_NEG
    "==", // OP_EQ
    "!=", // OP_NE
    ">",  // OP_GT
    ">=", // OP_GTE
    "<",  // OP_LT
    "<=", // OP_LTE
    "&&", // OP_AND
    "||", // OP_OR
    "!",  // OP_NOT
    "->", // OP_ASSOC
    "<-"  // OP_RET
};
