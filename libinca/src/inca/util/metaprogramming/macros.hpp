/* -*- C++ -*-
 *
 * File: macros.hpp
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


// Because this file is intended to be repeatedly included and de-included,
// we only want to include these once. Note that ONLY THIS SMALL PART of the
// file is double-include-protected.
#ifndef INCA_METAPROGRAMMING_MACRO_INCLUDES
#define INCA_METAPROGRAMMING_MACRO_INCLUDES
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/comparison.hpp>
    #include <boost/mpl/arithmetic.hpp>
    #include <boost/mpl/logical.hpp>
    #include <boost/mpl/min_max.hpp>
    #include <boost/mpl/identity.hpp>
    #include <boost/type_traits.hpp>
    #include <boost/utility/enable_if.hpp>
#endif


/*---------------------------------------------------------------------------*/
#ifndef UNDEFINE_INCA_METAPROGRAMMING_MACROS            /* Create the macros */
/*---------------------------------------------------------------------------*/

// Whine a little bit if we're re-including this (possibly an error)
#ifndef INCA_METAPROGRAMMING_MACROS
#   define INCA_METAPROGRAMMING_MACROS
#else
#   warning "Macro definitions are being included for a second time. This is probably a coding error."
#endif

// Stuff to keep peace between the preprocessor and the templates
#define COMMA ,

// Handy macros to make the template code less nasty
#define ENABLE_IF(PRED)    typename ::boost::enable_if< PRED, void >::type
#define DISABLE_IF(PRED)   typename ::boost::disable_if< PRED, void >::type
#define ENABLE_IF_C(COND)  typename ::boost::enable_if_c< COND, void >::type
#define DISABLE_IF_C(COND) typename ::boost::disable_if_c< COND, void >::type
#define ENABLE_IF_T(PRED, T)   typename ::boost::enable_if< PRED, T >::type
#define DISABLE_IF_T(PRED, T)  typename ::boost::disable_if< PRED, T >::type
#define ENABLE_IF_CT(COND, T)  typename ::boost::enable_if_c< COND, T >::type
#define DISABLE_IF_CT(COND, T) typename ::boost::disable_if_c< COND, T >::type
#define ENABLE_FUNCTION_IF(PRED)    typename ::boost::enable_if< PRED, void >::type * dummy = NULL
#define DISABLE_FUNCTION_IF(PRED)   typename ::boost::disable_if< PRED, void >::type * dummy = NULL
#define ENABLE_FUNCTION_IF_C(COND)  typename ::boost::enable_if_c< COND, void >::type * dummy = NULL
#define DISABLE_FUNCTION_IF_C(COND) typename ::boost::disable_if_c< COND, void >::type * dummy = NULL

#define LAZY_ENABLE_IF(X, PRED)    typename ::boost::enable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED >, void >::type
#define LAZY_DISABLE_IF(X, PRED)   typename ::boost::disable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED >, void >::type
#define LAZY_ENABLE_IF_C(X, COND)  typename ::boost::enable_if_c< DEFERRED_TRUE(X) && COND, void >::type
#define LAZY_DISABLE_IF_C(X, COND) typename ::boost::disable_if_c< DEFERRED_TRUE(X) && COND, void >::type
#define LAZY_ENABLE_IF_T(X, PRED, T)   typename ::boost::enable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED > , T >::type
#define LAZY_DISABLE_IF_T(X, PRED, T)  typename ::boost::disable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED > , T >::type
#define LAZY_ENABLE_IF_CT(X, COND, T)  typename ::boost::enable_if_c< DEFERRED_TRUE(X)::value && COND, T >::type
#define LAZY_DISABLE_IF_CT(X, COND, T) typename ::boost::disable_if_c< DEFERRED_TRUE(X)::value && COND, T >::type
#define LAZY_ENABLE_FUNCTION_IF(X, PRED)    typename ::boost::enable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED >, void >::type * dummy = NULL
#define LAZY_DISABLE_FUNCTION_IF(X, PRED)   typename ::boost::disable_if< ::boost::mpl::and_< DEFERRED_TRUE(X), PRED >, void >::type * dummy = NULL
#define LAZY_ENABLE_FUNCTION_IF_C(X, COND)  typename ::boost::enable_if_c< DEFERRED_TRUE(X)::value && COND, void >::type * dummy = NULL
#define LAZY_DISABLE_FUNCTION_IF_C(X, COND) typename ::boost::disable_if_c< DEFERRED_TRUE(X)::value && COND, void >::type * dummy = NULL

#define IF(P, T, E)         ::boost::mpl::if_< P COMMA T COMMA E >::type
#define NOT(P)              ::boost::mpl::not_< P >
#define OR2(P1, P2)         ::boost::mpl::or_< P1 COMMA P2 >
#define OR3(P1, P2, P3)     ::boost::mpl::or_< P1 COMMA P2 COMMA P3 >
#define OR4(P1, P2, P3, P4) ::boost::mpl::or_< P1 COMMA P2 COMMA P3 COMMA P4 >
#define AND2(P1, P2)        ::boost::mpl::and_< P1 COMMA P2 >
#define AND3(P1, P2, P3)    ::boost::mpl::and_< P1 COMMA P2 COMMA P3 >
#define AND4(P1, P2, P3, P4)::boost::mpl::and_< P1 COMMA P2 COMMA P3 COMMA P4 >

#define MIN2(P1, P2)        ::boost::mpl::min< P1 COMMA P2 >::type
#define MIN3(P1, P2, P3)    ::boost::mpl::min< typename ::boost::mpl::min< P1 COMMA P2 >::type COMMA P3 >::type
#define MIN4(P1, P2, P3, P4)::boost::mpl::min< typename ::boost::mpl::min< P1 COMMA P2 >::type COMMA \
                                               typename ::boost::mpl::min< P3 COMMA P4 >::type >::type
#define MAX2(P1, P2)        ::boost::mpl::max< P1 COMMA P2 >::type
#define MAX3(P1, P2, P3)    ::boost::mpl::max< typename ::boost::mpl::max< P1 COMMA P2 >::type COMMA P3 >::type
#define MAX4(P1, P2, P3, P4)::boost::mpl::max< typename ::boost::mpl::max< P1 COMMA P2 >::type COMMA \
                                               typename ::boost::mpl::max< P3 COMMA P4 >::type >::type

#define DEFERRED_TRUE(X)    ::boost::is_same< X COMMA X >
#define DEFERRED_FALSE(X)   ::boost::mpl::not_< ::boost::is_same< X COMMA X > >
#define IDENTITY(T)         ::boost::mpl::identity<T>
#define BOOLEAN(B)          ::boost::mpl::bool_<B>
#define INT(I)              ::boost::mpl::int_<I>

#define DERIVED_FROM(C, BASE)   ::boost::is_base_and_derived< BASE COMMA C >
#define IS_SAME(C1, C2)         ::boost::is_same< C1 COMMA C2 >
#define IS_A(C, B)              ::boost::mpl::or_< ::boost::is_same< B COMMA C > COMMA              \
                                                   ::boost::is_base_and_derived< B COMMA C > >
#define IS_CONST(C)             ::boost::is_const< C >

#define EQUAL(P1, P2)       typename ::boost::mpl::equal_to< P1 COMMA P2 >::type
#define LESS(P1, P2)        typename ::boost::mpl::less< P1 COMMA P2 >::type
#define PLUS(P1, P2)        typename ::boost::mpl::plus< P1 COMMA P2 >::type
#define MINUS(P1, P2)       typename ::boost::mpl::minus< P1 COMMA P2 >::type
#define TIMES(P1, P2)       typename ::boost::mpl::times< P1 COMMA P2 >::type
#define DIVIDES(P1, P2)     typename ::boost::mpl::divides< P1 COMMA P2 >::type
#define MODULUS(P1, P2)     typename ::boost::mpl::modulus< P1 COMMA P2 >::type
#define NEGATE(P1)          typename ::boost::mpl::negate< P1 >::type

#define BOOLEAN_METAFUNCTION_C(NAME, COND)                                  \
    template <class T, typename Enable = void>                              \
    struct NAME {                                                           \
        typedef ::boost::mpl::false_ type;                                  \
        static const bool value = type::value;                              \
    };                                                                      \
    template <class T>                                                      \
    struct NAME<T, ENABLE_IF_C( (COND) )> {                                 \
        typedef ::boost::mpl::true_ type;                                   \
        static const bool value = type::value;                              \
    };

#define BOOLEAN_METAFUNCTION(NAME, PRED)                                    \
    BOOLEAN_METAFUNCTION_C(NAME, (PRED::value) )



/*---------------------------------------------------------------------------*/
#else    /* UNDEFINE_INCA_METAPROGRAMMING_MACROS was defined, so remove them */
/*---------------------------------------------------------------------------*/

#undef COMMA

#undef ENABLE_IF
#undef DISABLE_IF
#undef ENABLE_IF_C
#undef DISABLE_IF_C
#undef ENABLE_IF_T
#undef DISABLE_IF_T
#undef ENABLE_IF_CT
#undef DISABLE_IF_CT
#undef ENABLE_FUNCTION_IF
#undef DISABLE_FUNCTION_IF
#undef ENABLE_FUNCTION_IF_C
#undef DISABLE_FUNCTION_IF_C

#undef LAZY_ENABLE_IF
#undef LAZY_DISABLE_IF
#undef LAZY_ENABLE_IF_C
#undef LAZY_DISABLE_IF_C
#undef LAZY_ENABLE_IF_T
#undef LAZY_DISABLE_IF_T
#undef LAZY_ENABLE_IF_CT
#undef LAZY_DISABLE_IF_CT
#undef LAZY_ENABLE_FUNCTION_IF
#undef LAZY_DISABLE_FUNCTION_IF
#undef LAZY_ENABLE_FUNCTION_IF_C
#undef LAZY_DISABLE_FUNCTION_IF_C

#undef IF
#undef NOT
#undef OR2
#undef OR3
#undef OR4
#undef AND2
#undef AND3
#undef AND4

#undef DEFERRED_TRUE
#undef DEFERRED_FALSE
#undef IDENTITY
#undef BOOLEAN
#undef INT

#undef DERIVED_FROM
#undef IS_SAME
#undef IS_A
#undef IS_CONST

#undef EQUAL
#undef LESS_THAN
#undef PLUS
#undef MINUS
#undef TIMES
#undef DIVIDES
#undef MODULUS
#undef NEGATE

#undef BOOLEAN_METAFUNCTION
#undef BOOLEAN_METAFUNCTION_C

// Finally, make sure we can re-enable these macros later
#undef UNDEFINE_INCA_METAPROGRAMMING_MACROS
#undef INCA_METAPROGRAMMING_MACROS

/*---------------------------------------------------------------------------*/
#endif                                                 /* All done...go home */
/*---------------------------------------------------------------------------*/
