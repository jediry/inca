/* -*- C++ -*-
 *
 * File: Grayscale.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */


#ifndef INCA_MATH_COLOR_GRAYSCALE
#define INCA_MATH_COLOR_GRAYSCALE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca math library
namespace inca {
    namespace math {
        // Class definition
        class Grayscale {
        public:
            static const size_t components = 1;
            static const bool   hasAlpha = false;
        };

        class GrayscaleA {
        public:
            static const size_t components = 2;
            static const bool   hasAlpha = true;
        };
    };
};

#endif