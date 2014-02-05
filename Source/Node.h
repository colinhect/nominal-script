#ifndef NODE_H
#define NODE_H

#include <stdint.h>

typedef enum
{
    NODE_NIL,
    NODE_INTEGER,
    NODE_REAL,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
} NodeType;

typedef union
{
    uint64_t integerValue;
    double realValue;
} NodeData;

typedef struct _Node
{
    NodeType type;
    struct _Node* first;
    struct _Node* second;
    NodeData data;    
} Node;

Node* Node_Create(NodeType type, Node* first, Node* second, NodeData data);
void Node_Free(Node* node);

#endif