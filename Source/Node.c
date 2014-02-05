#include "Node.h"

#include <stdlib.h>

Node* Node_Create(NodeType type, Node* first, Node* second, NodeData data)
{
    Node* node;
    node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->first = first;
    node->second = second;
    node->data = data;
    return node;
}

void Node_Free(Node* node)
{
    if (node->first)
    {
        Node_Free(node->first);
    }

    if (node->second)
    {
        Node_Free(node->second);
    }

    free(node);
}