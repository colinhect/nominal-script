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
#ifndef NODE_H
#define NODE_H

#include <stdint.h>

///
/// \brief A type of AST node.
typedef enum
{
    NODE_NIL,
    NODE_INTEGER,
    NODE_REAL,
    NODE_STRING,
    NODE_IDENT,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
} NodeType;

///
/// \brief An AST node.
typedef struct _Node
{
    NodeType    type;

    union
    {
        uint32_t    handle;
        int64_t     integerValue;
        double      realValue;
    } data;

    struct _Node*   first;
    struct _Node*   second;
} Node;

///
/// \brief Creates a new AST node without data.
///
/// \param type
///     The type of node.
/// \param first
///     The first child node.
/// \param second
///     The second child node.
///
/// \returns The new node.
Node* Node_WithoutData(
    NodeType    type,
    Node*       first,
    Node*       second
    );

///
/// \brief Creates a new AST node with a handle value.
///
/// \param type
///     The type of node.
/// \param value
///     The value.
/// \param first
///     The first child node.
/// \param second
///     The second child node.
///
/// \returns The new node.
Node* Node_WithHandle(
    NodeType    type,
    uint32_t    value,
    Node*       first,
    Node*       second
    );

///
/// \brief Creates a new AST node with an integer value.
///
/// \param type
///     The type of node.
/// \param value
///     The value.
/// \param first
///     The first child node.
/// \param second
///     The second child node.
///
/// \returns The new node.
Node* Node_WithInteger(
    NodeType    type,
    int64_t     value,
    Node*       first,
    Node*       second
    );

///
/// \brief Creates a new AST node with a real value.
///
/// \param type
///     The type of node.
/// \param value
///     The value.
/// \param first
///     The first child node.
/// \param second
///     The second child node.
///
/// \returns The new node.
Node* Node_WithReal(
    NodeType    type,
    double      value,
    Node*       first,
    Node*       second
    );

///
/// \brief Frees a node.
///
/// \param node
///     The node to free.
void Node_Free(
    Node*   node
    );

#endif