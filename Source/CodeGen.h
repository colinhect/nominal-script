#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>

typedef struct _Node Node;
typedef struct _StringPool StringPool;

size_t GenerateCode(Node* node, StringPool* stringPool, unsigned char* byteCode, size_t index);

#endif