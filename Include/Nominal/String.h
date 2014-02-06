#ifndef NOMINAL_STRING_H
#define NOMINAL_STRING_H

#include "Nominal/State.h"
#include "Nominal/Value.h"

NomValue NomString_FromString(NomState* s, const char* string);
const char* NomString_AsString(NomState* s, NomValue value);

#endif