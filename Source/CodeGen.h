#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>

typedef struct Node Node;

size_t GenerateCode(Node* node, unsigned char* bc, size_t i);

#endif