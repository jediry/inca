/*
 * File: StreamException.hpp
 * 
 * Author: Ryan L. Saunders
 * 
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The stream_exception class is an exception object that implements the
 *      ostream output operator <<, allowing the error message either to be
 *      initialized at construction time, or to be built up incrementally
 *      using the standard IOstream << operator.
 */

#ifndef INCA_STREAM_EXCEPTION
#define INCA_STREAM_EXCEPTION

// Import system configuration
#include <inca/inca-common.h>

// Import exception superclass and related types
#include <exception>
#include <sstream>

// This is the root Inca exception class
namespace inca {
    class StreamException : public std::exception {
    public:
        // Constructors
        StreamException() { }
        StreamException(const StreamException &e) : ss(e.message()) { }
        StreamException(const std::string &msg) : ss(msg) { }

        // Accessors
        string message() const { return ss.str(); }
        const char * what() const { return message().c_str(); }

        // << operator implementations
        template <typename T>
        StreamException & operator<<(const T & t) {
            ss << t;
            return *this;
        }

    protected:
        std::ostringstream ss;
    };

    // ostream writer for this exception type
    inline std::ostream & operator<<(std::ostream &os,
                                     const StreamException &e) {
        return os << e.message();
    }
};

#endif