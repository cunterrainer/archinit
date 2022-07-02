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
        printf(RED AIS "<Error: couldn't start subprocess [%s]" NRM_ENDL, process);
        return -1;
    }
    printf(GRN AIS "<Successfully started subprocess [%s]" NRM_ENDL, process);

    if(sinp != NULL)
    {
        if(fprintf(p, "%s", sinp) < 0) {
            printf(MAG AIS "<Error: writing to stdin of subprocess [%s] | stdin: %s" NRM_ENDL, process, sinp);
        }
        else {
            printf(BLU AIS "<Successfully wrote to stdin of subprocess [%s] | stdin: %s" NRM_ENDL, process, sinp);
        }
    }

    int status;
    if((status = pclose(p)) == -1) {
        printf(RED AIS "<Error: failed to close subprocess [%s]" NRM_ENDL, process);
        return status;
    }
    printf(GRN AIS "<Successfully finished subprocess [%s]" NRM_ENDL, process);
    return WEXITSTATUS(status);
}

#endif
