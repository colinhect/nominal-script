#ifndef BYTECODE_H
#define BYTECODE_H

typedef enum
{
    OP_LET,
    OP_SET,
    OP_GET,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
    OP_EQ,
    OP_NE,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_ASSOC,
    OP_RET,
    OP_PUSH
} op_code;

extern const int OP_PREC[];
extern const char* const OP_STR[];

#endif