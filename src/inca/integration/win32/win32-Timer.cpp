/** -*- C++ -*-
 *
 * File: win32-Timer.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Include precompiled header
#include <inca/precomp.h>

// Import system configuration
#include <inca/inca-common.h>

#if __MS_WINDOZE__  // Ensure we're compiling for the right platform

// Import the class definition
#include <inca/util/Timer>

// Import the BEAST
#include <windows.h>

clock_t inca::getSystemClockFrequency() {
    LARGE_INTEGER freq;
    if (QueryPerformanceFrequency(&freq)) {
        return clock_t(freq.LowPart);
    } else {
        INCA_ERROR("getSystemClockFrequency(): "
                   "No high-performance counter available on this system")
        return clock_t(0);
    }
}

clock_t inca::getSystemClocks() {
    LARGE_INTEGER count;
    if (QueryPerformanceCounter(&count)) {
        return clock_t(count.LowPart);
    } else {
        INCA_ERROR("getSystemClocks(): "
                   "No high-performance counter available on this system")
        return clock_t(0);
    }
}

#endif
