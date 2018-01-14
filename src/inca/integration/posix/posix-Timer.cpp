/** -*- C++ -*-
 *
 * File: posix-Timer.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#if __GNUC__    // Ensure we're compiling for the right platform

// Import the class definition
#include <inca/util/Timer>

// Import the POSIX gettimeofday function
#if TIME_WITH_SYS_TIME       // We may need both files...not too sure...
#   include <sys/time.h>
#   include <time.h>
#elif HAVE_SYS_TIME_H        // Just this one...
#   include <sys/time.h>
#elif HAVE_TIME_H            // Last resort...
#   include <time.h>
#endif


clock_t inca::getSystemClockFrequency() {
    // The POSIX gettimeofday function guarantees us microsecond resolution
    return clock_t(1000000);
}

clock_t inca::getSystemClocks() {
    struct timeval tval;
    gettimeofday(&tval, NULL);

    // Convert from (sec, usec) struct to (usec) counter_t
    return clock_t(tval.tv_sec * 1000000 + tval.tv_usec);
}

#endif
