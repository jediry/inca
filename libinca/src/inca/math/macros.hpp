/**
 * File: macros.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains a number of preprocessor macros for the purpose of
 *      simplifying the implementation of array-based algorithms
 *      (i.e., the complex, linear algebra and color types).
 *
 *      Because there is so much common (and verbose!) code among the
 *      algorithms working on these types, the coding of these algorithms is
 *      simplified and made more readable through extensive, systematic use of
 *      the C preprocessor to generate code.
 *
 * Using this file:
 *      Because many of the macros declared herein are one or two letters in
 *      length, the macros should not be allowed to exist any longer than
 *      necessary, lest they replace a regular variable that happens to be
 *      named the same. This file does not have the usual #ifndef multi-include
 *      protection, and thus may be included multiple times. If the macro
 *      UNDEFINE_INCA_MATH_MACROS is defined before including this file then,
 *      rather than the macros being defined, they are undef'd.
 *
 * Using the macros:
 *      The first group of macros are useful for generating class constructors
 *      with parameter lists corresponding to the elements in the array.
 *
 *      XXX Doc me here XXX
 *
 *      The rest of the macros are used to create functions and operators
 *      operating on array-based types.
 *
 *      The actual template parameter list is created by one of the following
 *      macros, depending on the number of needed parameters.
 *          S_TEMPLATE      Template with only a scalar-type parameter
 *          A_TEMPLATE(n)   Template with 'n' distinct dimension parameters
 *          L_TEMPLATE(n)   Template with 'n' distinct colorspace parameters
 *          M_TEMPLATE(n,o) Template with 'n' distinct dimension parameters and
 *                          'o' distinct matrix access/storage order pairs
 *
 *      The array type is the base class for all of the other types (with the
 *      exception of scalar), and so operations defined on it will work for
 *      all array-based types.
 *          A(n)        Array of dimension n
 *
 *      The scalar and complex types don't need a dimensionality parameter,
 *      since their number of elements is intrinsic to their definition.
 *          S           Scalar
 *          S_ARG       Scalar function argument
 *          C           Complex
 *          Q           Quaternion
 *          O           Octonion
 *
 *      The color type implicitly provides the array length via a
 *      colorspace parameter.
 *          L(cs)       Color of colorspace cs
 *
 *      The arbitrary-dimensional linear algebra types take one or more
 *      dimensionality parameters, and possibly a matrix access/storage order
 *      parameter.
 *          P(n)        Point of dimension n
 *          V(n)        Vector of dimension n
 *          M(m,n,o)    Matrix of dimensions m x n, with storage order o
 *
 *      Access to the parameters specified in the template declaration can be
 *      accomplished with the following convenience macros. In each case, the
 *      general X(n) macro also has a number of shortcuts like X0, X1, X2...
 *          D(n)        Dimensionality parameter n
 *          CS(n)       Colorspace parameter n
 *          AS(n)       Linked access/storage order parameter pair n
 *          ASi(a,s)    Independent access/storage order parameter pair a,s
 */


/*---------------------------------------------------------------------------*/
#ifndef UNDEFINE_INCA_MATH_MACROS   /* Define the macros                     */
/*---------------------------------------------------------------------------*/

// Import Boost static assert mechanism
#include <boost/static_assert.hpp>

// Import Boost preprocessor metaprogramming library
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>


/*---------------------------------------------------------------------------*/
/* Variable-length parameter list constructor macros                         */
/*---------------------------------------------------------------------------*/

// These macros control which dimensionalities the constructors are
// generated for.
#define MIN_DIMENSION 2
#define MAX_DIMENSION 9

// This macro is used by the calls to BOOST_PP_REPEAT to assign constructor
// parameters to their corresponding slot in the list
#define ASSIGN_ARG(Z, IDX, JUNK) (*this)[IDX] = e ## IDX;

// This macro is used to instantiate a single variable-arg-count constructor
#define PARAM_LIST_CONSTRUCTOR(Z, DIM, CLASS)                               \
    template <typename scalar2>                                             \
    explicit CLASS(BOOST_PP_ENUM_PARAMS(DIM, scalar2 e)) {                  \
        /* Cause a compile-time failure if this constructor is used with    \
         * an instance of the wrong dimensionality.                         \
         */                                                                 \
        BOOST_STATIC_ASSERT(static_size == DIM);                            \
                                                                            \
        /* Copy each paramter to its slot in the array */                   \
        BOOST_PP_REPEAT(DIM, ASSIGN_ARG, BOOST_PP_EMPTY);                   \
    }

// This macro generates a series of variable-arg-count constructors for the
// specified class, spanning the given range
#define PARAM_LIST_CONSTRUCTORS(CLASS, MIN, MAX)                            \
    BOOST_PP_REPEAT_FROM_TO(MIN, BOOST_PP_INC(MAX),                         \
                            PARAM_LIST_CONSTRUCTOR, CLASS);


// This macro is used to instantiate the List<N>(List<N-1>, scalar) constructors
#define UP_DIM_CONSTRUCTOR(Z, K, CLASS)                                     \
    explicit CLASS(const CLASS<scalar_t, static_size - K> &arr,             \
                   BOOST_PP_REPEAT(K,  )                                    \
        /* Cause a compile-time failure if this constructor is used with    \
         * an instance of the wrong dimensionality.                         \
         */                                                                 \
        BOOST_STATIC_ASSERT(DIM == dimension);                              \
                                                                            \
        /* Copy each paramter to its slot in the array */                   \
        BOOST_PP_REPEAT(DIM, ASSIGN_ARG, BOOST_PP_EMPTY);                   \
    }



/*---------------------------------------------------------------------------*/
/* Template simplification macros                                            */
/*---------------------------------------------------------------------------*/

// Template declaration macros
#define S_TEMPLATE template <typename scalar>

#define A_TEMPLATE(D)                                                       \
    template <typename scalar BOOST_PP_COMMA_IF(D)                          \
              BOOST_PP_ENUM_PARAMS(D, inca::size_t dim)>

#define L_TEMPLATE(CS)                                                      \
    template <typename scalar BOOST_PP_COMMA_IF(D)                          \
              BOOST_PP_ENUM_PARAMS(D, class cs)>

#define M_TEMPLATE(D, M)                                                    \
    template <typename scalar                                               \
              BOOST_PP_COMMA_IF(D) BOOST_PP_ENUM_PARAMS(D, inca::size_t dim)\
              BOOST_PP_COMMA_IF(M) BOOST_PP_ENUM_PARAMS(M, bool access)     \
              BOOST_PP_COMMA_IF(M) BOOST_PP_ENUM_PARAMS(M, bool storage)>


// Array base class type
#define A(n)        boost::array<scalar, n>

// Color type
#define L(cs)        Color<scalar, cs>

// Scalar and complex types
#define S           typename scalar_traits<scalar>::value_type
#define S_ARG       typename scalar_traits<scalar>::arg_type
#define Q           Quaternion<scalar>

// Linear algebra types
#define P(n)        Point<scalar, n>
#define V(n)        Vector<scalar, n>
#define M(m,n,o)    Matrix<scalar, m, n, o>


// Macros for referring to the dimension parameters from the template
// declaration's parameter list
#define D(n)    dim ## n
#define D0      D(0)
#define D1      D(1)
#define D2      D(2)
#define D3      D(3)
#define D4      D(4)
#define D5      D(5)
#define D6      D(6)


// Macros for referring to the colorspace parameters from the template
// declaration's parameter list
#define CS(n)   cs ## n
#define CS0     CS(0)
#define CS1     CS(1)
#define CS2     CS(2)
#define CS3     CS(3)
#define CS4     CS(4)
#define CS5     CS(5)
#define CS6     CS(6)


// Macros for referring to the matrix order parameters from the template
// declaration's parameter list
#define ASb(a,s)    a,s                         // Literal A/S order
#define ASi(a,s)    access ## a,storage ## s    // Independent A/S order
#define AS(n)       access ## n,storage ## n    // Linked A/S order
#define AS0         AS(0)
#define AS1         AS(1)
#define AS2         AS(2)
#define AS3         AS(3)
#define AS4         AS(4)
#define AS5         AS(5)
#define AS6         AS(6)



#define OSTREAM_OP_A(RHS)                                                   \
    ostream & operator<<(ostream &o, const RHS & rhs) {                     \
        return o << string(rhs);                                            \
    }

#define A_ASSIGN_A(LHS, OP, RHS)                                            \
    LHS & operator OP (LHS & lhs, const RHS & rhs) {                        \
        for (index_t i = 0; i < lhs.static_size; i++)                              \
            lhs[i] OP rhs[i];                                               \
        return lhs;                                                         \
    }

#define A_ASSIGN_S(LHS, OP, RHS_DUMMY)                                      \
    LHS & operator OP (LHS & lhs, S_ARG rhs) {                              \
        for (index_t i = 0; i < lhs.static_size; i++)                              \
            lhs[i] OP rhs;                                                  \
        return lhs;                                                         \
    }

#define OP_A(RET, OP, RHS)                                                  \
    RET operator OP (const RHS & rhs) {                                     \
        RET result;                                                         \
        for (index_t i = 0; i < rhs.static_size; i++)                              \
            result[i] = OP rhs[i];                                          \
        return result;                                                      \
    }

#define A_OP_A(RET, LHS, OP, RHS)                                           \
    RET operator OP (const LHS & lhs, const RHS & rhs) {                    \
        RET result;                                                         \
        for (index_t i = 0; i < lhs.static_size; i++)                              \
            result[i] = lhs[i] OP rhs[i];                                   \
        return result;                                                      \
    }

#define A_OP_S(RET, LHS, OP, RHS_DUMMY)                                     \
    RET operator OP (const LHS & lhs, S_ARG rhs) {                          \
        RET result;                                                         \
        for (index_t i = 0; i < lhs.static_size; i++)                              \
            result[i] = lhs[i] OP rhs;                                      \
        return result;                                                      \
    }

#define S_OP_A(RET, LHS_DUMMY, OP, RHS)                                     \
    RET operator OP (S_ARG lhs, const RHS & rhs) {                          \
        RET result;                                                         \
        for (index_t i = 0; i < rhs.static_size; i++)                              \
            result[i] = lhs OP rhs[i];                                      \
        return result;                                                      \
    }

#define A_EQ_A(LHS, RHS)                                                    \
    bool operator == (const LHS & lhs, const RHS & rhs) {                   \
        for (index_t i = 0; i < lhs.static_size; i++)                              \
            if (! effectivelyEqual(lhs[i], rhs[i]))                         \
                return false;                                               \
        return true;                                                        \
    }

#define A_NE_A(LHS, RHS)                                                    \
    bool operator != (const LHS & lhs, const RHS & rhs) {                   \
        return ! (lhs == rhs);                                              \
    }

#define A_FUNC_1A(RET, FUNC, L1)                                            \
    RET FUNC(const L1 & l1) {                                               \
        RET result;                                                         \
        for (index_t i = 0; i < l1.static_size; i++)                               \
            result[i] = FUNC(l1[i]);                                        \
        return result;                                                      \
    }

#define A_FUNC_2A(RET, FUNC, L1, L2)                                        \
    RET FUNC(const L1 & l1, const L2 & l2) {                                \
        RET result;                                                         \
        for (index_t i = 0; i < l1.static_size; i++)                               \
            result[i] = FUNC(l1[i], l2[i]);                                 \
        return result;                                                      \
    }

/*---------------------------------------------------------------------------*/
#else   /* UNDEFINE_INCA_MATH_MACROS was defined, so remove the macros       */
/*---------------------------------------------------------------------------*/

#undef S_TEMPLATE
#undef A_TEMPLATE
#undef L_TEMPLATE
#undef M_TEMPLATE

#undef S
#undef S_ARG
#undef Q

#undef A

#undef L

#undef P
#undef V
#undef M

#undef D
#undef D0
#undef D1
#undef D2
#undef D3
#undef D4
#undef D5
#undef D6

#undef CS
#undef CS0
#undef CS1
#undef CS2
#undef CS3
#undef CS4
#undef CS5
#undef CS6

#undef ASi
#undef AS
#undef AS0
#undef AS1
#undef AS2
#undef AS3
#undef AS4
#undef AS5
#undef AS6

#undef OSTREAM_OP_A
#undef A_ASSIGN_S
#undef A_ASSIGN_A
#undef OP_A
#undef A_OP_A
#undef A_OP_S
#undef S_OP_A
#undef A_EQ_A
#undef A_NE_A
#undef A_FUNC_1A
#undef A_FUNC_2A

// Finally, make sure we can re-enable these macros later
#undef UNDEFINE_INCA_MATH_MACROS

#endif
