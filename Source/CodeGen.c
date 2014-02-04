#include "CodeGen.h"
#include "ByteCode.h"
#include "Node.h"
#include "../Include/NomValue.h"

#define OPCODE(op)      bc[i++] = (unsigned char)op
#define VALUE(v)        *(NomValue*)&bc[i] = v; i += sizeof(NomValue)

size_t GenerateCode(Node* node, unsigned char* bc, size_t i)
{
    switch (node->type)
    {
    case NODE_NIL:
        OPCODE(OP_PUSH);
        VALUE(NomNil());
        break;
    case NODE_INTEGER:
        OPCODE(OP_PUSH);
        VALUE(NomInteger((int)node->data.intValue));
        break;
    case NODE_FLOAT:
        OPCODE(OP_PUSH);
        VALUE(NomFloat((float)node->data.floatValue));
        break;
    case NODE_UNARY_OP:
        i = GenerateCode(node->first, bc, i);
        OPCODE(node->data.intValue);
        break;
    case NODE_BINARY_OP:
        i = GenerateCode(node->second, bc, i);
        i = GenerateCode(node->first, bc, i);
        OPCODE(node->data.intValue);
        break;
    }
    return i;
}