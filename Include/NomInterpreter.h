#ifndef NOMINTERPRETER_H
#define NOMINTERPRETER_H

#include "NomValue.h"

///
/// \brief A Nominal interpreter.
typedef struct
{
    NomValue st[1024];
    size_t sp;
    unsigned char bc[4096];
    size_t pc;
    char error[1024];
    int errorFlag;
} NomInterpreter;

///
/// \brief Creates a new Nominal interpreter.
///
/// \returns The new Nominal interpreter.
NomInterpreter* NomInterpreter_Create();

///
/// \brief Frees a Nominal interpreter.
///
/// \param interpreter
///     The interpreter to free.
void NomInterpreter_Free(NomInterpreter* interpreter);

int NomInterpreter_Execute(NomInterpreter* interpreter, const char* source);

NomValue NomInterpreter_Pop(NomInterpreter* interpreter);
const char* NomInterpreter_Error(NomInterpreter* interpreter);

void NomValue_AsString(NomInterpreter* interpreter, char* dest, NomValue value);

#endif