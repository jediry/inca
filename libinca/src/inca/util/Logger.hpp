/*
 * File: Logger.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Logger class contains functions to output logging messages of
 *      various degrees of seriousness. Which types of messages are filtered
 *      and which are printed to the console is determined at compile time,
 *      through the definition of the macros:
 *          LOGGING_TRACE   allow function call trace
 *          LOGGING_DEBUG   allow debugging messages
 *          LOGGING_INFO    allow status information
 *          LOGGING_WARNING allow warning messages
 *          LOGGING_ERROR   allow error messages
 *          LOGGING_FATAL   allow fatal error messages
 *
 *      Additionally, this file defines a number of strings that may be used
 *      when constructing the logging message, allowing messages to be
 *      symbolically tagged according to the type of event.
 */

#ifndef INCA_UTIL_LOGGER
#define INCA_UTIL_LOGGER

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca utilities collection
namespace inca {
    // Forward declarations
    class Logger;


    // The globally-accessible Logger instance
    extern Logger logger;


    // Logging level definitions
    enum LoggerLevel {
        None,       // Nothing is printed, ever
        Fatal,      // Only really, really bad things get through
        Error,      // When things go wrong, you'll hear about it
        Warning,    // If things MIGHT go wrong you'll...
        Info,       // Even when things go right, we'll tell you
        Debug,      // When you REALLY want to know...
        Trace,      // Function calls, exceptions, the works.
    };

    // These allow us to symbolically represent types of action
    #define CREATION        " + "
    #define DELETION        " - "
    #define SERIALIZATION   "<< "
    #define DESERIALIZATION ">> "

    // How much we indent
    #define INDENT          "  "
};


class inca::Logger : public ostringstream {
public:
    // Compile-time logging level
    #ifdef LOGGER_LEVEL
    static const LoggerLevel loggerLevel = LOGGER_LEVEL;
    #else
    static const LoggerLevel loggerLevel = Debug;
    #endif

    // Constructors
    explicit Logger() : target(cout), indentLevel(0) { }
    Logger(const Logger &l) : target(l.target), indentLevel(l.indentLevel) { }
    explicit Logger(ostream &o) : target(o), indentLevel(0) { }

    // Functions for indenting nicely
    void increaseIndentLevel() { indentLevel++; }
    void decreaseIndentLevel() { indentLevel--; }
    int getIndentLevel() { return indentLevel; }
    void setIndentLevel(int level) { indentLevel = level; }

    // Function call trace functions
    void traceCall(const string &function) {
        if (loggerLevel >= Trace)
            target << "CALL   " << function << endl;
    }

    void traceReturn(const string &function) {
        if (loggerLevel >= Trace)
            target << "RETURN " << function << endl;
    }
    
    void traceThrow(const string &function) {
        if (loggerLevel >= Trace)
            target << "THROW  " << function << endl;
    }

    // Logging functions -- these dump the buffered input to the output target
    void debug() {
        if (loggerLevel >= Debug) {
            for (int i = 0; i < indentLevel; i++)   target << INDENT;
            target << str() << endl;
        }
        str("");     // Clear the buffer
    }

    void info() {
        if (loggerLevel >= Info) {
            for (int i = 0; i < indentLevel; i++)   target << INDENT;
            target << str() << endl;
        }
        str("");     // Clear the buffer
    }

    void warning() {
        if (loggerLevel >= Warning) {
            for (int i = 0; i < indentLevel; i++)   target << INDENT;
            target << "Warning: " << str() << endl;
        }
        str("");     // Clear the buffer
    }

    void error() {
        if (loggerLevel >= Error) {
            for (int i = 0; i < indentLevel; i++)   target << INDENT;
            target << "ERROR: " << str() << endl;
        }
        str("");     // Clear the buffer
    }

    void fatal() {
        if (loggerLevel >= Fatal) {
            for (int i = 0; i < indentLevel; i++)   target << INDENT;
            target << "FATAL: " << str() << endl;
        }
        str("");     // Clear the buffer
    }

protected:
    ostream & target;
    int indentLevel;
};

#endif
