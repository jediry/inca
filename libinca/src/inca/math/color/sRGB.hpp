/* -*- C++ -*-
 *
 * File: sRGB.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */


#ifndef INCA_MATH_COLOR_SRGB
#define INCA_MATH_COLOR_SRGB

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca math library
namespace inca {
    namespace math {
        // Class definition
        class sRGB {
        public:
            static const size_t components = 3;
            static const bool   hasAlpha = false;
        };

        class sRGBA {
        public:
            static const size_t components = 4;
            static const bool   hasAlpha = true;
        };
    };
};

#endif
