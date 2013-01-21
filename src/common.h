//
// common.h
// Common utility functions.
//

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <string>

#if defined(unix) || defined(__unix__) || defined(__unix)
#define UNIX
#endif

#if defined(WIN32) || defined(_WIN32)
#define WIN32
#endif

using std::vector;
using std::set;
using std::map;
using std::string;

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

#endif