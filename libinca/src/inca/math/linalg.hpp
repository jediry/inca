/**
 * File: linalg.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      linear algebra primitive types and the algorithms that work on them.
 *
 *      It also provides macros for easily typedefing the primitives.
 */

#ifndef INCA_MATH_LINALG_HPP
#define INCA_MATH_LINALG_HPP


// Primitive types
#include "linalg/Point"
#include "linalg/Vector"
#include "linalg/Ray"
#include "linalg/Quaternion"
#include "linalg/Matrix"

// Operators & functions
#include "linalg/operations"
#include "linalg/transformations"
#include "linalg/projections"


// Macro to typedef the instantiations of the linear algebra types,
// with an optional prefix and suffix
#define INCA_MATH_LINALG_TYPES_CUSTOM(SCALAR, DIM, PREF, SUFF)               \
    typedef inca::math::Point<SCALAR, DIM>       PREF ## Point      ## SUFF; \
    typedef inca::math::Vector<SCALAR, DIM>      PREF ## Vector     ## SUFF; \
    typedef inca::math::Ray<SCALAR, DIM>         PREF ## Ray        ## SUFF; \
    typedef inca::math::Matrix<SCALAR, DIM, DIM> PREF ## Matrix     ## SUFF; \
    typedef inca::math::Quaternion<SCALAR>       PREF ## Quaternion ## SUFF;

// Simple macro omitting the prefix and suffix
#define INCA_MATH_LINALG_TYPES(SCALAR, DIM)                                  \
    INCA_MATH_LINALG_TYPES_CUSTOM(SCALAR, DIM, , )

#endif
