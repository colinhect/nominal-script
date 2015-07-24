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
#include "prelude.h"

#include <assert.h>
#include <stdio.h>

static NomValue prelude_print(
    NomState*   state
    )
{
    assert(state);

    char string[8192];
    size_t argcount = nom_getargcount(state);
    for (size_t i = 0; i < argcount; ++i)
    {
        NomValue arg = nom_getarg(state, i);
        if (nom_isstring(arg))
        {
            printf("%s", nom_getstring(state, arg));
        }
        else
        {
            nom_tostring(state, string, 8192, arg);
            printf("%s", string);
        }

        if (i < argcount - 1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return nom_nil();
}

static NomValue prelude_forvalues(
    NomState*   state
    )
{
    assert(state);

    NomValue values = nom_getarg(state, 0);
    NomValue function = nom_getarg(state, 1);

    if (nom_isiterable(state, values))
    {
        if (nom_isinvokable(state, function))
        {
            NomIterator iterator = { 0 };
            while (nom_next(state, values, &iterator))
            {
                NomValue value = iterator.value;
                nom_invoke(state, function, 1, &value);
            }
        }
        else
        {
            nom_seterror(state, "'function' is not invokable");
        }
    }
    else
    {
        nom_seterror(state, "'values' is not iterable");
    }

    return nom_nil();
}

static NomValue prelude_forkeys(
    NomState*   state
    )
{
    assert(state);

    NomValue keys = nom_getarg(state, 0);
    NomValue function = nom_getarg(state, 1);

    if (nom_isiterable(state, keys))
    {
        if (nom_isinvokable(state, function))
        {
            NomIterator iterator = { 0 };
            while (nom_next(state, keys, &iterator))
            {
                NomValue key = iterator.key;
                nom_invoke(state, function, 1, &key);
            }
        }
        else
        {
            nom_seterror(state, "'function' is not invokable");
        }
    }
    else
    {
        nom_seterror(state, "'keys' is not iterable");
    }

    return nom_nil();
}

static NomValue prelude_if(
    NomState*   state
    )
{
    assert(state);

    NomValue condition = nom_getarg(state, 0);
    NomValue thenBody = nom_getarg(state, 1);
    NomValue elseBody = nom_getarg(state, 2);

    NomValue result;
    if (nom_isinvokable(state, condition))
    {
        result = nom_invoke(state, condition, 0, NULL);
    }
    else
    {
        result = condition;
    }

    if (!nom_error(state))
    {
        if (nom_istrue(state, result) && nom_istrue(state, thenBody))
        {
            if (nom_isinvokable(state, thenBody))
            {
                return nom_invoke(state, thenBody, 0, NULL);
            }
            else
            {
                nom_seterror(state, "'then' is not invokable");
            }
        }
        else if (!nom_istrue(state, result) && nom_istrue(state, elseBody))
        {
            if (nom_isinvokable(state, elseBody))
            {
                return nom_invoke(state, elseBody, 0, NULL);
            }
            else
            {
                nom_seterror(state, "'else' is not invokable");
            }
        }
    }

    return nom_nil();
}

static NomValue prelude_while(
    NomState*   state
    )
{
    assert(state);

    NomValue condition = nom_getarg(state, 0);
    NomValue body = nom_getarg(state, 1);

    NomValue result = nom_nil();
    if (nom_isinvokable(state, condition))
    {
        if (nom_isinvokable(state, body))
        {
            for (;;)
            {
                NomValue value = nom_invoke(state, condition, 0, NULL);
                if (nom_error(state) || !nom_istrue(state, value))
                {
                    break;
                }

                result = nom_invoke(state, body, 0, NULL);
            }
        }
        else
        {
            nom_seterror(state, "'body' is not invokable");
        }
    }
    else
    {
        nom_seterror(state, "'condition' is not invokable");
    }

    return result;
}

static NomValue prelude_assertequal(
    NomState*   state
    )
{
    assert(state);

    NomValue actual = nom_getarg(state, 0);
    NomValue expected = nom_getarg(state, 1);

    if (!nom_equals(state, actual, expected))
    {
        char actualBuffer[128];
        nom_tostring(state, actualBuffer, 128, actual);
        char expectedBuffer[128];
        nom_tostring(state, expectedBuffer, 128, expected);

        nom_seterror(state, "Failed assertion: %s != %s", actualBuffer, expectedBuffer);
    }

    return nom_nil();
}

static NomValue prelude_collectgarbage(
    NomState*   state
    )
{
    assert(state);

    unsigned int count = nom_collectgarbage(state);

    NomValue result = nom_fromunsignedint(count);
    return result;
}

void import_prelude(
    NomState*   state
    )
{
    assert(state);

    nom_letvar(state, "nil", nom_nil());
    assert(!nom_error(state));
    nom_letvar(state, "true", nom_true());
    assert(!nom_error(state));
    nom_letvar(state, "false", nom_false());
    assert(!nom_error(state));
    nom_letvar(state, "print", nom_newfunction(state, prelude_print));
    assert(!nom_error(state));
    nom_letvar(state, "if", nom_newfunction(state, prelude_if));
    assert(!nom_error(state));
    nom_letvar(state, "while", nom_newfunction(state, prelude_while));
    assert(!nom_error(state));
    nom_letvar(state, "forValues", nom_newfunction(state, prelude_forvalues));
    assert(!nom_error(state));
    nom_letvar(state, "forKeys", nom_newfunction(state, prelude_forkeys));
    assert(!nom_error(state));
    nom_letvar(state, "assertEqual", nom_newfunction(state, prelude_assertequal));
    assert(!nom_error(state));
    nom_letvar(state, "collectGarbage", nom_newfunction(state, prelude_collectgarbage));
    assert(!nom_error(state));
}
