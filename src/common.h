//
// common.h
// Common utility functions.
//

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdarg>
#include <cstdio>

//
// Abort the program with a fatal error
void die(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);
    fprintf(stderr, "Fatal error! %s\n", buffer);
    exit(1);
}

#endif