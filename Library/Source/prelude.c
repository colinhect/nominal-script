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

#include "map.h"
#include "state.h"
#include "string.h"
#include "value.h"

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
        if (nom_isstring(state, arg))
        {
            printf("%s", nom_getstring(state, arg));
        }
        else
        {
            nom_tostring(state, string, sizeof(string), arg);
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

static NomValue prelude_tostring(
    NomState*   state
)
{
    assert(state);

    NomValue value = nom_getarg(state, 0);

    NomValue result = nom_nil();

    char string[8192];
    nom_tostring(state, string, sizeof(string), value);
    if (!nom_error(state))
    {
        result = nom_newstring(state, string);
    }

    return result;
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
    NomValue thenbody = nom_getarg(state, 1);
    NomValue elsebody = nom_getarg(state, 2);

    NomValue conditionresult;
    if (nom_isinvokable(state, condition))
    {
        conditionresult = nom_invoke(state, condition, 0, NULL);
    }
    else
    {
        conditionresult = condition;
    }

    NomValue result = nom_nil();
    if (!nom_error(state))
    {
        if (nom_istrue(state, conditionresult))
        {
            if (nom_isinvokable(state, thenbody))
            {
                result = nom_invoke(state, thenbody, 0, NULL);
            }
            else if (nom_istrue(state, thenbody))
            {
                nom_seterror(state, "'then' is not invokable");
            }
        }
        else
        {
            if (nom_isinvokable(state, elsebody))
            {
                return nom_invoke(state, elsebody, 0, NULL);
            }
            else if (nom_istrue(state, elsebody))
            {
                nom_seterror(state, "'else' is not invokable");
            }
        }
    }

    return result;
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

    int count = nom_collectgarbage(state);

    NomValue result = nom_fromunsignedint(count);
    return result;
}

static NomValue prelude_panic(
    NomState*   state
)
{
    assert(state);

    NomValue message = nom_getarg(state, 0);

    assert(state);

    if (nom_isstring(state, message))
    {
        nom_seterror(state, "%s", nom_getstring(state, message));
    }
    else
    {
        char string[8192];
        nom_tostring(state, string, sizeof(string), message);
        nom_seterror(state, "%s", string);
    }

    return nom_nil();
}

static NomValue prelude_class(
    NomState*   state
)
{
    assert(state);

    NomValue map = nom_nil();

    NomValue name = nom_getarg(state, 0);
    if (nom_isstring(state, name))
    {
        map = nom_getarg(state, 1);
        if (!nom_istrue(state, map))
        {
            map = nom_newmap(state);
        }

        map_setclass(state, map, state->classes.class);

        NomValue namestring = nom_newinternedstring(state, "name");
        map_insertorset(state, map, namestring, name);
    }
    else
    {
        nom_seterror(state, "'name' is not a String");
    }

    return map;
}

static NomValue prelude_classof(
    NomState*   state
)
{
    assert(state);

    NomValue value = nom_getarg(state, 0);

    NomValue result = state_classof(state, value);
    return result;
}

static NomValue prelude_object(
    NomState*   state
)
{
    assert(state);

    NomValue object = nom_nil();

    NomValue class = nom_getarg(state, 0);
    if (nom_ismap(state, class))
    {
        object = nom_getarg(state, 1);
        if (nom_ismap(state, object))
        {
            map_setclass(state, object, class);
        }
        else
        {
            object = nom_nil();
            nom_seterror(state, "'object' is not a Map");
        }
    }
    else
    {
        nom_seterror(state, "'class' is not a Map");
    }

    return object;
}

void import_prelude(
    NomState*   state
)
{
    assert(state);

    if (!nom_error(state))
    {
        nom_letvar(state, "print", nom_newfunction(state, prelude_print));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "toString", nom_newfunction(state, prelude_tostring));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "if", nom_newfunction(state, prelude_if));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "while", nom_newfunction(state, prelude_while));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "forValues", nom_newfunction(state, prelude_forvalues));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "forKeys", nom_newfunction(state, prelude_forkeys));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "assertEqual", nom_newfunction(state, prelude_assertequal));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "collectGarbage", nom_newfunction(state, prelude_collectgarbage));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "panic", nom_newfunction(state, prelude_panic));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "class", nom_newfunction(state, prelude_class));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "classOf", nom_newfunction(state, prelude_classof));
    }

    if (!nom_error(state))
    {
        nom_letvar(state, "object", nom_newfunction(state, prelude_object));
    }
}
