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
#include <nominal.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Prints the usage information to the user
void show_usage(char* argv[])
{
    assert(argv);
    fprintf(stderr, "Usage: %s [option] [file...]\n", argv[0]);
}

// Prints a hint indicating how to see additional information to the user
void show_hint(char* argv[])
{
    assert(argv);
    fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
}

// Prints the full help documentation to the user
void show_help(char* argv[])
{
    assert(argv);
    show_usage(argv);
    fprintf(stderr, "Options and arguments:\n"
        "-i, --interactive : Enter a read-eval-print loop prompt after execution\n"
        "-c, --code        : Execute the provided Nominal source code as a string\n"
        "-h, --help        : Display help text\n"
        "file              : Execute the provided Nominal source code file\n");
}

// Enters a read-eval-print loop
void repl(NomState* state)
{
    assert(state);

    char resultstring[8192];
    char line[4096];

    bool quit = false;
    while (!quit)
    {
        printf(":> ");
        if (fgets(line, 4096, stdin) != NULL && line[0] != '\n')
        {
            if (line[0] == '^')
            {
                nom_dumpbytecode(state, line[1] != '\n' ? line + 1 : NULL);
                if (nom_error(state))
                {
                    printf("Error: %s\n", nom_geterror(state));
                }
            }
            else
            {
                NomValue result = nom_evaluate(state, line);
                if (!nom_error(state))
                {
                    nom_tostring(state, resultstring, 8192, result);
                    printf("=> %s\n", resultstring);
                }
                else
                {
                    printf("Error: %s\n", nom_geterror(state));
                }
            }
        }
        else
        {
            quit = true;
        }
    }
}

// Parses the command-line arguments and executes; returns true if an error
// occurred, false otherwise
bool parse_and_execute(NomState* state, int argc, char* argv[])
{
    assert(state);
    assert(argc > 0);
    assert(argv);

    bool interactive = false;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 ||
            strcmp(argv[i], "--help") == 0)
        {
            show_help(argv);
        }
        else if (strcmp(argv[i], "-i") == 0 ||
            strcmp(argv[i], "--interactive") == 0)
        {
            interactive = true;
        }
        else if (strcmp(argv[i], "-c") == 0 ||
            strcmp(argv[i], "--code") == 0)
        {
            ++i;
            if (i >= argc)
            {
                fprintf(stderr, "Argument expected for the %s option\n", argv[i - 1]);
                show_usage(argv);
                show_hint(argv);
                return false;
            }
            else
            {
                const char* code = argv[i];
                nom_execute(state, code);
                if (nom_error(state))
                {
                    fprintf(stderr, "Error: %s\n", nom_geterror(state));
                    return false;
                }
            }
        }
        else if (argv[i][0] == '-')
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            show_usage(argv);
            show_hint(argv);
            return false;
        }
        else
        {
            const char* filepath = argv[i];
            nom_dofile(state, filepath);
            if (nom_error(state))
            {
                fprintf(stderr, "Error: %s\n", nom_geterror(state));
                return false;
            }
        }
    }

    if (interactive)
    {
        repl(state);
    }

    return true;
}

int main(int argc, char* argv[])
{
    NomState* state = nom_newstate();
    bool error = parse_and_execute(state, argc, argv);
    nom_freestate(state);

    if (error)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
