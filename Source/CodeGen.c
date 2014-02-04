#include "NomValue.h"

#include "CodeGen.h"
#include "ByteCode.h"
#include "Node.h"

#define OPCODE(op)      bc[i++] = (unsigned char)op
#define VALUE(v)        *(NomValue*)&bc[i] = v; i += sizeof(NomValue)

size_t GenerateCode(Node* node, unsigned char* bc, size_t i)
{
    switch (node->type)
    {
    case NODE_NIL:
        OPCODE(OP_PUSH);
        VALUE(NOM_NIL);
        break;
    case NODE_INTEGER:
        OPCODE(OP_PUSH);
        VALUE(NomInteger_FromInt(node->data.integerValue));
        break;
    case NODE_REAL:
        OPCODE(OP_PUSH);
        VALUE(NomReal_FromDouble(node->data.realValue));
        break;
    case NODE_UNARY_OP:
        i = GenerateCode(node->first, bc, i);
        OPCODE(node->data.integerValue);
        break;
    case NODE_BINARY_OP:
        i = GenerateCode(node->second, bc, i);
        i = GenerateCode(node->first, bc, i);
        OPCODE(node->data.integerValue);
        break;
    }
    return i;
}