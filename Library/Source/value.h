///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2015 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#ifndef VALUE_H
#define VALUE_H

#include <nominal.h>

// Enumeration of each type a value can be
typedef enum
{
    VALUETYPE_NIL,
    VALUETYPE_NUMBER,
    VALUETYPE_BOOLEAN,
    VALUETYPE_INTERNED_STRING,
    VALUETYPE_OBJECT
} ValueType;

// Enumeration of each type an object can be
typedef enum
{
    OBJECTTYPE_STRING,
    OBJECTTYPE_MAP,
    OBJECTTYPE_FUNCTION,
} ObjectType;

#define TYPE_MASK       (0x0000000000000007)
#define ID_MASK         (0xFFFFFFFF00000000)
#define QNAN_MASK       (0x000000007FFFFF00)
#define QNAN_VALUE      (0x000000007FF7A500)

#define IS_NUMBER(v)    ((v.raw & QNAN_MASK) != QNAN_VALUE)

#define SET_TYPE(v, t)  (v.data.lower = (TYPE_MASK & t) | (~TYPE_MASK & v.data.lower))
#define GET_TYPE(v)     (IS_NUMBER(v) ? VALUETYPE_NUMBER : (ValueType)(TYPE_MASK & v.data.lower))
#define SET_ID(v, i)    (v.data.upper = (uint32_t)i)
#define GET_ID(v)       (v.data.upper)

// A function for visiting Nominal values
typedef void (*ValueVisitor)(
    NomState*   state,
    NomValue    value
);

// Visits the specified value and all child values
void value_visit(
    NomState*       state,
    NomValue        value,
    ValueVisitor    visitor
);

#endif
