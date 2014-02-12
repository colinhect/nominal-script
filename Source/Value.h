///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2014 Colin Hill
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

#define SET_VALUE_BITS(offset, count, value, bits) \
    { \
    uint64_t mask = ((((uint64_t)1 << count) - 1) << offset); \
    value.data = (((uint64_t)bits << offset) & mask) | (value.data & ~mask); \
    }

#define GET_VALUE_BITS(offset, count, value) \
    ((value.data & ((((uint64_t)1 << count) - 1) << offset)) >> offset)

#define SET_TYPE_BITS(value, bits)      SET_VALUE_BITS(61, 3, value, bits)
#define GET_TYPE_BITS(value)            GET_VALUE_BITS(61, 3, value)

#define SET_INTEGER_BITS(value, bits)   SET_VALUE_BITS(0, 61, value, bits)
#define GET_INTEGER_BITS(value)         ((int64_t)GET_VALUE_BITS(0, 61, value))

#define SET_REAL_BITS(value, bits)      SET_VALUE_BITS(0, 32, value, bits)
#define GET_REAL_BITS(value)            ((uint64_t)GET_VALUE_BITS(0, 32, value))

#define SET_ID_BITS(value, bits)        SET_VALUE_BITS(0, 48, value, bits)
#define GET_ID_BITS(value)              ((uint64_t)GET_VALUE_BITS(0, 48, value))

#endif