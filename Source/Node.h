#ifndef NODE_H
#define NODE_H

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
    long integerValue;
    double realValue;
} NodeData;

typedef struct Node
{
    NodeType type;
    struct Node* first;
    struct Node* second;
    NodeData data;
} Node;

Node* CreateNode(NodeType type, Node* first, Node* second, NodeData data);
void FreeNode(Node* node);

#endif