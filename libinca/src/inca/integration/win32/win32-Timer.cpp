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
        logger << "getSystemClockFrequency(): "
                  "No high-performance counter available on this system";
        logger.error();
        return clock_t(0);
    }
}

clock_t inca::getSystemClocks() {
    LARGE_INTEGER count;
    if (QueryPerformanceCounter(&count)) {
        return clock_t(count.LowPart);
    } else {
        logger << "getSystemClocks(): "
                  "No high-performance counter available on this system";
        logger.error();
        return clock_t(0);
    }
}

#endif
