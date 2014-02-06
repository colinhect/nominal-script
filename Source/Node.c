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

Node* Node_WithoutData(NodeType type, Node* first, Node* second)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->type = type;
    n->data.integerValue = 0;
    n->first = first;
    n->second = second;
    return n;
}

Node* Node_WithHandle(NodeType type, uint32_t value, Node* first, Node* second)
{
    Node* n = Node_WithoutData(type, first, second);
    n->data.handle = value;
    return n;
}

Node* Node_WithInteger(NodeType type, uint64_t value, Node* first, Node* second)
{
    Node* n = Node_WithoutData(type, first, second);
    n->data.integerValue = value;
    return n;
}

Node* Node_WithReal(NodeType type, double value, Node* first, Node* second)
{
    Node* n = Node_WithoutData(type, first, second);
    n->data.realValue = value;
    return n;
}

void Node_Free(Node* n)
{
    if (n->first)
    {
        Node_Free(n->first);
    }

    if (n->second)
    {
        Node_Free(n->second);
    }

    free(n);
}