#include "Nominal/Value.h"

#include "CodeGen.h"
#include "ByteCode.h"
#include "Node.h"
#include "String.h"
#include "StringPool.h"

#define OPCODE(op)  byteCode[index++] = (unsigned char)op
#define VALUE(v)    *(NomValue*)&byteCode[index] = v; index += sizeof(NomValue)

size_t GenerateCode(Node* node, StringPool* stringPool, unsigned char* byteCode, size_t index)
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
    case NODE_STRING:
        {
            StringId id = StringPool_InsertOrFind(stringPool, node->data.stringValue); 
            OPCODE(OP_PUSH);
            VALUE(NomString_FromId(id));
        } break;
    case NODE_UNARY_OP:
        index = GenerateCode(node->first, stringPool, byteCode, index);
        OPCODE(node->data.integerValue);
        break;
    case NODE_BINARY_OP:
        index = GenerateCode(node->second, stringPool, byteCode, index);
        index = GenerateCode(node->first, stringPool, byteCode, index);
        OPCODE(node->data.integerValue);
        break;
    }
    return index;
}