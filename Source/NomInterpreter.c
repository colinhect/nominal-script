#include "../Include/NomInterpreter.h"
#include "Parser.h"
#include "CodeGen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

NomInterpreter* NomCreateInterpreter()
{
    NomInterpreter* interpreter;
    interpreter = (NomInterpreter*)malloc(sizeof(NomInterpreter));
    interpreter->sp = 0;
    interpreter->pc = 0;
    interpreter->errorFlag = 0;
    return interpreter;
}

void NomFreeInterpreter(NomInterpreter* interpreter)
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
    if (!NomValueIsNumber(l) || !NomValueIsNumber(r))\
    {\
        SetInterpreterError(interpreter, "Cannot %s non-numeric values", name);\
        break;\
    }\
    else if (l.type == NOM_FLOAT || r.type == NOM_FLOAT)\
    {\
        res = NomFloat(NomValueAsFloat(l) op NomValueAsFloat(r));\
    }\
    else\
    {\
        res = NomInteger(NomValueAsInt(l) op NomValueAsInt(r));\
    }

#define NEG(v)\
    if (!NomValueIsNumber(v))\
    {\
        SetInterpreterError(interpreter, "Cannot negate a non-numeric value");\
        break;\
    }\
    else if (v.type == NOM_FLOAT)\
    {\
        res = NomFloat(-NomValueAsFloat(v));\
    }\
    else\
    {\
        res = NomInteger(-NomValueAsInt(v));\
    }

int NomExecute(NomInterpreter* interpreter, const char* source)
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

NomValue NomPop(NomInterpreter* interpreter)
{
    if (interpreter->sp == 0)
    {
        return NomNil();
    }
    else
    {
        return POP();
    }
}

const char* NomError(NomInterpreter* interpreter)
{
    return interpreter->error;
}

void NomValueToString(NomInterpreter* interpreter, char* dest, NomValue value)
{
    interpreter;

    switch (value.type)
    {
    case NOM_INTEGER:
        sprintf(dest, "%d", NomValueAsInt(value));
        break;
    case NOM_FLOAT:
        sprintf(dest, "%f", NomValueAsFloat(value));
        break;
    case NOM_NIL:
        sprintf(dest, "nil");
        break;
    default:
        sprintf(dest, "<unknown>");
        break;
    }
}