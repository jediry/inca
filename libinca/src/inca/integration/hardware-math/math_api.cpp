/** -*- C++ -*-
 *
 * File: math_api
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include <inca/math/scalar/scalar_ops>
using namespace inca::math;

// Import the ANSI/IEEE math functions
#include <math.h>

// Make some nice aliases for the template classes we'll be filling out
typedef scalar_arithmetic_api<int>              i_arith_api;
typedef scalar_arithmetic_api<unsigned int>     ui_arith_api;
typedef scalar_arithmetic_api<long>             l_arith_api;
typedef scalar_arithmetic_api<unsigned long>    ul_arith_api;
typedef scalar_arithmetic_api<float>            f_arith_api;
typedef scalar_arithmetic_api<double>           d_arith_api;
typedef scalar_arithmetic_api<long double>      ld_arith_api;
typedef scalar_algebraic_api<float>             f_alg_api;
typedef scalar_algebraic_api<double>            d_alg_api;
typedef scalar_algebraic_api<long double>       ld_alg_api;
typedef scalar_transcendental_api<float>        f_trans_api;
typedef scalar_transcendental_api<double>       d_trans_api;
typedef scalar_transcendental_api<long double>  ld_trans_api;


/*---------------------------------------------------------------------------*
 | Integer absolute value
 *---------------------------------------------------------------------------*/
template<>
int i_arith_api::abs(arg_type s) { return ::abs(s); }
template<>
unsigned int ui_arith_api::abs(arg_type s) { return s; }
template<>
long l_arith_api::abs(arg_type s) { return ::abs(s); }
template<>
unsigned long ul_arith_api::abs(arg_type s) { return s; }


/*---------------------------------------------------------------------------*
 | Floating-point modulus
 *---------------------------------------------------------------------------*/
template <>
float       f_arith_api::modulus(arg_type s1, arg_type s2) {
    return ::fmodf(s1, s2);
}
template <>
double      d_arith_api::modulus(arg_type s1, arg_type s2) {
    return ::fmod(s1, s2);
}
template <>
long double ld_arith_api::modulus(arg_type s1, arg_type s2) {
    return ::fmodl(s1, s2);
}


/*---------------------------------------------------------------------------*
 | Floating-point absolute value
 *---------------------------------------------------------------------------*/
template<>
float       f_arith_api::abs(arg_type s) { return ::fabsf(s); }
template<>
double      d_arith_api::abs(arg_type s) { return ::fabs(s); }
template<>
long double ld_arith_api::abs(arg_type s) { return ::fabsl(s); }


/*---------------------------------------------------------------------------*
 | Floating-point rounding
 *---------------------------------------------------------------------------*/
template<>
float       f_arith_api::floor(arg_type s) { return ::floorf(s); }
template<>
double      d_arith_api::floor(arg_type s) { return ::floor(s); }
template<>
long double ld_arith_api::floor(arg_type s) { return ::floorl(s); }


template<>
float       f_arith_api::ceiling(arg_type s) { return ::ceilf(s); }
template<>
double      d_arith_api::ceiling(arg_type s) { return ::ceil(s); }
template<>
long double ld_arith_api::ceiling(arg_type s) { return ::ceill(s); }


template<>
float       f_arith_api::round(arg_type s) { return ::roundf(s); }
template<>
double      d_arith_api::round(arg_type s) { return ::round(s); }
template<>
long double ld_arith_api::round(arg_type s) { return ::roundl(s); }


/*---------------------------------------------------------------------------*
 | Floating-point square root
 *---------------------------------------------------------------------------*/
template<>
float       f_alg_api::sqrt(arg_type s) { return ::sqrtf(s); }
template<>
double      d_alg_api::sqrt(arg_type s) { return ::sqrt(s); }
template<>
long double ld_alg_api::sqrt(arg_type s) { return ::sqrtl(s); }


/*---------------------------------------------------------------------------*
 | Floating-point values of PI and E
 *---------------------------------------------------------------------------*/
template<> float        f_trans_api::PI() { return float(M_PI); }
template<> double       d_trans_api::PI() { return M_PI; }
template<> long double ld_trans_api::PI() { return M_PIl; }

template<> float        f_trans_api::E() { return float(M_E); }
template<> double       d_trans_api::E() { return M_E; }
template<> long double ld_trans_api::E() { return M_El; }


/*---------------------------------------------------------------------------*
 | Floating-point power/log functions
 *---------------------------------------------------------------------------*/
template<>
float       f_trans_api::pow(arg_type base, arg_type power) { return ::powf(base, power); }
template<>
double      d_trans_api::pow(arg_type base, arg_type power) { return ::pow(base, power); }
template<>
long double ld_trans_api::pow(arg_type base, arg_type power) { return ::powl(base, power); }

template<>
float       f_trans_api::log(arg_type base, arg_type s) { return ::logf(s) / ::logf(base); }
template<>
double      d_trans_api::log(arg_type base, arg_type s) { return ::log(s) / ::log(base); }
template<>
long double ld_trans_api::log(arg_type base, arg_type s) { return ::logl(s) / ::logl(base); }

template<>
float       f_trans_api::exp(arg_type power) { return ::expf(power); }
template<>
double      d_trans_api::exp(arg_type power) { return ::exp(power); }
template<>
long double ld_trans_api::exp(arg_type power) { return ::expl(power); }

template<>
float       f_trans_api::ln(arg_type s) { return ::logf(s); }
template<>
double      d_trans_api::ln(arg_type s) { return ::log(s); }
template<>
long double ld_trans_api::ln(arg_type s) { return ::logl(s); }


/*---------------------------------------------------------------------------*
 | Floating-point trigonometric functions
 *---------------------------------------------------------------------------*/
// Sine
template<>
float       f_trans_api::sin(arg_type s) { return ::sinf(s); }
template<>
double      d_trans_api::sin(arg_type s) { return ::sin(s); }
template<>
long double ld_trans_api::sin(arg_type s) { return ::sinl(s); }

// Cosine
template<>
float       f_trans_api::cos(arg_type s) { return ::cosf(s); }
template<>
double      d_trans_api::cos(arg_type s) { return ::cos(s); }
template<>
long double ld_trans_api::cos(arg_type s) { return ::cosl(s); }

// Tangent
template<>
float       f_trans_api::tan(arg_type s) { return ::tanf(s); }
template<>
double      d_trans_api::tan(arg_type s) { return ::tan(s); }
template<>
long double ld_trans_api::tan(arg_type s) { return ::tanl(s); }

// Cotangent
template<>
float       f_trans_api::cot(arg_type s) { return 1.0f / ::tanf(s); }
template<>
double      d_trans_api::cot(arg_type s) { return 1.0 / ::tan(s); }
template<>
long double ld_trans_api::cot(arg_type s) { return 1.0L / ::tanl(s); }

// Secant
template<>
float       f_trans_api::sec(arg_type s) { return 1.0f / ::cosf(s); }
template<>
double      d_trans_api::sec(arg_type s) { return 1.0 / ::cos(s); }
template<>
long double ld_trans_api::sec(arg_type s) { return 1.0L / ::cosl(s); }

// Cosecant
template<>
float       f_trans_api::csc(arg_type s) { return 1.0f / ::sinf(s); }
template<>
double      d_trans_api::csc(arg_type s) { return 1.0 / ::sin(s); }
template<>
long double ld_trans_api::csc(arg_type s) { return 1.0L / ::sinl(s); }


/*---------------------------------------------------------------------------*
 | Floating-point inverse trigonometric functions
 *---------------------------------------------------------------------------*/
// Inverse Sine
template<>
float       f_trans_api::arcsin(arg_type s) { return ::asinf(s); }
template<>
double      d_trans_api::arcsin(arg_type s) { return ::asin(s); }
template<>
long double ld_trans_api::arcsin(arg_type s) { return ::asinl(s); }

// Inverse Cosine
template<>
float       f_trans_api::arccos(arg_type s) { return ::acosf(s); }
template<>
double      d_trans_api::arccos(arg_type s) { return ::acos(s); }
template<>
long double ld_trans_api::arccos(arg_type s) { return ::acosl(s); }

// Inverse Tangent
template<>
float       f_trans_api::arctan(arg_type s) { return ::atanf(s); }
template<>
double      d_trans_api::arctan(arg_type s) { return ::atan(s); }
template<>
long double ld_trans_api::arctan(arg_type s) { return ::atanl(s); }

// Inverse Tangent (2 argument version)
template<>
float       f_trans_api::arctan(arg_type sin, arg_type cos) { return ::atan2f(sin, cos); }
template<>
double      d_trans_api::arctan(arg_type sin, arg_type cos) { return ::atan2(sin, cos); }
template<>
long double ld_trans_api::arctan(arg_type sin, arg_type cos) { return ::atan2l(sin, cos); }

// Inverse Cotangent
template<>
float       f_trans_api::arccot(arg_type s) { return ::atanf(1.0f / s); }
template<>
double      d_trans_api::arccot(arg_type s) { return ::atan(1.0 / s); }
template<>
long double ld_trans_api::arccot(arg_type s) { return ::atanl(1.0L / s); }

// Inverse Tangent (2 argument version)
template<>
float       f_trans_api::arccot(arg_type csc, arg_type sec) { return ::atan2f(csc, sec); }
template<>
double      d_trans_api::arccot(arg_type csc, arg_type sec) { return ::atan2(csc, sec); }
template<>
long double ld_trans_api::arccot(arg_type csc, arg_type sec) { return ::atan2l(csc, sec); }

// Inverse Secant
template<>
float       f_trans_api::arcsec(arg_type s) { return ::acosf(1.0f / s); }
template<>
double      d_trans_api::arcsec(arg_type s) { return ::acos(1.0 / s); }
template<>
long double ld_trans_api::arcsec(arg_type s) { return ::acosl(1.0L / s); }

// Inverse Cosecant
template<>
float       f_trans_api::arccsc(arg_type s) { return ::asinf(1.0f / s); }
template<>
double      d_trans_api::arccsc(arg_type s) { return ::asin(1.0 / s); }
template<>
long double ld_trans_api::arccsc(arg_type s) { return ::asinl(1.0L / s); }
