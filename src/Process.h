#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>

#include "Output.h"

static int RunProcess(const char* process, const char* sinp) // stdin pipe
{
    fflush(stdout);
    FILE* p = popen(process, "w");
    if(p == NULL)
    {
        PrintError("couldn't start subprocess [%s]", process);
        return -1;
    }
    PrintSuccess("started subprocess [%s]", process);

    if(sinp != NULL)
    {
        if(fprintf(p, "%s", sinp) < 0) {
            PrintColor(MAG, "Error: writing to stdin of subprocess [%s] | stdin: %s", process, sinp);
        }
        else {
            PrintColor(BLU, "Successfully wrote to stdin of subprocess [%s] | stdin: %s", process, stdin);
        }
    }

    int status;
    if((status = pclose(p)) == -1) {
        PrintError("failed to close subprocess [%s]", process);
        return status;
    }
    PrintSuccess("finished subprocess [%s]", process);
    return WEXITSTATUS(status);
}

#endif
