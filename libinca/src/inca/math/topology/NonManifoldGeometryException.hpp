/*
 * File: TopologyExceptions.hpp
 * 
 * Author: Ryan L. Saunders
 * 
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      A NonManifoldGeometryException is thrown when a non-manifold condition
 *      is detected (e.g., three faces sharing an edge) in a context where
 *      non-manifold geometry is not allowed.
 *
 *      A DegenerateGeometryException is thrown when a degenerate case is
 *      detected (e.g., a face with only two vertices/edges).
 */

#ifndef INCA_MATH_TOPOLOGY_EXCEPTIONS
#define INCA_MATH_TOPOLOGY_EXCEPTIONS

// Include superclass definition
#include <inca/util/StreamException.hpp>


// This is part of the Inca polygon modeling library
namespace inca {
    namespace math {
        // This is the superclass for all geometry-related exceptions
        class GeometryException : public StreamException {
        public:
            // Constructors
            explicit GeometryException() : StreamException() { }
            explicit GeometryException(const string &msg)
                : StreamException(msg) { }
        };

        // This is thrown when a non-manifold condition is detected
        class NonManifoldGeometryException : public GeometryException {
        public:
            // Constructors
            explicit NonManifoldGeometryException() : GeometryException() { }
            explicit NonManifoldGeometryException(const string &msg)
                : GeometryException(msg) { }
        };

        // This is thrown when a degenerate condition is detected
        class DegenerateGeometryException : public GeometryException {
        public:
            // Constructors
            explicit DegenerateGeometryException() : GeometryException() { }
            explicit DegenerateGeometryException(const string &msg)
                : GeometryException(msg) { }
        };
    };
};

#endif
