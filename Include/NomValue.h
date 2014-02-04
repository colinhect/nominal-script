#pragma once

#include <stdint.h>

typedef enum
{
    NOM_NIL,
    NOM_INTEGER,
    NOM_FLOAT,
    NOM_BOOLEAN,
    NOM_STRING,
    NOM_MAP,
    NOM_FUNCTION
} NomType;

typedef struct
{
    union
    {
        int intValue;
        float floatValue;
        uint32_t unsignedValue;
    } data;
    NomType type;
} NomValue;

NomValue NomNil();
NomValue NomInteger(int value);
NomValue NomFloat(float value);

int NomValueIsNumber(NomValue value);

int NomValueAsInt(NomValue value);
float NomValueAsFloat(NomValue value);