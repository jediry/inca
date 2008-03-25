/*
 * File: type-traits.hpp
 * 
 * Author: Ryan L. Saunders
 *      Thanks to Jaakko Järvi & Gary Powell of the Boost Lambda Library
 *         and to Ullrich Köthe of the VIGRA Computer Vision Library
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements a simple, template-based mechanism for deducing
 *      the appropriate result type of an operation based on the types of
 *      the operands and the sort of operation being done. It is patterned
 *      after a similar mechanism implemented in the Boost Lambda Library (BLL),
 *      with some influence from the VIGRA image processing library.
 *
 *      Neither BLL's nor VIGRA's implementation is used herein, for various
 *      reasons. BLL's suffers from an annoying late-failure policy in which
 *      incompatible type combinations(e.g., int * std::string) result in a
 *      valid result type expression (boost::lambda::detail::unspecified),
 *      which delays the compilation failure until later (when the compiler
 *      attempts to convert 'unspecified' to the assigned-to type. VIGRA
 *      is not considered a hard Inca dependency, and thus cannot be used
 *      to implement core functionality.
 */

#pragma once
#ifndef INCA_MATH_TYPE_TRAITS
#define INCA_MATH_TYPE_TRAITS

// This is part of the Inca math library
namespace inca {
    namespace math {

        // General traits templates (all types undefined)
        template <typename S1,
                  typename S2,
                  typename Enabled = void>
            struct PromotionTraits { };

        template <typename T1,
                  typename Action,
                  typename Enabled = void>
            struct UnaryOperationTraits { };

        template <typename T1, typename T2,
                  typename Action,
                  typename Enabled = void>
            struct BinaryOperationTraits { };


        // Arithmetic operations
        template <typename Op> struct ArithmeticOperation { };
            struct Negation { };
            struct Sum { };
            struct Difference { };
            struct Product { };
            struct Division { };
            struct Modulus { };

            struct Minimum { };
            struct Maximum { };
            struct Magnitude { };           // a.k.a. AbsoluteValue
            struct Floor { };
            struct Ceiling { };
            struct Round { };
            struct Truncate { };

        // Power operations
        template <typename Op> struct PowerOperation { };
            struct NaturalLogarithm { };
            struct Exponential { };
            struct Logarithm { };
            struct Power { };
            struct SquareRoot { };
            struct CubeRoot { };

        // Trigonometric operations
        template <typename Op> struct TrigonometricOperation { };
            struct Sine { };
            struct Cosine { };
            struct Tangent { };
            struct Cotangent { };
            struct Secant { };
            struct Cosecant { };
            struct InverseSine { };
            struct InverseCosine { };
            struct InverseTangent { };
            struct InverseCotangent { };
            struct InverseSecant { };
            struct InverseCosecant { };

        // Hyperbolic operations
        template <typename Op> struct HyperbolicOperation { };
            struct HyperbolicSine { };
            struct HyperbolicCosine { };
            struct HyperbolicTangent { };
            struct HyperbolicCotangent { };
            struct HyperbolicSecant { };
            struct HyperbolicCosecant { };
            struct InverseHyperbolicSine { };
            struct InverseHyperbolicCosine { };
            struct InverseHyperbolicTangent { };
            struct InverseHyperbolicCotangent { };
            struct InverseHyperbolicSecant { };
            struct InverseHyperbolicCosecant { };

        // Complex operations
        template <typename Op> struct ComplexOperation { };
            struct Magnitude { };           // a.k.a. AbsoluteValue
            struct Angle { };               // a.k.a. Phase, Argument
            struct Conjugate { };
            struct RealPart { };
            struct ImaginaryPart { };

        // Vector operations
        template <typename Op> struct VectorOperation { };
            //struct Magnitude { };
            //struct Angle { };
            struct DotProduct { };
            struct CrossProduct { };

    };
};


// Import traits for various families of types
#include "scalar/scalar-type-traits.hpp"
#include "complex/complex-type-traits.hpp"
#include "linalg/linalg-type-traits.hpp"
#include "color/color-type-traits.hpp"

#endif
