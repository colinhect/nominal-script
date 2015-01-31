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
#ifndef NODE_H
#define NODE_H

#include "StringPool.h"

///
/// \brief A Nominal binary/unary operator
typedef enum
{
    OP_LET,
    OP_SET,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
    OP_EQ,
    OP_NE,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_ASSOC,
    OP_RET
} Operator;

///
/// \brief The precedence of each operator.
extern const int OP_PREC[];

///
/// \brief The string value of each operator.
extern const char* const OP_STR[];

///
/// \brief A type of AST node.
typedef enum
{
    NODE_NIL,
    NODE_NUMBER,
    NODE_STRING,
    NODE_MAP,
    NODE_IDENT,
    NODE_BINARY,
    NODE_UNARY,
    NODE_INDEX,
    NODE_SEQUENCE,
    NODE_CLOSURE,
    NODE_INVOCATION
} NodeType;

///
/// \brief An AST node.
typedef struct Node
{
    NodeType type;
    union
    {
        // A number literal
        struct
        {
            double value;
        } number;

        // A string literal
        struct
        {
            StringId id;
        } string;

        // A map literal (sequence of associations)
        struct
        {
            struct Node* assoc;
            struct Node* next;
        } map;

        // A variable name
        struct
        {
            StringId id;
        } ident;

        // A binary operation applied to two expressions
        struct
        {
            Operator op;
            struct Node* leftExpr;
            struct Node* rightExpr;
        } binary;

        // A unary operation applied to an expression
        struct
        {
            Operator op;
            struct Node* expr;
        } unary;

        // Indexing a value by a key
        struct
        {
            struct Node* expr;
            struct Node* key;
            bool bracket;
        } index;

        // A sequence of expressions
        struct
        {
            struct Node* expr;
            struct Node* next;
        } sequence;

        // A closure
        struct
        {
            struct Node* params;
            struct Node* exprs;
        } closure;

        // An invocation
        struct
        {
            struct Node* expr;
            struct Node* args;
        } invocation;
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
