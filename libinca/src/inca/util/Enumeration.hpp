 /** -*- C++ -*-
 *
 * \file Enumeration.hpp
 *
 * \author Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements a more robust and functional equivalent to the
 *      standard C 'enum'. In addition to providing the normal set of symbolic,
 *      integral constants, this mechanism also provides ways to retrieve the
 *      minimum and maximum values, to write the symbolic names to a
 *      std::ostream using the normal '<<' operator, and to perform integer
 *      arithmetic on the enumeration type without the need to explicitly
 *      cast back and forth to ints. This allows the user to write normal,
 *      C-style for looks using enumerated types:
 *
 *          for (MyEnum e = MyEnum::minimum(); e &lt; MyEnum::maximum(); ++e) ...
 *
 *      The Enumeration type also does validity checking during every mutating
 *      operation and throws an IllegalEnumerantException if the operation would
 *      result in an invalid value (this check is governed by the value of
 *      INCA_DO_BOUNDS_CHECKS -- see inca-common.h).
 *
 *      Note that this validity checking would make the above 'for' loop
 *      impossible, since incrementing past the maximum value (which would
 *      normally just terminate the loop) raises an exception. To compensate for
 *      this, extra 'legal' values called <code>&lt;enum-name&gt;_Overflow</code>
 *      and &lt;enum-name&gt;_Underflow are silently created with values one
 *      larger and one smaller, than the maximum and minimum, respectively.
 *
 * Usage:
 *      The easiest way to create an Enumeration is to use the preprocessor macros
 *      INCA_ENUM or INCA_ENUMV. Both macros expect two arguments: the first is
 *      the name for the enumerated type and the second is a "list" as defined
 *      by the Boost preprocessor library, which has the form:
 *          ( e1, ( e2, ( e3, ( e4, BOOST_PP_NIL ) ) ) )
 *
 *      The INCA_ENUM macro expects each element in the list to be the name of an
 *      element of the enumeration. E.g.:
 *          INCA_ENUM( AnEnum,
 *                   ( Value1,
 *                   ( Value2,
 *                   ( Value3, ... ) ) ) );
 *
 *      The INCA_ENUMV macro also allows the integer values for each item to be
 *      specified. Each element in the list is a "tuple", of the form
 *      ( NAME, VALUE ). E.g.:
 *          INCA_ENUMV( AnotherEnum,
 *                    ( (Value1, 1),
 *                    ( (Value2, 8),
 *                    ( (Value3, 37), ... ) ) ) );
 *
 *      As a convenience, if the macro INCA_DROP_MACRO_PREFIX is non-zero,
 *      then these macros will also be available under the shortened forms
 *      ENUM and ENUMV (the longer forms are still allowed).
 *
 * FIXME: Grrr...the operators don't work when the ENUM is declared inside a class,
 *        due to the different rules for operators. Is there a way around this?
 */

#ifndef INCA_ENUMERATION
#define INCA_ENUMERATION

// Import C++ standard library exception base class
#include <exception>

// Import IOStream declarations
#include <iostream>

// Import Boost preprocessor metaprogramming macros
#include <boost/preprocessor/list/fold_left.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/size.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/selection/min.hpp>
#include <boost/preprocessor/selection/max.hpp>

// Import Boost template SFINAE enable mechanism
#include <boost/utility/enable_if.hpp>


// This is part of the Inca utilities collection
namespace inca {
    // Forward declarations
    struct IllegalEnumerantException;
    template <class Derived, int min, int max, int sz, int deflt> class Enumeration;
}


// The exception class that is thrown when an
struct inca::IllegalEnumerantException : public std::exception {
    const char * what() const throw() {
        return "Illegal index for enumeration";
    }
};


// The Enumeration class, which implements most of the usual integer
// arithmetic operations
template <class Derived, int min, int max, int sz = (max - min + 1), int deflt = min>
class inca::Enumeration {
/*---------------------------------------------------------------------------*
 | Access to the derived class
 *---------------------------------------------------------------------------*/
private:
    Derived & derived() {
        return static_cast<Derived &>(*this);
    }
    const Derived & derived() const {
        return static_cast<const Derived &>(*this);
    }


/*---------------------------------------------------------------------------*
 | Static properties of the enumeration (min, max, default, etc.)
 *---------------------------------------------------------------------------*/
public:
    // This is just here to simplify the function templates later
    static const bool inca_enumeration_subclass = true;

    // How many (normal) values does this enumeration have?
    static const int count = sz;

    // Count/min/max/default values
    static int     size()    { return sz; }
    static Derived minimum() { return Derived(min); }
    static Derived maximum() { return Derived(max); }
    static Derived defaultValue() { return Derived(deflt); }


/*---------------------------------------------------------------------------*
 | Validity checks (may be overridden by subclass)
 *---------------------------------------------------------------------------*/
public:
    // Validity check of a value (override this value in the subclass if you
    // want to change the validity checking behavior for some reason). Right
    // now, this just checks to see if the values is at most one less than\
    // the minimum or one greater than the maximum (see the file comments for
    // why we allow the extra on each side).
    bool isValid(int index) {
        return index >= (min - 1) && index <= (max + 1);
    }

    // Throw an IllegalEnumerantException if this value is invalid. This
    // function does nothing (zip. zero. zilch.) if INCA_DO_BOUNDS_CHECKS
    // is zero or undefined.
    void validate(int index) throw(IllegalEnumerantException) {
        #if INCA_DO_BOUNDS_CHECKS
            if (! derived().isValid(index))
                throw IllegalEnumerantException();
        #endif
    }


/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Default constructor
    Enumeration() : index(defaultValue()) { }

    // Integer constructor (allows implicit conversion)
    Enumeration(int idx) throw(IllegalEnumerantException) {
        validate(idx);                  // Throw if invalid
        index = idx;                    // Otherwise, assign it
    }


/*---------------------------------------------------------------------------*
 | Operators
 *---------------------------------------------------------------------------*/
public:
    // Increment/decrement operators
    Derived & operator++() throw(IllegalEnumerantException) {
        int newValue = index + 1;
        validate(newValue);
        index = newValue;
        return derived();
    }
    Derived & operator--() throw(IllegalEnumerantException) {
        int newValue = index - 1;
        validate(newValue);
        index = newValue;
        return derived();
    }
    Derived operator++(int) throw(IllegalEnumerantException) {
        Derived temp = derived();
        ++(*this);
        return temp;
    }
    Derived operator--(int) throw(IllegalEnumerantException) {
        Derived temp = derived();
        --(*this);
        return temp;
    }

    // Cast-to-integer
    operator int() const { return index; }

protected:
    int index;
};


/*---------------------------------------------------------------------------*
 | Arithmetic and relational operators
 *---------------------------------------------------------------------------*/
namespace inca {
#define ENUM_OP(RTYPE)                                                      \
    template <typename E>                                                   \
    typename boost::enable_if_c< E::inca_enumeration_subclass, RTYPE>::type operator

#define ENUM_REL_OP(OP)                                                     \
    ENUM_OP(bool) OP (E e, int i) {                                         \
        return int(e) OP i;                                                 \
    }                                                                       \
    ENUM_OP(bool) OP (int i, E e) {                                         \
        return i OP int(e);                                                 \
    }                                                                       \
    ENUM_OP(bool) OP (E e0, E e1) {                                         \
        return int(e0) OP int(e1);                                          \
    }

#define ENUM_ARITH_OP(OP)                                                   \
    ENUM_OP(E) OP (E e, int i) throw(IllegalEnumerantException) {           \
        return E(int(e) OP i);                                              \
    }                                                                       \
    ENUM_OP(E) OP (int i, E e) throw(IllegalEnumerantException) {           \
        return E(i OP int(e));                                              \
    }                                                                       \
    ENUM_OP(E) OP (E e0, E e1) throw(IllegalEnumerantException) {           \
        return E(int(e0) OP int(e1));                                       \
    }                                                                       \
    ENUM_OP(E &) OP ## = (E e, int i) throw(IllegalEnumerantException) {    \
        return (e = E(int(e), i));                                          \
    }                                                                       \
    ENUM_OP(E &) OP ## = (E e0, E e1) throw(IllegalEnumerantException) {    \
        return (e0 = E(int(e0), int(e1)));                                  \
    }

    // Relational operators
    ENUM_REL_OP(==)
    ENUM_REL_OP(!=)
    ENUM_REL_OP(<=)
    ENUM_REL_OP(>=)
    ENUM_REL_OP(<)
    ENUM_REL_OP(>)

    // Arithmetic operations (and computed-assignment counterparts)
    ENUM_ARITH_OP(+)
    ENUM_ARITH_OP(-)
    ENUM_ARITH_OP(*)
    ENUM_ARITH_OP(/)
    ENUM_ARITH_OP(%)

    // Bitwise operations (and computed-assignment counterparts)
    ENUM_ARITH_OP(&)
    ENUM_ARITH_OP(|)
    ENUM_ARITH_OP(^)
    ENUM_ARITH_OP(<<)
    ENUM_ARITH_OP(>>)
    ENUM_OP(E) ~ (E e) throw(IllegalEnumerantException) {
        return E(~ int(e));
    }

#undef ENUM_OP
#undef ENUM_REL_OP
#undef ENUM_ARITH_OP
}


// These macros create an Enumeration, complete with IOStream <</>> operators.
// The first argument is the name of the enumeration type, while the second is
// a list of enumerant names.
#define INCA_ENUM(NAME, VALUES)                                             \
    /* An anonymous enum associated with our Enumeration subclass */        \
    enum {                                                                  \
        BOOST_PP_LIST_FOR_EACH(INCA_ENUM_VALUE, /* */, VALUES)              \
    };                                                                      \
    static const int NAME ## _Underflow = -1;                               \
    static const int NAME ## _Overflow  = BOOST_PP_LIST_SIZE(VALUES);       \
                                                                            \
    /* The actual Enumeration subclass that bears the NAME */               \
    class NAME : public ::inca::Enumeration<NAME, 0,                        \
                                            BOOST_PP_LIST_SIZE(VALUES) - 1, \
                                            BOOST_PP_LIST_SIZE(VALUES)> {   \
    public:                                                                 \
        /* Default constructor */                                           \
        NAME() { }                                                          \
                                                                            \
        /* Initialization from integer constructor */                       \
        NAME(int idx) :                                                     \
            ::inca::Enumeration<NAME, 0,                                    \
                                BOOST_PP_LIST_SIZE(VALUES) - 1,             \
                                BOOST_PP_LIST_SIZE(VALUES)>(idx) { }        \
    };                                                                      \
                                                                            \
    /* IOStream writer operator. Prints name of enumerant */                \
    inline std::ostream & operator<<(std::ostream & os, NAME e) {           \
        switch (e) {                                                        \
            BOOST_PP_LIST_FOR_EACH(INCA_ENUM_OSTREAM_CASE, /* */, VALUES)   \
            case NAME ## _Underflow: return os << #NAME "(underflow)";      \
            case NAME ## _Overflow:  return os << #NAME "(overflow)";       \
            default:                 return os << #NAME "(" << int(e) << ')';\
        }                                                                   \
    }


// This macro is identical to the previous except that the VALUES list is
// instead a list of ( NAME, VALUE ) tuples.
#define INCA_ENUMV(NAME, VALUES)                                            \
    /* An anonymous enum associated with our Enumeration subclass */        \
    enum {                                                                  \
        BOOST_PP_LIST_FOR_EACH(INCA_ENUMV_VALUE, /* */, VALUES)             \
    };                                                                      \
    static const int NAME ## _Underflow =                                   \
        BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MIN, 50, VALUES) - 1;       \
    static const int NAME ## _Overflow  =                                   \
        BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MAX, 0, VALUES) + 1;        \
                                                                            \
    /* The actual Enumeration subclass that bears the NAME */               \
    class NAME : public ::inca::Enumeration<NAME,                           \
            BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MIN, 50, VALUES),       \
            BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MAX, 0, VALUES),        \
            BOOST_PP_LIST_SIZE(VALUES)> {                                   \
    public:                                                                 \
        /* Default constructor */                                           \
        NAME() { }                                                          \
                                                                            \
        /* Initialization from integer constructor */                       \
        NAME(int idx) : ::inca::Enumeration<NAME,                           \
            BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MIN, 50, VALUES),       \
            BOOST_PP_LIST_FOLD_LEFT(INCA_ENUMV_LIST_MAX, 0, VALUES),        \
            BOOST_PP_LIST_SIZE(VALUES)>(idx){ }                             \
    };                                                                      \
                                                                            \
    /* IOStream writer operator. Prints name of enumerant */                \
    inline std::ostream & operator<<(std::ostream & os, NAME e) {           \
        switch (e) {                                                        \
            BOOST_PP_LIST_FOR_EACH(INCA_ENUMV_OSTREAM_CASE, /* */, VALUES)  \
            case NAME ## _Underflow: return os << #NAME "(underflow)";      \
            case NAME ## _Overflow:  return os << #NAME "(overflow)";       \
            default:                 return os << #NAME "(" << int(e) << ')';\
        }                                                                   \
    }


// These macros write out "NAME," as part of the anonymous enum
#define INCA_ENUM_VALUE(R, JUNK, ELEM)  ELEM,
#define INCA_ENUMV_VALUE(R, JUNK, ELEM)                                     \
    BOOST_PP_TUPLE_ELEM(2, 0, ELEM) = BOOST_PP_TUPLE_ELEM(2, 1, ELEM),

// These macros write out "case NAME: return os << #NAME;" as part of the
// switch statement inside the '<<' operator implementation
#define INCA_ENUM_OSTREAM_CASE(R, JUNK, ELEM)                               \
    case ELEM:  return os << BOOST_PP_STRINGIZE(ELEM);

#define INCA_ENUMV_OSTREAM_CASE(R, JUNK, ELEM)                              \
    case BOOST_PP_TUPLE_ELEM(2, 0, ELEM):                                   \
        return os << BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, ELEM));

// These macros find the smallest/largest values of the list of enumerants
#define INCA_ENUMV_LIST_MIN(d, current, x)                                   \
    BOOST_PP_MIN(current, BOOST_PP_TUPLE_ELEM(2, 1, x))
#define INCA_ENUMV_LIST_MAX(d, current, x)                                   \
    BOOST_PP_MAX(current, BOOST_PP_TUPLE_ELEM(2, 1, x))


// Finally, if we've been instructed to drop the INCA_ prefix on macros, do so
#if INCA_DROP_MACRO_PREFIX
    #define ENUM(NAME, VALUES)  INCA_ENUM(NAME, VALUES)
    #define ENUMV(NAME, VALUES) INCA_ENUMV(NAME, VALUES)
#endif

#endif

