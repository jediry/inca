/*
 * File: color-type-traits.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements type traits for color types.
 *
 *      This file is not intended to be used by itself, but rather to be
 *      included indirectly, via ../type-traits.hpp. See that file for further
 *      documentation.
 */

#ifndef INCA_MATH_COLOR_TYPE_TRAITS
#define INCA_MATH_COLOR_TYPE_TRAITS

// This is part of the Inca math library
namespace inca {
    namespace math {
    /*-----------------------------------------------------------------------*
     | IsColor metafunction
     *-----------------------------------------------------------------------*/
        template <typename C> struct IsColor {
            static const bool value = false;
        };
        template <typename S, typename CS> struct IsColor< Color<S, CS> > {
            static const bool value = true;
        };


    /*-----------------------------------------------------------------------*
     | Promotion traits for color types
     *-----------------------------------------------------------------------*/
        // Empty general case
        template <typename S> struct ColorSpacePromote { };

        // Helper metafunction for chosing the correct colorspace and alpha
        template <typename CS1, typename CS2>
        struct ColorSpacePromote { /* Empty default version */ };

        // Identical colorspaces
        template <typename CS>
        struct ColorSpacePromote<CS, CS> { typedef CS Type; };

        // Colorspaces differing only by alpha
        template <template <bool alpha> CS>
            struct ColorSpacePromote< CS<true>, CS<false> > {
                typedef CS<true> Type;
            };
        template <template <bool alpha> CS>
            struct ColorSpacePromote< CS<false>, CS<true> > {
                typedef CS<true> Type;
            };

        // Two color arguments
        template <typename C1, typename C2>
        struct PromotionTraits<C1, C2,
                ENABLE_IF( AND2( IsColor<C1>, IsColor<C2> )) > {
            typedef ScalarPromote<typename C1::scalar_t,
                                  typename C2::scalar_t>::Type      ScalarType;
            typedef ColorSpacePromote<typename C1::ColorSpace,
                                      typename C2::ColorSpace>::Type ColorSpaceType;
            typedef Color<ScalarType, ColorSpaceType> PromoteType;
        };
        
        // One color, one scalar
        template <typename C, typename S>
        struct PromotionTraits<C, S, ENABLE_IF( AND2( IsColor<C>,
                                                      IsScalar<S> )) > {
            typedef ScalarPromote<typename C::scalar_t, S>::Type    ScalarType;
            typedef Color<ScalarType, typename C::ColorSpace>       PromoteType;
        };
        template <typename S, typename C>
        struct PromotionTraits<S, C, ENABLE_IF( AND2( IsScalar<S>,
                                                      IsColor<C> )) >
            : public PromotionTraits<C, S> { };


    /*-----------------------------------------------------------------------*
     | Promotion traits for color types
     *-----------------------------------------------------------------------*/
        // Unary negation is a Color
        template <typename C>
        struct UnaryOperationTraits< Color<S, CS>, ArithmeticOperation<Negation> > {
            typedef Color<S, CS> ReturnType;
        };

        // Other 

        // All the power operations work with colors and scalars
        template <typename C, typename Op>
        struct UnaryOperationTraits< C, PowerOperation<Op> >
        template<class Act, typename Scalar1, typename Scalar2, class ColorSpace>
        struct plain_return_type_2<arithmetic_action<Act>,
                                   ::inca::math::Color<Scalar1, ColorSpace>,
                                   ::inca::math::Color<Scalar2, ColorSpace> > {
            // First, figure out what the resulting scalar type should be
            typedef typename return_type_2<arithmetic_action<Act>,
                                                 Scalar1, Scalar2>::type scalar;

            // Now we can tell you the return type
            typedef inca::math::Color<scalar, ColorSpace> type;
        };

        template<class Act, typename Scalar1, typename Scalar2, class ColorSpace>
        struct plain_return_type_2<arithmetic_action<Act>,
                                   ::inca::math::Color<Scalar1, ColorSpace>,
                                   Scalar2> {
            // First, figure out what the resulting scalar type should be
            typedef typename return_type_2<arithmetic_action<Act>,
                                                 Scalar1, Scalar2>::type scalar;

            // Now we can tell you the return type
            typedef inca::math::Color<scalar, ColorSpace> type;
        };

        template<class Act, typename Scalar1, typename Scalar2, class ColorSpace>
        struct plain_return_type_2<arithmetic_action<Act>, Scalar1,
                                   ::inca::math::Color<Scalar2, ColorSpace> > {

            // First, figure out what the resulting scalar type should be
            typedef typename return_type_2<arithmetic_action<Act>,
                                                 Scalar1, Scalar2>::type scalar;

            // Now we can tell you the return type
            typedef inca::math::Color<scalar, ColorSpace> type;