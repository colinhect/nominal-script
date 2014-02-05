#ifndef NOMSTRING_H
#define NOMSTRING_H

#include "NomState.h"
#include "NomValue.h"

NomValue NomString_FromString(NomState* s, const char* string);
const char* NomString_AsString(NomState* s, NomValue value);

#endif