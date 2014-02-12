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
        if (node->data.map.node)
        {
            Node_Free(node->data.map.node);
        }
        if (node->data.map.next)
        {
            Node_Free(node->data.map.next);
        }
        break;
    case NODE_BINARY:
        if (node->data.binary.left)
        {
            free(node->data.binary.left);
        }
        if (node->data.binary.right)
        {
            free(node->data.binary.right);
        }
        break;
    case NODE_UNARY:
        if (node->data.unary.node)
        {
            free(node->data.unary.node);
        }
        break;
    case NODE_SEQUENCE:
        if (node->data.sequence.node)
        {
            Node_Free(node->data.sequence.node);
        }
        if (node->data.sequence.next)
        {
            Node_Free(node->data.sequence.next);
        }
        break;
    }

    free(node);
}