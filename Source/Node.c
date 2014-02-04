#include "Node.h"

#include <stdlib.h>

Node* CreateNode(NodeType type, Node* first, Node* second, NodeData data)
{
    Node* node;
    node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->first = first;
    node->second = second;
    node->data = data;
    return node;
}

void FreeNode(Node* node)
{
    if (node->first)
    {
        FreeNode(node->first);
    }

    if (node->second)
    {
        FreeNode(node->second);
    }

    free(node);
}