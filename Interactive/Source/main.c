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

#include <stdlib.h>
#include <stdio.h>

int main()
{
    char resultstring[8192];
    char line[4096];

    NomState* state = nom_newstate();

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
                NomValue result = nom_execute(state, line);
                if (!nom_error(state))
                {
                    nom_tostring(state, resultstring, 8192, result);
                    printf("%s\n", resultstring);
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

    nom_freestate(state);

    return 0;
}
