/*
 * File: IncaException.hpp
 * 
 * Author: Ryan L. Saunders
 * 
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The IncaException class is the base class for all exceptions thrown
 *      by Inca code. It can be initialized with an error message at
 *      construction time, or the message can be built incrementally,
 *      using the ostream << operators.
 */

#ifndef INCA_EXCEPTION
#define INCA_EXCEPTION

// Import system configuration
#include <inca/inca-common.h>

// This is the root Inca exception class
namespace inca {
    class IncaException : public ostringstream {
    public:
        // Constructors
        IncaException() { }
        IncaException(const IncaException &e) : ostringstream(e.str()) { }
        IncaException(const string &msg) : ostringstream(msg) { }

        // Accessors
        const string getMessage() const { return str(); }
        operator string() const { return str(); }
    };

    inline ostream & operator<<(ostream &o, const IncaException &e) {
        return o << string(e);
    }
};

#endif
