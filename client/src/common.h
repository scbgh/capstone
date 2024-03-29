//
// common.h
// Common utility functions.
//

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#if defined(unix) || defined(__unix__) || defined(__unix)
#define UNIX
#endif

#if defined(WIN32) || defined(_WIN32)
#define WIN32
#endif

using std::list;
using std::vector;
using std::set;
using std::map;
using std::string;

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <SDL.h>

// Epsilon value for floating point comparisons
#define EPS 0.001

#ifndef INFINITY
#define INFINITY HUGE_VAL
#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462
#endif

#define DEG_TO_RAD(x) ((x)/180.0*M_PI)
#define RAD_TO_DEG(x) ((x)/M_PI*180.0)

//
// Abort the program with a fatal error
static void Die(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);
    fprintf(stderr, "Fatal error! %s\n", buffer);
    exit(1);
}

//
// Output a line of debug info
static void Debug(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);
    printf("%s\n", buffer);
}

#endif