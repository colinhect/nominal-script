#include "NomState.h"
#include "NomString.h"

#include "State.h"
#include "Parser.h"
#include "CodeGen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

NomState* NomState_Create()
{
    NomState* s;
    s = (NomState*)malloc(sizeof(NomState));
    s->sp = 0;
    s->ip = 0;
    s->errorFlag = 0;
    s->heap = Heap_Create();
    return s;
}

void NomState_Free(NomState* s)
{
    Heap_Free(s->heap);
    free(s);
}

void NomState_SetError(NomState* s, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(s->error, fmt, args);
    va_end(args);
    s->errorFlag = 1;
}

#define POP()       s->stack[--s->sp]
#define PUSH(v)     s->stack[s->sp++] = v
#define READAS(t)   res = *(t*)&s->byteCode[s->ip]; s->ip += sizeof(t)

#define ARITH(l, r, op, name)\
    if (!NomValue_IsNumber(l) || !NomValue_IsNumber(r))\
    {\
        NomState_SetError(s, "Cannot %s non-numeric values", name);\
        break;\
    }\
    else if (NomReal_Check(l) || NomReal_Check(r))\
    {\
        res = NomReal_FromFloat(NomValue_AsFloat(l) op NomValue_AsFloat(r));\
    }\
    else\
    {\
        res = NomInteger_FromInt(NomValue_AsInt(l) op NomValue_AsInt(r));\
    }

#define NEG(v)\
    if (!NomValue_IsNumber(v))\
    {\
        NomState_SetError(s, "Cannot negate a non-numeric value");\
        break;\
    }\
    else if (NomReal_Check(v))\
    {\
        res = NomReal_FromFloat(-NomValue_AsFloat(v));\
    }\
    else\
    {\
        res = NomInteger_FromInt(-NomValue_AsInt(v));\
    }

int NomState_Execute(NomState* s, const char* source)
{
    Parser* p;
    Node*   node;

    s->errorFlag = 0;

    p = Parser_Create(source);
    node = Parser_Expr(p);

    if (!node)
    {
        NomState_SetError(s, Parser_GetError(p));
    }
    else
    {
        size_t end = GenerateCode(node, s->byteCode, s->ip);
        Node_Free(node);

        while (s->ip < end && !s->errorFlag)
        {
            NomValue l;
            NomValue r;
            NomValue res;

            op_code op = (op_code)s->byteCode[s->ip++];
            switch (op)
            {
            case OP_PUSH:
                READAS(NomValue);
                PUSH(res);                
                break;
            case OP_ADD:
                l = POP();
                r = POP();
                ARITH(l, r, +, "add");
                PUSH(res);
                break;
            case OP_SUB:
                l = POP();
                r = POP();
                ARITH(l, r, -, "subtract");
                PUSH(res);
                break;
            case OP_MUL:
                l = POP();
                r = POP();
                ARITH(l, r, *, "multiply");
                PUSH(res);
                break;
            case OP_DIV:
                l = POP();
                r = POP();
                ARITH(l, r, /, "divide");
                PUSH(res);
                break;
            case OP_NEG:
                l = POP();
                NEG(l);
                PUSH(res);
                break;
            }
        }
    }

    return !s->errorFlag;
}

NomValue NomState_Pop(NomState* s)
{
    if (s->sp == 0)
    {
        return NOM_NIL;
    }
    else
    {
        return POP();
    }
}

const char* NomState_Error(NomState* s)
{
    return s->error;
}

void NomValue_AsString(NomState* s, char* dest, NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
        sprintf(dest, "%d", NomValue_AsInt(value));
        break;
    case NOM_TYPE_REAL:
        sprintf(dest, "%f", NomValue_AsDouble(value));
        break;
    case NOM_TYPE_STRING:
        sprintf(dest, "%s", NomString_AsString(s, value));
        break;
    case NOM_TYPE_NIL:
        sprintf(dest, "nil");
        break;
    default:
        sprintf(dest, "<unknown>");
        break;
    }
}