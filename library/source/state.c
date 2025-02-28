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
#include "state.h"

#include "value.h"
#include "map.h"
#include "function.h"
#include "parser.h"
#include "prelude.h"
#include "codegen.h"
#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Pops a stack frame from the callstack and returns the return instruction
// pointer
#define POP_FRAME()\
    --state->cp;

// Pushes a stack frame on the callstack given the return instruction pointer
// and the scope
#define PUSH_FRAME(i, a)\
    state->callstack[state->cp].ip = i;\
    state->callstack[state->cp].argcount = a;\
    state->callstack[state->cp++].scope = nom_nil(); // Default scope, may be overridden

// Returns the top stack frame on the callstack
#define TOP_FRAME()\
    (&state->callstack[state->cp - 1])

// Pops a value from the value stack and returns it
#define POP_VALUE()\
    state->stack[--state->sp]

// Pushes a value to the value stack
#define PUSH_VALUE(v)\
    state->stack[state->sp++] = v

// Returns the top value in the value stack
#define TOP_VALUE()\
    state->stack[state->sp - 1]

// Returns the value in the stack at the given offset from the top
#define PEEK_VALUE(n)\
    state->stack[state->sp - 1 - (n)]

// Reads the a typed value from the byte code at the current instruction
#define READAS(t)\
    *(t*)&state->bytecode[state->ip]; state->ip += sizeof(t)

static void compile(
    NomState*   state,
    const char* source
);

static void ret(
    NomState*   state
);

static void call(
    NomState*   state,
    uint8_t     argcount,
    bool        execute
);

static void mark(
    NomState*   state,
    NomValue    value
);

NomState* nom_newstate(
    void
)
{
    NomState* state = (NomState*)malloc(sizeof(NomState));
    assert(state);

    memset(state, 0, sizeof(NomState));

    state->cp = 1;
    state->heap = heap_new();
    state->stringpool = stringpool_new(STATE_STRING_POOL_SIZE);

    // Define intrinsic global variables
    nom_letvar(state, "nil", nom_nil());
    nom_letvar(state, "true", nom_true());
    nom_letvar(state, "false", nom_false());

    // Define intrinsic strings
    state->strings.name = nom_newinternedstring(state, "name");
    state->strings.new = nom_newinternedstring(state, "new");
    state->strings.add = nom_newinternedstring(state, "add");
    state->strings.subtract = nom_newinternedstring(state, "subtract");
    state->strings.multiply = nom_newinternedstring(state, "multiply");
    state->strings.divide = nom_newinternedstring(state, "divide");

    // Define the intrinsic class map
    state->classes.class = nom_newmap(state);
    nom_insert(state, state->classes.class, state->strings.name, nom_newinternedstring(state, "Class"));
    nom_letvar(state, "Class", state->classes.class);
    map_setclass(state, state->classes.class, state->classes.class);

    // Define the other intrinsic classes
    state->classes.nil = state_newclass(state, "Nil");
    state->classes.number = state_newclass(state, "Number");
    state->classes.boolean = state_newclass(state, "Boolean");
    state->classes.string = state_newclass(state, "String");
    state->classes.map = state_newclass(state, "Map");
    state->classes.function = state_newclass(state, "Function");
    state->classes.module = state_newclass(state, "Module");

    // Import the prelude library
    if (!nom_error(state))
    {
        import_prelude(state);
    }

    return state;
}

void nom_freestate(
    NomState*   state
)
{
    assert(state);

    // Free the string pool
    if (state->stringpool)
    {
        stringpool_free(state->stringpool);
    }

    // Free the heap
    if (state->heap)
    {
        heap_free(state->heap);
    }

    free(state);
}

NomValue nom_import(
    NomState*   state,
    const char* module
)
{
    assert(state);
    assert(module);

    // Create a new map to serve as the module's namespace
    NomValue module_scope = nom_newmap(state);
    map_setclass(state, module_scope, state->classes.module);

    if (!nom_error(state))
    {
        // Remember where the instruction pointer was before import
        uint32_t ip = state->ip;

        // Begin a new frame with the module scope
        PUSH_FRAME(state->ip, 0);
        TOP_FRAME()->scope = module_scope;

        // Perform the import
        char modulepath[256];
        sprintf(modulepath, "%s.ns", module);
        nom_dofile(state, modulepath);

        // End the frame
        POP_FRAME();

        // Restore the instruction pointer
        state->ip = ip;
    }

    if (!nom_error(state))
    {
        return module_scope;
    }
    else
    {
        return nom_nil();
    }
}

void nom_letvar(
    NomState*   state,
    const char* identifier,
    NomValue    value
)
{
    assert(state);
    assert(identifier);

    StringId id = stringpool_getid(state->stringpool, identifier);
    state_letinterned(state, id, value);
}

void nom_setvar(
    NomState*   state,
    const char* identifier,
    NomValue    value
)
{
    assert(state);
    assert(identifier);

    StringId id = stringpool_getid(state->stringpool, identifier);
    state_setinterned(state, id, value);
}

NomValue nom_getvar(
    NomState*   state,
    const char* identifier
)
{
    assert(state);
    assert(identifier);

    StringId id = stringpool_getid(state->stringpool, identifier);
    NomValue result = state_getinterned(state, id);
    return result;
}

size_t nom_getargcount(
    NomState*   state
)
{
    assert(state);
    StackFrame* frame = TOP_FRAME();
    return frame->argcount;
}

NomValue nom_getarg(
    NomState*   state,
    size_t      index
)
{
    assert(state);

    NomValue value;

    // If the index is within the number of args in the current stack frame
    size_t argcount = nom_getargcount(state);
    if (index < argcount)
    {
        // Peek at the value on the stack
        value = PEEK_VALUE(argcount - index - 1);
    }
    else
    {
        // Otherwise return nil
        value = nom_nil();
    }

    return value;
}

void nom_execute(
    NomState*   state,
    const char* source
)
{
    assert(state);
    assert(source);

    uint32_t ip = state->ip;

    // Ensure that no trailing bytecode is executed before the newly compiled
    // bytecode
    state->ip = state->end;

    // Compile and execute the code
    compile(state, source);
    state_execute(state);

    // Set the instruction pointer to the end of the bytecode
    state->ip = ip;
}

NomValue nom_evaluate(
    NomState*   state,
    const char* source
)
{
    nom_execute(state, source);
    if (!nom_error(state) && state->sp > 0)
    {
        return POP_VALUE();
    }
    else
    {
        return nom_nil();
    }
}

#ifdef _WIN32
#include <direct.h>
// MSDN recommends against using getcwd & chdir names
#define cwd _getcwd
#define cd _chdir
#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
#endif

void nom_dofile(
    NomState*   state,
    const char* path
)
{
    assert(state);
    assert(path);

    FILE* fp = fopen(path, "r");
    if (fp)
    {
        fseek(fp, 0L, SEEK_END);
        long length = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        char* source = malloc(length + 1);

        size_t bytesread = fread(source, sizeof(char), length, fp);
        if (bytesread > 0)
        {
            source[bytesread] = '\0';
        }
        else
        {
            nom_seterror(state, "Failed to read file '%s'", path);
        }

        fclose(fp);

        if (!nom_error(state))
        {
            char parent[256] = { 0 };
            char previous[256] = { 0 };
            cwd(previous, sizeof(previous));

            bool changed_directories = false;
            char* last = strrchr(path, '/');
            if (last != NULL)
            {
                size_t parent_len = strlen(path) - strlen(last + 1);
                strncpy(parent, path, parent_len);
                if (cd(parent) == 0)
                {
                    changed_directories = true;
                }
            }

            nom_execute(state, source);

            if (changed_directories)
            {
                cd(previous);
            }
        }

        free(source);
    }
    else
    {
        nom_seterror(state, "Failed to open file '%s'", path);
    }
}

void nom_dumpbytecode(
    NomState*   state,
    const char* source
)
{
    assert(state);

    if (source)
    {
        compile(state, source);
    }
    else
    {
        state->ip = 0;
    }

    while (state->ip < state->end)
    {
        OpCode op = (OpCode)state->bytecode[state->ip++];
        printf("0x%08x: %s\t", state->ip - 1, OPCODE_NAMES[op]);

        switch (op)
        {
        case OPCODE_PUSH:
        {
            NomValue value = READAS(NomValue);
            char buffer[256];
            nom_tostring(state, buffer, 256, value);
            printf("%s", buffer);
        }
        break;

        case OPCODE_DUP:
        {
            uint32_t index = READAS(uint32_t);
            printf("%u", index);
        }
        break;

        case OPCODE_DEFINE:
        case OPCODE_FETCH:
        case OPCODE_ASSIGN:
        {
            StringId id = READAS(StringId);
            const char* string = stringpool_find(state->stringpool, id);
            printf("%s", string);
        }
        break;

        case OPCODE_MAP:
        {
            uint32_t itemCount = READAS(uint32_t);
            printf("\t%u", itemCount);
        }
        break;

        case OPCODE_FUNCTION:
        {
            uint32_t ip = READAS(uint32_t);
            uint32_t paramcount = READAS(uint32_t);
            printf("0x%08x %u ", ip, paramcount);
            for (uint32_t i = 0; i < paramcount; ++i)
            {
                StringId id = READAS(StringId);
                const char* param = stringpool_find(state->stringpool, id);
                printf("%s", param);
                if (i < paramcount - 1)
                {
                    printf(" ");
                }
            }
        }
        break;

        case OPCODE_JUMP:
        case OPCODE_JUMPIF:
        {
            uint32_t ip = READAS(uint32_t);
            printf("0x%08x", ip);
        }
        break;

        case OPCODE_CALL:
        {
            uint32_t argcount = READAS(uint32_t);
            printf("%u", argcount);
        }
        break;

        default:
            break;
        }

        printf("\n");
    }

    state->ip = state->end;
}

bool nom_error(
    NomState*   state
)
{
    assert(state);
    return state->errorflag;
}

void nom_seterror(
    NomState*   state,
    const char* fmt,
    ...
)
{
    assert(state);
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    vsprintf(state->error, fmt, args);
    va_end(args);
    state->errorflag = true;
}

const char* nom_geterror(
    NomState*   state
)
{
    assert(state);
    state->errorflag = false;
    return state->error;
}

int nom_collectgarbage(
    NomState*   state
)
{
    assert(state);

    // Mark all values on the stack
    for (uint32_t i = 0; i < state->sp; ++i)
    {
        value_visit(state, state->stack[i], mark);
    }

    // Mark all scopes on the callstack
    for (uint32_t i = 0; i < state->cp; ++i)
    {
        value_visit(state, state->callstack[i].scope, mark);
    }

    // Sweep
    unsigned int count = heap_sweep(state->heap);

    return count;
}

void state_letinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
)
{
    assert(state);

    StackFrame* frame = TOP_FRAME();
    NomValue string = string_newinterned(id);

    // Create a new map for the scope if this is the first variable declared
    // in this scope
    if (!nom_ismap(state, frame->scope))
    {
        frame->scope = nom_newmap(state);
    }

    // Attempt to define the variable at the top-most scope
    NomValue scope = frame->scope;
    if (!map_insert(state, scope, string, value))
    {
        nom_seterror(state, "Variable '%s' already exists", stringpool_find(state->stringpool, id));
    }
}

void state_setinterned(
    NomState*   state,
    StringId    id,
    NomValue    value
)
{
    assert(state);

    NomValue string = string_newinterned(id);

    bool success = false;

    // For each stack frame
    for (int i = state->cp - 1; i >= 0; --i)
    {
        StackFrame* frame = &state->callstack[i];
        NomValue closure_value;

        // Try to set the variable
        NomValue scope = frame->scope;
        if (map_update(state, scope, string, value))
        {
            success = true;
            break;
        }
        else if (!nom_isnil(frame->closure_scope) && map_find(state, frame->closure_scope, string, &closure_value))
        {
            if (map_update(state, frame->closure_scope, string, value))
            {
                success = true;
                break;
            }
        }
    }

    if (!success)
    {
        nom_seterror(state, "No variable '%s'", stringpool_find(state->stringpool, id));
    }
}

NomValue state_getinterned(
    NomState*   state,
    StringId    id
)
{
    assert(state);

    NomValue result = nom_nil();
    NomValue string = string_newinterned(id);

    bool success = false;

    // For each stack frame
    for (int i = state->cp - 1; i >= 0; --i)
    {
        StackFrame* frame = &state->callstack[i];
        // Try to get the variable value
        NomValue scope = frame->scope;
        if (map_find(state, scope, string, &result))
        {
            success = true;
            break;
        }
        else if (!nom_isnil(frame->closure_scope) && map_find(state, frame->closure_scope, string, &result))
        {
            success = true;
            break;
        }
    }

    if (!success)
    {
        nom_seterror(state, "No variable '%s' in scope", stringpool_find(state->stringpool, id));
    }

    return result;
}

NomValue state_call(
    NomState*   state,
    NomValue    value,
    uint8_t     argcount,
    NomValue*   args
)
{
    assert(state);

    for (uint8_t i = 0; i < argcount; ++i)
    {
        PUSH_VALUE(args[i]);
    }

    PUSH_VALUE(value);

    call(state, argcount, true);

    NomValue result = POP_VALUE();
    return result;
}

void state_execute(
    NomState*   state
)
{
    assert(state);

    uint32_t startcp = state->cp;

    StringId id;
    NomValue l, r, result;
    OpCode op;
    uint32_t count, ip;
    bool stop = false;

    uint32_t end = state->end;
    while (state->ip < end && !state->errorflag && !stop)
    {
        op = (OpCode)state->bytecode[state->ip++];
        switch (op)
        {
        case OPCODE_PUSH:
            result = READAS(NomValue);
            PUSH_VALUE(result);
            break;

        case OPCODE_POP:
            (void)POP_VALUE();
            break;

        case OPCODE_DUP:
            count = READAS(uint32_t);
            result = PEEK_VALUE(count);
            PUSH_VALUE(result);
            break;

        case OPCODE_ADD:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_add(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_SUB:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_sub(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_MUL:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_mul(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_DIV:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_div(state, l, r);
            PUSH_VALUE(result);
            break;

        case OPCODE_NEG:
            l = POP_VALUE();
            result = nom_neg(state, l);
            PUSH_VALUE(result);
            break;

        case OPCODE_EQ:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_equals(state, l, r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_NE:
            l = POP_VALUE();
            r = POP_VALUE();
            result = !nom_equals(state, l, r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_GT:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_todouble(l) > nom_todouble(r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_GTE:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_todouble(l) >= nom_todouble(r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_LT:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_todouble(l) < nom_todouble(r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_LTE:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_todouble(l) <= nom_todouble(r) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_AND:
            l = POP_VALUE();
            r = POP_VALUE();
            result = (nom_istrue(state, l) && nom_istrue(state, r)) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_OR:
            l = POP_VALUE();
            r = POP_VALUE();
            result = (nom_istrue(state, l) || nom_istrue(state, r)) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_NOT:
            l = POP_VALUE();
            result = !nom_istrue(state, l) ? nom_true() : nom_false();
            PUSH_VALUE(result);
            break;

        case OPCODE_DEFINE:
            id = READAS(StringId);
            state_letinterned(state, id, TOP_VALUE());
            break;

        case OPCODE_ASSIGN:
            id = READAS(StringId);
            state_setinterned(state, id, TOP_VALUE());
            break;

        case OPCODE_FETCH:
            id = READAS(StringId);
            result = state_getinterned(state, id);
            PUSH_VALUE(result);
            break;

        case OPCODE_INSERT:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_insert(state, r, l, TOP_VALUE()))
            {
                nom_seterror(state, "Value for key '%s' already exists", nom_getstring(state, l));
            }
            break;

        case OPCODE_UPDATE:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_update(state, r, l, TOP_VALUE()))
            {
                nom_seterror(state, "No value for key '%s'", nom_getstring(state, l));
            }
            break;

        case OPCODE_FIND:
            l = POP_VALUE();
            r = POP_VALUE();
            if (!nom_find(state, r, l, &result))
            {
                nom_seterror(state, "No value for key '%s'", nom_getstring(state, l));
            }
            else
            {
                PUSH_VALUE(result);
            }
            break;

        case OPCODE_GET:
            l = POP_VALUE();
            r = POP_VALUE();
            result = nom_get(state, r, l);
            PUSH_VALUE(result);
            break;

        case OPCODE_SET:
            l = POP_VALUE();
            r = POP_VALUE();
            nom_set(state, r, l, TOP_VALUE());
            break;

        case OPCODE_MAP:
            count = READAS(uint32_t);
            result = nom_newmap(state);
            for (uint32_t i = 0; i < count; ++i)
            {
                NomValue key = POP_VALUE();
                NomValue value = POP_VALUE();
                map_set(state, result, key, value);
            }
            PUSH_VALUE(result);
            break;

        case OPCODE_FUNCTION:
            ip = READAS(uint32_t);
            count = READAS(uint32_t);
            result = function_new(state, ip);
            for (uint32_t i = 0; i < count; ++i)
            {
                StringId parameter = READAS(StringId);
                function_addparam(state, result, parameter);
            }
            PUSH_VALUE(result);
            break;

        case OPCODE_CLASSOF:
            result = state_classof(state, POP_VALUE());
            PUSH_VALUE(result);
            break;

        case OPCODE_JUMP:
            ip = READAS(uint32_t);
            state->ip = ip;
            break;

        case OPCODE_JUMPIF:
            ip = READAS(uint32_t);
            l = POP_VALUE();
            if (nom_istrue(state, l))
            {
                state->ip = ip;
            }
            break;

        case OPCODE_CALL:
            count = READAS(uint32_t);
            call(state, count, false);
            break;

        case OPCODE_RET:
            ret(state);
            if (state->cp < startcp)
            {
                stop = true;
            }
            break;

        case OPCODE_INVALID:
            nom_seterror(state, "Invalid opcode");
            break;
        }
    }
}

NomValue state_newclass(
    NomState*   state,
    const char* name
)
{
    assert(state);
    assert(name);

    NomValue classname = nom_newinternedstring(state, name);
    NomValue classvalue = nom_newmap(state);
    nom_insert(state, classvalue, state->strings.name, classname);
    map_setclass(state, classvalue, state->classes.class);

    nom_letvar(state, name, classvalue);

    return classvalue;
}

NomValue state_classof(
    NomState*   state,
    NomValue    value
)
{
    assert(state);
    assert(state);

    NomValue result = nom_nil();

    ValueType type = GET_TYPE(value);

    switch (type)
    {
    case VALUETYPE_NIL:
        result = state->classes.nil;
        break;
    case VALUETYPE_NUMBER:
        result = state->classes.number;
        break;
    case VALUETYPE_BOOLEAN:
        result = state->classes.boolean;
        break;
    case VALUETYPE_INTERNED_STRING:
        result = state->classes.string;
        break;
    case VALUETYPE_OBJECT:
    {
        result = map_getclass(state, value);
        if (!nom_istrue(state, result))
        {
            HeapObject* object = heap_getobject(state->heap, value);
            if (object)
            {
                switch (object->type)
                {
                case OBJECTTYPE_STRING:
                    result = state->classes.string;
                    break;
                case OBJECTTYPE_MAP:
                    result = state->classes.map;
                    break;
                case OBJECTTYPE_FUNCTION:
                    result = state->classes.function;
                    break;
                }
            }
        }
    }
    break;
    }

    return result;
}

static void compile(
    NomState*   state,
    const char* source
)
{
    assert(state);
    assert(source);

    state->errorflag = false;

    Parser* p = parser_new(source, state->stringpool);
    Node* node = parser_exprs(p, true);

    if (!node)
    {
        nom_seterror(state, parser_geterror(p));
    }
    else
    {
        state->end = generatecode(node, state->bytecode, state->end);
        node_free(node);
    }

    parser_free(p);
}

static void ret(
    NomState*   state
)
{
    assert(state);

    StackFrame* frame = TOP_FRAME();
    NomValue result = POP_VALUE();

    for (int i = 0; i < frame->argcount; ++i)
    {
        (void)POP_VALUE();
    }

    state->ip = frame->ip;

    // Pop the frame before pushing the result to ensure we're returning to the correct scope
    POP_FRAME();
    PUSH_VALUE(result);
}

static void call(
    NomState*   state,
    uint8_t     argcount,
    bool        execute
)
{
    assert(state);

    NomValue value = POP_VALUE();
    if (nom_iscallable(state, value))
    {
        value = function_resolve(state, value);

        PUSH_FRAME(state->ip, argcount);

        // Use the function's closure as the scope for the new frame
        NomValue closure = function_getclosure(state, value);
        if (nom_ismap(state, closure))
        {
            TOP_FRAME()->closure_scope = closure;
        }

        if (function_isnative(state, value))
        {
            NomFunction function = function_getnative(state, value);
            value = function(state);
            PUSH_VALUE(value);
            ret(state);
        }
        else
        {
            size_t paramcount = function_getparamcount(state, value);
            if (argcount <= paramcount)
            {
                size_t i = paramcount;
                while (i > 0)
                {
                    --i;
                    NomValue arg = nom_getarg(state, i);
                    StringId param = function_getparam(state, value, i);
                    state_letinterned(state, param, arg);
                }
                state->ip = function_getip(state, value);

                if (execute)
                {
                    state_execute(state);
                }
            }
            else
            {
                nom_seterror(state, "Too many arguments given (expected %u)", paramcount);
            }
        }
    }
    else
    {
        nom_seterror(state, "Value cannot be called");
    }
}

static void mark(
    NomState*   state,
    NomValue    value
)
{
    assert(state);
    heap_mark(state->heap, value);
}
