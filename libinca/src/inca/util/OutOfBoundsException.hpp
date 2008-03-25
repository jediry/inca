/* -*- C++ -*-
 *
 * File: OutOfBoundsException.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file defines exception classes and macros for supporting runtime
 *      integer bounds and range checks.
 *
 *      Run-time range checking is governed by the value of
 *      INCA_DO_BOUNDS_CHECKS macro, which may be:
 *          0           no range checks will be performed
 *          1           range checks will be performed
 *          otherwise   range checks will be performed only if DEBUG is defined
 *
 *      There are several types of checks.
 *
 *      The first check is basically an argument sanity check, to verify that
 *      a collection of indices/sizes has at least the same number of elements
 *      as the dimensionality of the container. If this check fails an
 *      IncorrectDimensionalityException is thrown.
 *
 *      The second check is to maintain the constraint that reshaping a
 *      MultiArray should not change the total number of elements. If this is
 *      attempted, an IllegalSizeException is thrown.
 *
 *      The third check depends on whether the list being checked is a list
 *      of sizes or a list of indices. For sizes, this check simply ensures
 *      that each size is non-negative (negative sizes are meaningless, but
 *      zero sizes are allowed). For indices, this check ensures that each
 *      index falls between the base and extent (inclusive) for that dimension.
 *      If this check fails, an IllegalSizeException or an
 *      IndexOutOfBoundsException (respectively) is thrown.
 *
 * XXX This documentation was cut/paste from MultiArray. Probably needs (lotsa) revising.
 *     Also, some of these macros are specific to MultiArray. Blegh.
 *
 *     Raster does bounds checking, regardless of the value of this.
 */

#pragma once
#ifndef INCA_UTIL_OUT_OF_BOUNDS_EXCEPTION
#define INCA_UTIL_OUT_OF_BOUNDS_EXCEPTION

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca utilities collection
namespace inca {
    // Exception classes
    class OutOfBoundsException;
};


// If we are range-checking, define these macros to throw exceptions
#if INCA_DO_BOUNDS_CHECKS
    // This low-level macro is called by the others to actually throw
    // the exception.
    #define INCA_BOUNDS_CHECK(MIN, MAX, ACT, DIM, STREAM_EXPR) {            \
        if ((ACT) < (MIN) || (ACT) > (MAX)) {                               \
            OutOfBoundsException e(MIN, MAX, ACT, DIM);                     \
            e << STREAM_EXPR;                                               \
            throw e;                                                        \
        }                                                                   \
    }

    // Ensure each index is within the range [base, extent]
    #define INCA_BOUNDS_CHECK_MULTIDIM(BOUNDS, DIM, LIST_TYPE, LIST_NAME) { \
        INCA_BOUNDS_CHECK(DIM, DIM, LIST_NAME.size(), -1,                   \
            "Multi-dim indices have incorrect dimensionality "              \
            << LIST_NAME.size())                                            \
        typename LIST_TYPE::const_iterator it = LIST_NAME.begin();          \
        for (IndexType d = 0; d < DIM; ++d) {                               \
            INCA_BOUNDS_CHECK(BOUNDS.base(d), BOUNDS.extent(d),             \
                IndexType(*it), d,                                          \
                "Multi-dim index " << IndexType(*it) << " along "           \
                    << "dimension " << d << " is not within ["              \
                    << BOUNDS.base(d) << ", " << BOUNDS.extent(d)           \
                    << ']')                                                 \
            ++it;                                                           \
        }                                                                   \
    }

    // Ensure the linear index is within the range [0, SIZE - 1]
    #define INCA_BOUNDS_CHECK_LINEAR(COLLECTION, IDX) {                     \
        INCA_BOUNDS_CHECK(0, (COLLECTION.size() - 1), (IDX), -1,            \
            "Linear index " << (IDX) << " is not within the bounds [0, "    \
            << (COLLECTION.size() - 1) << ']')                              \
    }


// Otherwise, define them as no-ops
#else
    #define INCA_BOUNDS_CHECK(MIN, MAX, ACT, DIM, STREAM_EXPR)
    #define INCA_BOUNDS_CHECK_MULTIDIM(BOUNDS, DIM, LIST_TYPE, LIST_NAME)
    #define INCA_BOUNDS_CHECK_LINEAR(COLLECTION, IDX)
#endif


// Import stream-enabled exception superclass
#include "StreamException.hpp"


// Exception classes
class inca::OutOfBoundsException : public inca::StreamException {
public:
    // Constructor taking the valid range and the OOB value
    OutOfBoundsException(IndexType min, IndexType max, IndexType act,
                         const std::string & msg = std::string())
        : StreamException(msg),
          _minimum(min), _maximum(max), _actual(act) { }

    // Constructor additionally taking the dimension along which the OOB occurred
    OutOfBoundsException(IndexType min, IndexType max, IndexType act, IndexType dim,
                         const std::string & msg = std::string())
        : StreamException(msg),
          _minimum(min), _maximum(max), _actual(act), _dimension(dim) { }

    // Accessors
    IndexType minimum()   const { return _minimum; }
    IndexType maximum()   const { return _maximum; }
    IndexType actual()    const { return _actual; }
    IndexType dimension() const { return _dimension; }

protected:
    IndexType _minimum, _maximum, _actual, _dimension;
};

#endif
