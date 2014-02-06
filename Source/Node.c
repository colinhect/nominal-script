#include "Node.h"

#include <stdlib.h>

Node* Node_Create(NodeType type, Node* first, Node* second, NodeData data)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->type = type;
    n->first = first;
    n->second = second;
    n->data = data;
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

    if (n->type == NODE_STRING)
    {
        free(n->data.stringValue);
    }

    free(n);
}