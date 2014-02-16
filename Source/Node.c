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
#include "Node.h"

#include <stdlib.h>

Node* Node_Create(
    NodeType    type
    )
{
    Node* node = (Node*)malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    node->type = type;
    return node;
}

void Node_Free(
    Node*   node
    )
{
    switch (node->type)
    {
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
    case NODE_BINARY:
        if (node->data.binary.leftExpr)
        {
            free(node->data.binary.leftExpr);
        }
        if (node->data.binary.rightExpr)
        {
            free(node->data.binary.rightExpr);
        }
        break;
    case NODE_UNARY:
        if (node->data.unary.expr)
        {
            free(node->data.unary.expr);
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
    }

    free(node);
}