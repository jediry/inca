/*
 * File: UnsupportedOperationException.hpp
 * 
 * Author: Ryan L. Saunders
 * 
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The UnsupportedOperationException class is thrown whenever an
 *      operation is requested that is not supported. This serves two primary
 *      purposes:
 *          1) it is thrown when a no-op (virtual) function in a base
 *             is not overridden in a subclass. In this case, it represents
 *             an optional function not implemented by the subclass.
 *          2) it is thrown when a function has not yet been implemented. In
 *             this case, it represents an implementation deficiency.
 */

#ifndef INCA_UNSUPPORTED_OPERATION_EXCEPTION
#define INCA_UNSUPPORTED_OPERATION_EXCEPTION

// Import superclass definition
#include "IncaException.hpp"


// This is part of the Inca utilities collection
namespace inca {
    // This is thrown when an unsupported operation is implemented
    class UnsupportedOperationException : public IncaException {
    public:
        // Constructors
        UnsupportedOperationException() : IncaException() { }
        UnsupportedOperationException(const UnsupportedOperationException &e)
            : IncaException(e) { }
        UnsupportedOperationException(const string &msg)
            : IncaException(msg) { }
    };
}

#endif
