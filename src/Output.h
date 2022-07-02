#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define NRM "\x1B[0m"
#define NRM_ENDL NRM "\n"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

#define AIS "[Arch installer script]"

#define PRINT_PREDEF_BODY(color, status, message) va_list args;\
                                                  va_start(args, message);\
                                                  PrintCustom(color, status, message, args);\
                                                  va_end(args)

static void PrintCustom(const char* const color, const char* const status, const char* const message, va_list args)
{
    va_list args2;
    va_copy(args2, args);

    size_t needed = vsnprintf(NULL, 0, message, args) + 1;
    char* const buffer = (char*)malloc(needed * sizeof(char));
    if(buffer == NULL) return;
    buffer[needed-1] = '\0';
    vsnprintf(buffer, needed, message, args2);

    printf("%s" AIS "<%s%s>" NRM_ENDL, color, status, buffer);
    va_end(args2);
    free(buffer);
}

static void PrintError(const char* const message, ...)
{
    PRINT_PREDEF_BODY(RED, "Error: ", message);
}


static void PrintSuccess(const char* const message, ...)
{
    PRINT_PREDEF_BODY(GRN, "Success: ", message);
}

static void PrintColor(const char* const color, const char* const message, ...)
{
    PRINT_PREDEF_BODY(color, "", message);
}

#endif
