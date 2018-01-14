/** -*- C++ -*-
 *
 * File: win32-missing.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import system configuration
#include <inca/inca-common.h>

#if __MS_WINDOZE__  // Ensure we're compiling for the right platform

// Import standard math functions
#include <cmath>
#include <float.h>

bool std::isnan(float x)       { return _isnan(x) != 0; }
bool std::isnan(double x)      { return _isnan(x) != 0; }
bool std::isnan(long double x) { return _isnan(x) != 0; }

// Windows doesn't have the appropriate round function for floats
float roundf(float x) {
    return (float)long(x);
}

// ...or for doubles...
double round(double x) {
    return (double)long(x);
}

// ...or for long doubles
long double roundl(long double x) {
    return (long double)long(x);
}

#endif
