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
/// \file
///////////////////////////////////////////////////////////////////////////////
#pragma once

extern "C"
{
#include "nominal.h"
}

#undef bool
#undef false
#undef true

#include <cassert>

namespace nom
{

class Value
{
public:
    friend class State;

    Value() :
        _value(nom_nil())
    {
    }

    bool isObject() const
    {
        bool result = false;
        if (_state)
        {
            result = nom_isobject(_value) != 0;
        }
        return result;
    }

    bool isClass() const
    {
        bool result = false;
        if (_state)
        {
            result = nom_isclass(_state, _value) != 0;
        }
        return result;
    }

    bool isCallable() const
    {
        bool result = false;
        if (_state)
        {
            result = nom_iscallable(_state, _value) != 0;
        }
        return result;
    }

    Value operator[](Value key) const
    {
        assert(_state == key._state);
        return Value(_state, nom_get(_state, _value, key._value));
    }

    Value operator+(Value other) const
    {
        assert(_state == other._state);
        return Value(_state, nom_add(_state, _value, other._value));
    }

    operator int() const
    {
        return nom_toint(_value);
    }

private:
    Value(NomState* state, NomValue value) :
        _state(state),
        _value(value)
    {
    }

    NomState* _state { nullptr };
    NomValue _value;
};

class State
{
public:
    State() :
        _state(nom_newstate())
    {
    }

    ~State()
    {
        nom_freestate(_state);
    }

    Value newNumber(int value)
    {
        return Value(_state, nom_fromint(value));
    }

    Value newMap()
    {
        return Value(_state, nom_newmap(_state));
    }

private:
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    NomState* _state;
};

}
