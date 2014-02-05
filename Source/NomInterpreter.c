#include "NomInterpreter.h"

#include "Parser.h"
#include "CodeGen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

NomInterpreter* NomInterpreter_Create()
{
    NomInterpreter* interpreter;
    interpreter = (NomInterpreter*)malloc(sizeof(NomInterpreter));
    interpreter->sp = 0;
    interpreter->pc = 0;
    interpreter->errorFlag = 0;
    return interpreter;
}

void NomInterpreter_Free(NomInterpreter* interpreter)
{
    free(interpreter);
}

void SetInterpreterError(NomInterpreter* interpreter, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(interpreter->error, fmt, args);
    va_end(args);
    interpreter->errorFlag = 1;
}

#define POP()       interpreter->st[--interpreter->sp]
#define PUSH(v)     interpreter->st[interpreter->sp++] = v
#define READAS(t)   res = *(t*)&interpreter->bc[interpreter->pc]; interpreter->pc += sizeof(t)

#define ARITH(l, r, op, name)\
    if (!NomValue_IsNumber(l) || !NomValue_IsNumber(r))\
    {\
        SetInterpreterError(interpreter, "Cannot %s non-numeric values", name);\
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
        SetInterpreterError(interpreter, "Cannot negate a non-numeric value");\
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

int NomInterpreter_Execute(NomInterpreter* interpreter, const char* source)
{
    Parser* p;
    Node*   node;

    interpreter->errorFlag = 0;

    p = CreateParser(source);
    node = ParseExpr(p);

    if (!node)
    {
        SetInterpreterError(interpreter, p->error);
    }
    else
    {
        size_t end = GenerateCode(node, interpreter->bc, interpreter->pc);
        FreeNode(node);

        while (interpreter->pc < end && !interpreter->errorFlag)
        {
            NomValue l;
            NomValue r;
            NomValue res;

            op_code op = (op_code)interpreter->bc[interpreter->pc++];
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

    return !interpreter->errorFlag;
}

NomValue NomInterpreter_Pop(NomInterpreter* interpreter)
{
    if (interpreter->sp == 0)
    {
        return NOM_NIL;
    }
    else
    {
        return POP();
    }
}

const char* NomInterpreter_Error(NomInterpreter* interpreter)
{
    return interpreter->error;
}

void NomValue_AsString(NomInterpreter* interpreter, char* dest, NomValue value)
{
    interpreter;

    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
        sprintf(dest, "%d", NomValue_AsInt(value));
        break;
    case NOM_TYPE_REAL:
        sprintf(dest, "%f", NomValue_AsDouble(value));
        break;
    case NOM_TYPE_NIL:
        sprintf(dest, "nil");
        break;
    default:
        sprintf(dest, "<unknown>");
        break;
    }
}