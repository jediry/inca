/**
 * File: color.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      color primitive types algorithms.
 *
 *      It also provides macros for easily typedefing the primitives.
 */

#ifndef INCA_MATH_COLOR_HPP
#define INCA_MATH_COLOR_HPP


// Primitive types
#include "color/Color"

// Colorspace definitions
#include "color/sRGB.hpp"
#include "color/Grayscale.hpp"


// Macro to typedef the instantiations of the color primitive types,
// with an optional prefix and suffix
#define INCA_MATH_COLOR_TYPES_CUSTOM(SCALAR, COLORSPACE, PREF, SUFF)        \
    typedef inca::math::Color<SCALAR, inca::math:: COLORSPACE> PREF ## Color ## SUFF;

// Simple macro omitting the prefix and suffix
#define INCA_MATH_COLOR_TYPES(SCALAR, COLORSPACE)                           \
    INCA_MATH_COLOR_TYPES_CUSTOM(SCALAR, COLORSPACE, , )

#endif
