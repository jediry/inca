/*
 * File: scalar-type-traits.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements type traits for scalar types.
 *
 *      This file is not intended to be used by itself, but rather to be
 *      included indirectly, via ../type-traits.hpp. See that file for further
 *      documentation.
 */

#ifndef INCA_MATH_SCALAR_TYPE_TRAITS
#define INCA_MATH_SCALAR_TYPE_TRAITS

// This is part of the Inca math library
namespace inca {
    namespace math {
    /*-----------------------------------------------------------------------*
     | IsScalar metafunction
     *-----------------------------------------------------------------------*/
        template <typename S> struct IsScalar {
            static const bool value = false;
        };
        template <> struct IsScalar<bool> {
            static const bool value = true;
        };
        template <> struct IsScalar<signed char> {
            static const bool value = true;
        };
        template <> struct IsScalar<char> {
            static const bool value = true;
        };
        template <> struct IsScalar<unsigned char> {
            static const bool value = true;
        };
        template <> struct IsScalar<short> {
            static const bool value = true;
        };
        template <> struct IsScalar<unsigned short> {
            static const bool value = true;
        };
        template <> struct IsScalar<int> {
            static const bool value = true;
        };
        template <> struct IsScalar<unsigned int> {
            static const bool value = true;
        };
        template <> struct IsScalar<long> {
            static const bool value = true;
        };
        template <> struct IsScalar<unsigned long> {
            static const bool value = true;
        };
        template <> struct IsScalar<float> {
            static const bool value = true;
        };
        template <> struct IsScalar<double> {
            static const bool value = true;
        };
        template <> struct IsScalar<long double> {
            static const bool value = true;
        };

        
    /*-----------------------------------------------------------------------*
     | Promotion traits for scalar types
     *-----------------------------------------------------------------------*/
        // Empty general case
        template <typename S> struct ScalarPromoteCode { };

        // Promotion codes for builtin types
        template <> struct ScalarPromoteCode<bool> {
            static const int value = 10;
        };
        template <> struct ScalarPromoteCode<unsigned char> {
            static const int value = 20;
        };
        template <> struct ScalarPromoteCode<char> {
            static const int value = 30;
        };
        template <> struct ScalarPromoteCode<signed char> {
            static const int value = 40;
        };
        template <> struct ScalarPromoteCode<unsigned short> {
            static const int value = 50;
        };
        template <> struct ScalarPromoteCode<short> {
            static const int value = 60;
        };
        template <> struct ScalarPromoteCode<unsigned int> {
            static const int value = 70;
        };
        template <> struct ScalarPromoteCode<int> {
            static const int value = 80;
        };
        template <> struct ScalarPromoteCode<unsigned long> {
            static const int value = 90;
        };
        template <> struct ScalarPromoteCode<long> {
            static const int value = 100;
        };
        template <> struct ScalarPromoteCode<float> {
            static const int value = 110;
        };
        template <> struct ScalarPromoteCode<double> {
            static const int value = 120;
        };
        template <> struct ScalarPromoteCode<long double> {
            static const int value = 130;
        };

        // Helper metafunction for chosing the correct type based on code
        template <typename S1, typename S2, typename Enabled = void>
        struct ScalarPromote {
            typedef S1 Type;
        };
        template <typename S1, typename S2>
        struct ScalarPromote<S1, S2, ENABLE_IF_C( LESS_THAN(ScalarPromoteCode<S1>::value,
                                                            ScalarPromoteCode<S2>::value ))> {
            typedef S2 Type;
        };

        // Promotion traits for scalar types
        template <typename S1, typename S2>
        struct PromotionTraits<S1, S2, ENABLE_IF( AND2( IsScalar<S1>,
                                                        IsScalar<S2> )) {
            typedef ScalarPromote<S1, S2>::Type PromoteType;
        };


    /*-----------------------------------------------------------------------*
     | Operation return types
     *-----------------------------------------------------------------------*/

        // Scalar unary scalar arithmetic operation
        template <typename T1, typename A>
        struct UnaryOperationTraits< T1, ArithmeticAction<A>,
                                    ENABLE_IF( IsScalar<T1> )> {
            typedef T1 ReturnType;
        }:

        // Homogeneous binary scalar arithmetic operation
        template <typename T1, typename A>
        struct BinaryOperationTraits< T1, T1, ArithmeticAction<A>,
                                     ENABLE_IF( IsScalar<T1> )> {
            typedef T1 ReturnType;
        }:

        // Heterogeneous binary scalar arithmetic operation
        template <typename T1, typename T2, typename A>
        struct BinaryOperationTraits< T1, T2, ArithmeticAction<A>,
                                     ENABLE_IF( AND2( IsScalar<T1>,
                                                      IsScalar<T2> ) )> {
            typedef PromotionTraits<T1, T2>::ReturnType ReturnType;
        }:    
    };
};


#endif
