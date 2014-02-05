#include "NomValue.h"

#include "CodeGen.h"
#include "ByteCode.h"
#include "Node.h"

#define OPCODE(op)      byteCode[index++] = (unsigned char)op
#define VALUE(v)        *(NomValue*)&byteCode[index] = v; index += sizeof(NomValue)

size_t GenerateCode(Node* node, unsigned char* byteCode, size_t index)
{
    switch (node->type)
    {
    case NODE_NIL:
        OPCODE(OP_PUSH);
        VALUE(NOM_NIL);
        break;
    case NODE_INTEGER:
        OPCODE(OP_PUSH);
        VALUE(NomInteger_FromUnsignedLongLong(node->data.integerValue));
        break;
    case NODE_REAL:
        OPCODE(OP_PUSH);
        VALUE(NomReal_FromDouble(node->data.realValue));
        break;
    case NODE_UNARY_OP:
        index = GenerateCode(node->first, byteCode, index);
        OPCODE(node->data.integerValue);
        break;
    case NODE_BINARY_OP:
        index = GenerateCode(node->second, byteCode, index);
        index = GenerateCode(node->first, byteCode, index);
        OPCODE(node->data.integerValue);
        break;
    }
    return index;
}