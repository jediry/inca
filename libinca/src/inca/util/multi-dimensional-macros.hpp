/* -*- C++ -*-
 *
 * File: multi-dimensional-macros.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains a number of preprocessor macros for the purpose of
 *      simplifying the implementation of arbitrary-dimensional container
 *      types (e.g., Array, MultiArray).
 *
 *      Because there is so much common (and verbose!) code among the
 *      algorithms working on these types, the coding of these algorithms is
 *      simplified and made more readable through extensive, systematic use of
 *      the C preprocessor to generate code.
 *
 * Using this file:
 *      Because many of the macros declared herein have fairly short,
 *      non-"namespaced" names, they should not be allowed to exist any longer
 *      than necessary, lest they conflict with other macros or variables.
 *      This file does not have the usual #ifndef...#endif multi-include
 *      protection, and thus may be included multiple times. If the macro
 *      UNDEFINE_INCA_MULTI_DIM_MACROS is defined when this file is included
 *      then, instead of defining the macros, it #undef's them (and
 *      UNDEFINE_INCA_MULTI_DIM_MACROS as well).
 */

/*---------------------------------------------------------------------------*/
#ifndef UNDEFINE_INCA_MULTI_DIM_MACROS                  /* Create the macros */
/*---------------------------------------------------------------------------*/

// These macros control the range across which the macros are evaluated.
// If the've not been defined externally, we'll assume some defaults.
#ifndef MIN_DIM
#   define MIN_DIM 2
#   define INCA_MULTI_DIM_MIN_DIM_DEFAULTED
#endif
#ifndef MAX_DIM
#   define MAX_DIM 10
#   define INCA_MULTI_DIM_MAX_DIM_DEFAULTED
#endif


// Import Boost compile-time code generation utilities 
#include <boost/static_assert.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/facilities/expand.hpp>


// These macros are shorthand for Boost preprocessor macros
#define CAT(S1, S2)                 BOOST_PP_CAT(S1, S2)
#define PARAMS(COUNT, P)            BOOST_PP_ENUM_PARAMS(COUNT, P)
#define REPEAT(COUNT, MACRO, DATA)  BOOST_PP_REPEAT(COUNT, MACRO, DATA)
#define FOR_RANGE(START, END, MACRO, DATA)                                  \
    BOOST_PP_REPEAT_FROM_TO(START, BOOST_PP_INC(END), MACRO, DATA)


// These are utility macros used by the FOR_* repetition macros
#define EVAL_MACRO1(Z, ITER, MACRO) MACRO(ITER)
#define EVAL_MACRO2(Z, ITER, MACRO) MACRO(ITER)

// This macro expands MACRO(n) for values of n in [0..DIM - 1]
#define FOR_EACH(DIM, MACRO)    REPEAT(DIM, EVAL_MACRO1, MACRO)

// This macro expands MACRO for every dimensionality in [MIN_DIM..MAX_DIM],
// and can be used to generate dimensionality-specific function definitions.
#define FOR_ALL_DIMS(MACRO)     FOR_RANGE(MIN_DIM, MAX_DIM, EVAL_MACRO2, MACRO)


/*---------------------------------------------------------------------------*/
#else             /* UNDEFINE_INCA_MULTI_DIM_MACROS was defined, so removem  */
/*---------------------------------------------------------------------------*/

// Undefine these only if we defined them ourselves. If they were externally
// defined, we leave them untouched.
#ifdef INCA_MULTI_DIM_MIN_DIM_DEFAULTED
#   undef MIN_DIM
#   undef INCA_MULTI_DIM_MIN_DIM_DEFAULTED
#endif
#ifdef INCA_MULTI_DIM_MAX_DIM_DEFAULTED
#   undef MAX_DIM
#   undef INCA_MULTI_DIM_MAX_DIM_DEFAULTED
#endif

#undef CAT
#undef PARAMS
#undef REPEAT
#undef FOR_RANGE

#undef EVAL_MACRO1
#undef EVAL_MACRO2
#undef FOR_ALL_DIMS
#undef FOR_EACH

// Finally, make sure we can re-enable these macros later
#undef UNDEFINE_INCA_MULTI_DIM_MACROS

/*---------------------------------------------------------------------------*/
#endif                                                 /* All done...go home */
/*---------------------------------------------------------------------------*/
