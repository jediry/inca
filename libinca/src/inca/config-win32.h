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

// We need this in order to use the math.h constants
#ifndef _USE_MATH_DEFINES
#   define _USE_MATH_DEFINES 1
#endif

// ...and Windows doesn't have a few math thingies that we kinda need
#define M_PIl M_PI      // Alias the long double version to the double version
#define M_El M_E

float       roundf(float x);        // Prototype floating point round functions
double      round(double x);        // implemented in integration/win32
long double roundl(long double x);

// Windows also doesn't seem to know what a clock_t is...(sigh)
typedef long clock_t;

#endif