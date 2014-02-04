#include "../Include/Nominal.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
    NomInterpreter* interpreter = NomCreateInterpreter();
    
    for (;;)
    {
        char line[512];
        fgets(line, 512, stdin);
        if (line[0] != '\n')
        {
            if (NomExecute(interpreter, line))
            {
                NomValue result = NomPop(interpreter);
                char resultString[256];
                NomValueToString(interpreter, resultString, result);
                printf("Result is %s\n", resultString);
            }
            else
            {
                printf("Error: %s\n", NomError(interpreter));
            }
        }
        else
        {
            break;
        }
    }

    NomFreeInterpreter(interpreter);
    
    return 0;
}