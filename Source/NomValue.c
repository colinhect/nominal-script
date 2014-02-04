#include "../Include/NomValue.h"

#include <float.h>
#include <math.h>

#ifdef _MSC_VER
#pragma warning (disable : 4056)
#define INFINITY (FLT_MAX+FLT_MAX)
#define NAN (INFINITY-INFINITY)
#endif

NomValue NomNil()
{
    NomValue v;
    v.type = NOM_NIL;
    v.data.intValue = 0;
    return v;
}

NomValue NomInteger(int value)
{
    NomValue v;
    v.type = NOM_INTEGER;
    v.data.intValue = value;
    return v;
}

NomValue NomFloat(float value)
{
    NomValue v;
    v.type = NOM_FLOAT;
    v.data.floatValue = value;
    return v;
}

int NomValueIsNumber(NomValue value)
{
    switch (value.type)
    {
    case NOM_INTEGER:
    case NOM_FLOAT:
        return 1;
    default:
        return 0;
    }
}

int NomValueAsInt(NomValue value)
{
    switch (value.type)
    {
    case NOM_INTEGER:
        return value.data.intValue;
    case NOM_FLOAT:
        return (int)value.data.floatValue;
    default:
        return -1;
    }
}

float NomValueAsFloat(NomValue value)
{
    switch (value.type)
    {
    case NOM_INTEGER:
        return (float)value.data.intValue;
    case NOM_FLOAT:
        return value.data.floatValue;
    default:
        return NAN;
    }
}