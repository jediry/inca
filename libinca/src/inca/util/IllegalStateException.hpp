/** -*- C++ -*-
 *
 * \file    IllegalStateException.hpp
 * 
 * \author  Ryan L. Saunders
 * 
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The IllegalStateException class is thrown whenever an object or function
 *      reaches (or would reach) a state that is inconsistent, or otherwise not
 *      permitted. For example, this could be thrown if a request is made to
 *      destroy a window that does not exist.
 */

#ifndef INCA_ILLEGAL_STATE_EXCEPTION
#define INCA_ILLEGAL_STATE_EXCEPTION

// Import superclass definition
#include "StreamException.hpp"


// This is part of the Inca utilities collection
namespace inca {
    // This is thrown when an unsupported operation is implemented
    class IllegalStateException : public StreamException {
    public:
        // Constructors
        explicit IllegalStateException(const std::string & msg = "")
            : StreamException(msg) { }
    };
}

#endif
