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


// Because this file is intended to be repeatedly included and de-included,
// we only want to include these once. Note that ONLY THIS SMALL PART of the
// file is double-include-protected.
#ifndef INCA_MULTI_DIM_MACRO_INCLUDES
#define INCA_MULTI_DIM_MACRO_INCLUDES
    #include <boost/static_assert.hpp>
    #include <boost/preprocessor/repetition/repeat_from_to.hpp>
    #include <boost/preprocessor/repetition/enum.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/arithmetic/inc.hpp>
    #include <boost/preprocessor/facilities/expand.hpp>
    #include <boost/preprocessor/facilities/empty.hpp>
    #include <boost/preprocessor/list/adt.hpp>
    #include <boost/preprocessor/list/at.hpp>
    #include <boost/preprocessor/list/size.hpp>
    #include <boost/preprocessor/tuple/elem.hpp>
#endif


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


// These macros are shorthand for Boost preprocessor macros
#define CAT(S1, S2)                 BOOST_PP_CAT(S1, S2)
#define EMPTY                       BOOST_PP_EMPTY()
#define NIL                         BOOST_PP_NIL
#define LIST_AT(LIST, i)            BOOST_PP_LIST_AT(LIST, i)
#define LIST_SIZE(LIST)             BOOST_PP_LIST_SIZE(LIST)
#define LIST_FIRST(LIST)            BOOST_PP_LIST_FIRST(LIST)
#define LIST_REST(LIST)             BOOST_PP_LIST_REST(LIST)
#define TUPLE_ELEM                  BOOST_PP_TUPLE_ELEM
#define COMMA_IF(COND)              BOOST_PP_COMMA_IF(COND)
#define PARAMS(COUNT, P)            BOOST_PP_ENUM_PARAMS(COUNT, P)
#define PARAMS2(COUNT, P1, P2)      BOOST_PP_ENUM_BINARY_PARAMS(COUNT, P1, P2)
#define REPEAT(COUNT, MACRO, DATA)  BOOST_PP_REPEAT(COUNT, MACRO, DATA)
#define FOR_RANGE(START, END, MACRO, DATA)                                  \
    BOOST_PP_REPEAT_FROM_TO(START, BOOST_PP_INC(END), MACRO, DATA)

    #define EXP_THING(z, n, DATA) BOOST_PP_EXPAND(DATA)

// These are utility macros used by the FOR_* repetition macros
#define EVAL_MACRO1(Z, ITER, MACRO) MACRO(ITER)
#define EVAL_MACRO2(Z, ITER, MACRO) MACRO(ITER)

// This macro expands MACRO(n) for values of n in [0..DIM - 1]
#define FOR_EACH(DIM, MACRO)    REPEAT(DIM, EVAL_MACRO1, MACRO)

// This macro expands MACRO for every dimensionality in [MIN_DIM..MAX_DIM],
// and can be used to generate dimensionality-specific function definitions.
#define FOR_ALL_DIMS(MACRO)     FOR_RANGE(MIN_DIM, MAX_DIM, EVAL_MACRO2, MACRO)

// These macros expect a LIST of 2-TUPLEs, where each tuple is (type, name).
//
#define FORMAL_PARAMS(LIST)                                                 \
    BOOST_PP_ENUM(BOOST_PP_LIST_SIZE(LIST), MAKE_FORMAL_PARAM, LIST)
#define ACTUAL_PARAMS(LIST)                                                 \
    BOOST_PP_ENUM(BOOST_PP_LIST_SIZE(LIST), MAKE_ACTUAL_PARAM, LIST)
#define TRAILING_FORMAL_PARAMS(LIST)                                        \
    BOOST_PP_ENUM_TRAILING(BOOST_PP_LIST_SIZE(LIST), MAKE_FORMAL_PARAM, LIST)
#define TRAILING_ACTUAL_PARAMS(LIST)                                        \
    BOOST_PP_ENUM_TRAILING(BOOST_PP_LIST_SIZE(LIST), MAKE_ACTUAL_PARAM, LIST)

#define MAKE_FORMAL_PARAM(z, n, LIST)                                       \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_LIST_AT(LIST, n))                    \
    BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_LIST_AT(LIST, n))

#define MAKE_ACTUAL_PARAM(z, n, LIST)                                       \
    BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_LIST_AT(LIST, n))


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
#undef EMPTY
#undef NIL
#undef LIST_AT
#undef LIST_SIZE
#undef LIST_FIRST
#undef LIST_REST
#undef TUPLE_ELEM
#undef COMMA_IF
#undef PARAMS
#undef PARAMS2
#undef REPEAT
#undef FOR_RANGE

#undef EVAL_MACRO1
#undef EVAL_MACRO2
#undef FOR_ALL_DIMS
#undef FOR_EACH

#undef FORMAL_PARAMS
#undef ACTUAL_PARAMS
#undef TRAILING_FORMAL_PARAMS
#undef TRAILING_ACTUAL_PARAMS
#undef MAKE_FORMAL_PARAM
#undef MAKE_ACTUAL_PARAM

// Finally, make sure we can re-enable these macros later
#undef UNDEFINE_INCA_MULTI_DIM_MACROS

/*---------------------------------------------------------------------------*/
#endif                                                 /* All done...go home */
/*---------------------------------------------------------------------------*/
