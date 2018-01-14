/*
 * File: TopologyExceptions.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      A NonManifoldTopologyException is thrown when a non-manifold condition
 *      is detected (e.g., three faces sharing an edge) in a context where
 *      non-manifold geometry is not allowed.
 *
 *      A DegenerateTopologyException is thrown when a degenerate case is
 *      detected (e.g., a face with only two vertices/edges).
 */

#pragma once
#ifndef INCA_MATH_TOPOLOGY_EXCEPTIONS
#define INCA_MATH_TOPOLOGY_EXCEPTIONS

// Include superclass definition
#include <inca/util/StreamException.hpp>


// This is part of the Inca polygon modeling library
namespace inca {
    namespace math {
        // This is the superclass for all topology-related exceptions
        class TopologyException : public StreamException {
        public:
            // Constructors
            explicit TopologyException() : StreamException() { }
            explicit TopologyException(const std::string &msg)
                : StreamException(msg) { }
        };

        // This is thrown when an inconsistent topological operation is
        // requested (e.g., traversal of two unconnnected entities)
        class InvalidTopologyException : public TopologyException {
        public:
            // Constructors
            explicit InvalidTopologyException() : TopologyException() { }
            explicit InvalidTopologyException(const std::string &msg)
                : TopologyException(msg) { }
        };

        // This is thrown when a non-manifold condition is detected
        class NonManifoldTopologyException : public TopologyException {
        public:
            // Constructors
            explicit NonManifoldTopologyException() : TopologyException() { }
            explicit NonManifoldTopologyException(const std::string &msg)
                : TopologyException(msg) { }
        };
    };
};

#endif
