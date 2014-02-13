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

#include "ByteCode.h"
#include "StringPool.h"

///
/// \brief A type of AST node.
typedef enum
{
    NODE_NIL,
    NODE_INTEGER,
    NODE_REAL,
    NODE_STRING,
    NODE_MAP,
    NODE_IDENT,
    NODE_BINARY,
    NODE_UNARY,
    NODE_SEQUENCE
} NodeType;

///
/// \brief An AST node.
typedef struct _Node
{
    NodeType type;
    union
    {
        // NODE_INTEGER
        struct
        {
            long long value;
        } integer;

        // NODE_REAL
        struct
        {
            double value;
        } real;

        // NODE_STRING
        struct
        {
            StringId id;
        } string;

        // NODE_MAP (compatable with NODE_SEQUENCE)
        struct
        {
            struct _Node* node;
            struct _Node* next;
        } map;

        // NODE_IDENT
        struct
        {
            StringId id;
        } ident;

        // NODE_BINARY
        struct
        {
            OpCode op;
            struct _Node* left;
            struct _Node* right;
        } binary;

        // NODE_UNARY
        struct
        {
            OpCode op;
            struct _Node* node;
        } unary;

        // NODE_SEQUENCE
        struct
        {
            struct _Node* node;
            struct _Node* next;
        } sequence;
    } data;
} Node;

///
/// \brief Creates a new AST node.
///
/// \param type
///     The node type.
///
/// \returns The new node.
Node* Node_Create(
    NodeType    type
    );

///
/// \brief Frees an AST node.
///
/// \param node
///     The node to free.
void Node_Free(
    Node*   node
    );

#endif