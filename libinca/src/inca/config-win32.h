/*
 * File: config-win32.h
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains configuration options for building on the Win32
 *      platform. As this file must be maintained by hand (whereas the config.h
 *      file can be created automatically on UNIX variants (via autoconf), the
 *      macro names used here are chosen to match those generated by autoconf,
 *      for the sake of simplicity.
 */

#ifndef INCA_CONFIG_WIN32
#define INCA_CONFIG_WIN32

// Tell the world that we're compiling for the Great Whore of Redmond ;-)
#define __MS_WINDOZE__ 1

// We can't allow the archaic windows headers to define macros for min and max
#define NOMINMAX 1

#define M_PIf   3.14159265358979323846f
#define M_Ef    2.71828182845904523536f
#define M_PI    3.14159265358979323846
#define M_E     2.71828182845904523536
#define M_PIl   3.14159265358979323846
#define M_El    2.71828182845904523536

// ...and Windows doesn't have a few math thingies that we kinda need

float       roundf(float x);        // Prototype floating point round functions
double      round(double x);        // implemented in integration/win32
long double roundl(long double x);
namespace std {
    bool isnan(float x);
    bool isnan(double x);
    bool isnan(long double x);
}

// Windows also doesn't seem to know what a clock_t is...(sigh)
typedef long clock_t;

// Disable some obnoxious warnings
#pragma warning(disable : 4290)     // Ignoring C++ exception spec.
#pragma warning(disable : 4561)     // __fastcall incompatible with '/clr' option
#pragma warning(disable : 4996)     // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct


#define HAVE_LIBFFTW3F  1
#define HAVE_LIBFFTW3   1
//#define HAVE_LIBFFTW3L  0

#endif
