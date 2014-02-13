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
#include <Nominal.h>

#include <stdlib.h>
#include <stdio.h>

int main()
{
    NomState* state = NomState_Create();

    for (;;)
    {
        printf(":> ");
        char line[512];
        fgets(line, 512, stdin);
        if (line[0] != '\n')
        {
            NomValue result = NomState_Execute(state, line);
            if (!NomState_ErrorOccurred(state))
            {
                char resultString[256];
                NomValue_AsString(resultString, result);
                printf("%s\n", resultString);
            }
            else
            {
                printf("Error: %s\n", NomState_GetError(state));
            }
        }
        else
        {
            break;
        }
    }

    NomState_Free(state);

    return 0;
}