#ifndef NOMINTERPRETER_H
#define NOMINTERPRETER_H

#include "NomValue.h"

///
/// \brief A Nominal state.
typedef struct _NomState NomState;

///
/// \brief Creates a new Nominal state.
///
/// \returns The new Nominal state.
NomState* NomState_Create();

///
/// \brief Frees a Nominal state.
///
/// \param state
///     The state to free.
void NomState_Free(NomState* s);

int NomState_Execute(NomState* s, const char* source);

NomValue NomState_Pop(NomState* s);
const char* NomState_Error(NomState* s);

void NomValue_AsString(NomState* s, char* dest, NomValue value);

#endif