/*
 * File: math.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      mathematical primitive types, which live in the inca::math namespace.
 *
 *      In the inca::math library (as well as most of the other Inca libraries),
 *      how scalar math done is controlled by the \a scalar template parameter
 *      that is used to instantiate the classes. This scalar type must have the
 *      usual arithmetic operators implemented for it, and should specialize
 *      the scalar operations defined in math/scalar_ops. This permits
 *      specialized, more exact mathematical representations to peacefully
 *      coexist alongside ordinary, hardware-supported math types
 *      (int, float, double, etc.).
 */

#ifndef INCA_MATH
#define INCA_MATH

// Scalar types, operators, and functions
#include "math/scalar.hpp"

// Linear algebra types, operators, and functions
#include "math/linalg.hpp"

// Color types, operators, and functions
#include "math/color.hpp"


// Number generators
#include "math/generator/RandomUniform"
#include "math/generator/RandomGaussian"

#if 0
// Parametric space-curve definitions
#include "curve/ParametricCurve"
#include "curve/ControlPointCurve"
#include "curve/PolyLineCurve"
#include "curve/BezierCurve"
#include "curve/BSplineCurve"


// Parametric surface definitions
#include "surface/ParametricSurface"
#include "surface/ControlPointSurface"
#include "surface/PolyQuadSurface"
#include "surface/BezierSurface"
#include "surface/BSplineSurface"


// Implicit surface definitions
//#include "surface/ImplicitSurface"
//#include "surface/PolynomialSurface"
#include "surface/Plane"

#endif

// Analytic solid definitions
#include "math/solid/Sphere"
#include "math/solid/Block"

#endif
