#ifndef NOMINAL_VALUE_H
#define NOMINAL_VALUE_H

///
/// \brief Enumeration of each type a Nominal value can be.
typedef enum
{
    NOM_TYPE_NIL,
    NOM_TYPE_INTEGER,
    NOM_TYPE_REAL,
    NOM_TYPE_BOOLEAN,
    NOM_TYPE_STRING,
    NOM_TYPE_STATIC_STRING,
    NOM_TYPE_MAP,
    NOM_TYPE_FUNCTION
} NomType;

///
/// \brief A Nominal value.
typedef struct
{
    char type;
    union
    {
        int integerValue;
        float realValue;
        unsigned handle;
    } data;
} NomValue;

///
/// \brief The constant value for nil.
NomValue NOM_NIL;

///
/// \brief Checks if a Nominal value is an integer.
///
/// \param value
///     The value in question.
///
/// \returns 1 if the value is an integer; 0 otherwise.
int NomInteger_Check(NomValue value);

///
/// \brief Constructs a Nominal integer value from an int.
///
/// \param value
///     The value.
///
/// \returns The Nominal integer value.
NomValue NomInteger_FromInt(int value);

///
/// \brief Constructs a Nominal integer value from an unsigned long long.
///
/// \param value
///     The value.
///
/// \returns The Nominal integer value.
NomValue NomInteger_FromUnsignedLongLong(unsigned long long value);

///
/// \brief Checks if a Nominal value is a real.
///
/// \param value
///     The value in question.
///
/// \returns 1 if the value is a real; 0 otherwise.
int NomReal_Check(NomValue value);

///
/// \brief Constructs a Nominal real value from a float.
///
/// \param value
///     The value.
///
/// \returns The Nominal real value.
NomValue NomReal_FromFloat(float value);

///
/// \brief Constructs a Nominal real value from a double.
///
/// \param value
///     The value.
///
/// \returns The Nominal real value.
NomValue NomReal_FromDouble(double value);

///
/// \brief Checks whether a Nominal value is a real or integer.
///
/// \param value
///     The value in question.
///
/// \returns 1 if the value is a real or integer; 0 otherwise.
int NomValue_IsNumber(NomValue value);

///
/// \brief Returns the Nominal value coerced to an int.
///
/// \returns The resulting value.
int NomValue_AsInt(NomValue value);

///
/// \brief Returns the Nominal value coerced to a float.
///
/// \returns The resulting value.
float NomValue_AsFloat(NomValue value);

///
/// \brief Returns the Nominal value coerced to a double.
///
/// \returns The resulting value.
double NomValue_AsDouble(NomValue value);

#endif