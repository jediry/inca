/**
 * File: scalar.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      scalar primitive types and algorithms.
 *
 *      This file also declares macros for typedefing the set of
 *      related scalar types. For example, calling:
 *          INCA_MATH_SCALAR_TYPES(double, IS_WITHIN_TEMPLATE)
 *      creates a list of typedefs something like:
 *          typedef typename inca::math::scalar_traits<double>::value_type scalar_t;
 *          typedef typename inca::math::scalar_traits<double>::arg_type scalar_arg_t;
 *          .
 *          .
 *          .
 *      This permits the scalar types to typedef'd fairly effortlessly. The
 *      second argument should be one of the values IS_WITHIN_TEMPLATE or
 *      IS_NOT_WITHIN_TEMPLATE, depending on the context where the macro is
 *      instantiated (this is necessary because the C++ standard doesn't permit
 *      the "typename" keyword outside of template definitions. There is also
 *      a version called INCA_MATH_SCALAR_TYPES_CUSTOM that accepts two
 *      additional, optional arguments: arbitrary text to be prefixed and
 *      suffixed onto all of the typedef'd names.
 */

#ifndef INCA_MATH_SCALAR_HPP
#define INCA_MATH_SCALAR_HPP


// Primitive types
#include "scalar/scalar_types"

// Operators & functions
#include "scalar/scalar_ops"


// Macro to typedef the variants of a scalar type,
// with an optional prefix and suffix
#define INCA_MATH_SCALAR_TYPES_CUSTOM(SCALAR, TYPENAME, PREF, SUFF)         \
    typedef inca::math::scalar_traits<SCALAR>  PREF ## scalar_traits ## SUFF;\
    typedef TYPENAME inca::math::scalar_traits<SCALAR>::value_type PREF ## scalar_t     ## SUFF;\
    typedef TYPENAME inca::math::scalar_traits<SCALAR>::arg_type   PREF ## scalar_arg_t ## SUFF;

// Simple macro omitting the prefix and suffix
#define INCA_MATH_SCALAR_TYPES(SCALAR, TYPENAME)                             \
    INCA_MATH_SCALAR_TYPES_CUSTOM(SCALAR, TYPENAME, , )

// Symbolic constants for specifying whether or not the macro instantiation is
// within a template declaration
#define IS_WITHIN_TEMPLATE      typename
#define IS_NOT_WITHIN_TEMPLATE  

#endif
