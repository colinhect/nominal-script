#include "Nominal.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
    NomInterpreter* interpreter = NomInterpreter_Create();
    
    for (;;)
    {
        char line[512];
        fgets(line, 512, stdin);
        if (line[0] != '\n')
        {
            if (NomInterpreter_Execute(interpreter, line))
            {
                NomValue result = NomInterpreter_Pop(interpreter);
                char resultString[256];
                NomValue_AsString(interpreter, resultString, result);
                printf("Result is %s\n", resultString);
            }
            else
            {
                printf("Error: %s\n", NomInterpreter_Error(interpreter));
            }
        }
        else
        {
            break;
        }
    }

    NomInterpreter_Free(interpreter);
    
    return 0;
}