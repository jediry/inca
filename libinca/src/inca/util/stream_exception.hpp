/*
 * File: stream_exception.hpp
 * 
 * Author: Ryan L. Saunders
 * 
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The stream_exception class is an exception object that also implements
 *      ostream, allowing the error message to be either initialized
 *      at construction time, or built up incrementally using the standard
 *      IOstream << operator.
 */

#ifndef INCA_STREAM_EXCEPTION
#define INCA_STREAM_EXCEPTION

// Import system configuration
#include <inca/inca-common.h>

// Import stringstream types
#include <sstream>

// This is the root Inca exception class
namespace inca {
    class stream_exception {
    public:
        // Constructors
        stream_exception() { }
        stream_exception(const stream_exception &e) : ss(e.message()) { }
        stream_exception(const string &msg) : ss(msg) { }

        // Accessors
        string message() const { return ss.str(); }
        std::ostream & os() { return ss; }

    protected:
        std::ostringstream ss;
    };

    // ostream writer for this exception type
    inline std::ostream & operator<<(std::ostream &o,
                                     const stream_exception &e) {
        return o << e.message();
    }
};

#endif
