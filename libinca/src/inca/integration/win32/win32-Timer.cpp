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

#ifndef INCA_INTEGRATION_WIN32_TIMER
#define INCA_INTEGRATION_WIN32_TIMER

#if __MS_WINDOZE__  // Ensure we're compiling for the right platform

// Import the class definition
#include <inca/util/Timer>

// Import the BEAST
#include <windows.h>


template <typename scalar, bool notifyListeners>
inca::Timer::counter_t
inca::Timer::getSystemCounterFrequency() {
    LARGE_INTEGER freq;
    if (QueryPerformanceFrequency(&freq)) {
        return counter_t(freq.LowPart);
    } else {
        logger << "Timer::getSystemCounterFrequency(): No "
                  "high-performance counter available on this system";
        logger.error();
        return counter_t(0);
    }
}

template <typename scalar, bool notifyListeners>
inca::Timer::counter_t
inca::Timer::getSystemCounterValue() {
    LARGE_INTEGER count;
    if (QueryPerformanceCounter(&count)) {
        return counter_t(count.LowPart);
    } else {
        logger << "Timer::getSystemCounterValue(): No "
                  "high-performance counter available on this system";
        logger.error();
        return counter_t(0);
    }
}
#endif

#endif
