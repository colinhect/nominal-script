#pragma once

#include "NomValue.h"

typedef struct
{
    NomValue st[1024];
    size_t sp;
    unsigned char bc[4096];
    size_t pc;
    char error[1024];
    int errorFlag;
} NomInterpreter;

NomInterpreter* NomCreateInterpreter();
void NomFreeInterpreter(NomInterpreter* interpreter);

int NomExecute(NomInterpreter* interpreter, const char* source);

NomValue NomPop(NomInterpreter* interpreter);
const char* NomError(NomInterpreter* interpreter);

NomValue NomGet(NomInterpreter* interpreter, const char* identifier);

void NomValueToString(NomInterpreter* interpreter, char* dest, NomValue value);