/* -*- C++ -*-
 *
 * File: template-metaprogramming-macros.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains a number of preprocessor macros for the purpose of
 *      simplifying the implementation of template metaprograms.
 *
 *      Because template metaprograms typically involve nasty, convoluted,
 *      nested template parameters, the coding of these expressions is
 *      simplified and made more readable through extensive, systematic use of
 *      the C preprocessor to generate code.
 *
 * Using this file:
 *      Because many of the macros declared herein have fairly short,
 *      non-"namespaced" names, they should not be allowed to exist any longer
 *      than necessary, lest they conflict with other macros or variables.
 *      This file does not have the usual #ifndef...#endif multi-include
 *      protection, and thus may be included multiple times. If the macro
 *      UNDEFINE_INCA_METAPROGRAMMING_MACROS is defined when this file is
 *      included then, instead of defining the macros, it #undef's them (and
 *      UNDEFINE_INCA_METAPROGRAMMING_MACROS as well).
 */

/*---------------------------------------------------------------------------*/
#ifndef UNDEFINE_INCA_METAPROGRAMMING_MACROS            /* Create the macros */
/*---------------------------------------------------------------------------*/

// Import Boost template metaprogramming utilities
#include <boost/mpl/logical.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

// Stuff to keep peace between the preprocessor and the templates
#define COMMA ,

// Handy macros to make the template code less nasty
#define ENABLE_IF_C(CONDITION)  typename ::boost::enable_if_c< CONDITION >::type
#define DISABLE_IF_C(CONDITION) typename ::boost::disable_if_c< CONDITION >::type
#define ENABLE_IF(PREDICATE)    typename ::boost::enable_if< PREDICATE >::type
#define DISABLE_IF(PREDICATE)   typename ::boost::disable_if< PREDICATE >::type
#define ENABLE_IF_CT(CONDITION, T)  typename ::boost::enable_if_c< CONDITION, T >::type
#define DISABLE_IF_CT(CONDITION, T) typename ::boost::disable_if_c< CONDITION, T >::type
#define ENABLE_IF_T(PREDICATE, T)   typename ::boost::enable_if< PREDICATE, T >::type
#define DISABLE_IF_T(PREDICATE, T)  typename ::boost::disable_if< PREDICATE, T >::type
#define ENABLE_FUNCTION_IF_C(CONDITION)  typename ::boost::enable_if_c< CONDITION >::type * dummy = NULL
#define DISABLE_FUNCTION_IF_C(CONDITION) typename ::boost::disable_if_c< CONDITION >::type * dummy = NULL
#define ENABLE_FUNCTION_IF(PREDICATE)    typename ::boost::enable_if< PREDICATE >::type * dummy = NULL
#define DISABLE_FUNCTION_IF(PREDICATE)   typename ::boost::disable_if< PREDICATE >::type * dummy = NULL

#define IF(P, T, E)         ::boost::mpl::if_< P, T, E >
#define NOT(P)              ::boost::mpl::not_< P >
#define OR2(P1, P2)         ::boost::mpl::or_< P1 , P2 >
#define OR3(P1, P2, P3)     ::boost::mpl::or_< P1 , P2 , P3 >
#define OR4(P1, P2, P3, P4) ::boost::mpl::or_< P1 , P2 , P3 , P4 >
#define AND2(P1, P2)        ::boost::mpl::and_< P1 , P2 >
#define AND3(P1, P2, P3)    ::boost::mpl::and_< P1 , P2 , P3 >
#define AND4(P1, P2, P3, P4)::boost::mpl::and_< P1 , P2 , P3 , P4 >

#define IDENTITY(T)         ::boost::mpl::identity<T>

#define DERIVED_FROM(C, BASE)   ::boost::is_base_and_derived< BASE, C >
#define IS_SAME(C1, C2)         ::boost::is_same< C1 , C2 >


#define BOOL_METAFUNCTION_C(NAME, CONDITION)                                \
    template <class T, typename enable = void>                              \
    struct NAME {                                                           \
        typedef ::boost::mpl::false_ type;                                  \
        static const bool value = type::value;                              \
    };                                                                      \
    template <class T>                                                      \
    struct NAME<T, ENABLE_IF_C( (CONDITION) )> {                            \
        typedef ::boost::mpl::true_ type;                                   \
        static const bool value = type::value;                              \
    };

#define BOOL_METAFUNCTION(NAME, PREDICATE)                                  \
    BOOL_METAFUNCTION_C(NAME, PREDICATE::value)



/*---------------------------------------------------------------------------*/
#else    /* UNDEFINE_INCA_METAPROGRAMMING_MACROS was defined, so remove them */
/*---------------------------------------------------------------------------*/

#undef COMMA

#undef ENABLE_IF_C
#undef DISABLE_IF_C
#undef ENABLE_IF_CT
#undef DISABLE_IF_CT
#undef ENABLE_IF
#undef DISABLE_IF
#undef ENABLE_IF_T
#undef DISABLE_IF_T
#undef ENABLE_FUNCTION_IF
#undef DISABLE_FUNCTION_IF

#undef IF
#undef NOT
#undef OR2
#undef OR3
#undef AND2
#undef AND3

#undef IDENTITY
#undef DERIVED_FROM

#undef BOOL_METAFUNCTION

// Finally, make sure we can re-enable these macros later
#undef UNDEFINE_INCA_METAPROGRAMMING_MACROS

/*---------------------------------------------------------------------------*/
#endif                                                 /* All done...go home */
/*---------------------------------------------------------------------------*/
