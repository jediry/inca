/*
 * File: Imaging.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      imaging classes and functions.
 */

#ifndef INCA_IMAGING
#define INCA_IMAGING

// Import primitive type definitions
#include "imaging/color_types"
#include "imaging/Image"

// Macro to typedef all these thingies
#define INCA_IMAGING_TYPEDEFS(SCALAR, COLOR_SPACE, ALPHA, PREF, SUFF)   \
    typedef Inca::Imaging::Color<SCALAR, COLOR_SPACE, ALPHA>            \
                PREF ## Color ## SUFF;                                  \
    typedef Inca::Imaging::Image<PREF ## Color ## SUFF>                 \
                PREF ## Image ## SUFF;


//#include "MIPMap"

// Import Filter definitions
//#include "filter/Filter"
//#include "filter/RectangularFilter"
//#include "filter/BoxFilter"
//#include "filter/PyramidFilter"

//#include "Normalize"

#endif
