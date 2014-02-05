#include "Nominal.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
    NomState* s = NomState_Create();

    NomValue value = NomString_FromString(s, "Testing testing...");
    const char* test = NomString_AsString(s, value);
    
    for (;;)
    {
        char line[512];
        fgets(line, 512, stdin);
        if (line[0] != '\n')
        {
            if (NomState_Execute(s, line))
            {
                NomValue result = NomState_Pop(s);
                char resultString[256];
                NomValue_AsString(s, resultString, result);
                printf("Result is %s\n", resultString);
            }
            else
            {
                printf("Error: %s\n", NomState_Error(s));
            }
        }
        else
        {
            break;
        }
    }

    NomState_Free(s);
    
    return 0;
}