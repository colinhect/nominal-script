#include "NomValue.h"

#include <float.h>
#include <math.h>

#ifdef _MSC_VER
#pragma warning (disable : 4056)
#define INFINITY (FLT_MAX+FLT_MAX)
#define NAN (INFINITY-INFINITY)
#endif

NomValue NOM_NIL = { NOM_TYPE_NIL, 0 };

NomValue NomInteger_FromInt(int value)
{
    NomValue v;
    v.type = NOM_TYPE_INTEGER;
    v.data.integerValue = value;
    return v;
}

NomValue NomReal_FromFloat(float value)
{
    NomValue v;
    v.type = NOM_TYPE_REAL;
    v.data.realValue = value;
    return v;
}

NomValue NomReal_FromDouble(double value)
{
    NomValue v;
    v.type = NOM_TYPE_REAL;
    v.data.realValue = (float)value;
    return v;
}

int NomValue_IsNumber(NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
    case NOM_TYPE_REAL:
        return 1;
    default:
        return 0;
    }
}

int NomValue_AsInt(NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
        return value.data.integerValue;
    case NOM_TYPE_REAL:
        return (int)value.data.realValue;
    default:
        return -1;
    }
}

float NomValue_AsFloat(NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
        return (float)value.data.integerValue;
    case NOM_TYPE_REAL:
        return value.data.realValue;
    default:
        return NAN;
    }
}

double NomValue_AsDouble(NomValue value)
{
    switch (value.type)
    {
    case NOM_TYPE_INTEGER:
        return (double)value.data.integerValue;
    case NOM_TYPE_REAL:
        return (double)value.data.realValue;
    default:
        return NAN;
    }
}