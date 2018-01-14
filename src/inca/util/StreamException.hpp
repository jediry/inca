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

#pragma once
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
        explicit StreamException() { }
        StreamException(const StreamException & e) : _ss(e.message()) { }
        explicit StreamException(const std::string & msg) : _ss(msg) { }

        // Destructor
        virtual ~StreamException() throw() { }

        // Accessors
        virtual std::string message() const { return _ss.str(); }
        const char * what() const throw() { return message().c_str(); }

        // << operator implementations
        template <typename T>
        StreamException & operator<<(const T & t) {
            _ss << t;
            return *this;
        }

    protected:
        std::ostringstream _ss;
    };

    // ostream writer for this exception type
    inline std::ostream & operator<<(std::ostream & os,
                                     const StreamException & e) {
        return os << e.message();
    }
};

#endif
