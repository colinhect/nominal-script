#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>

typedef struct _Node Node;

size_t GenerateCode(Node* node, unsigned char* byteCode, size_t index);

#endif