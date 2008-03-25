/*
 * File: FileExceptions.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The FileException class is the base class for all file-related
 *      exceptions thrown by the Inca I/O subsystem.
 *
 *      The InvalidReferenceException XXX
 *
 *      The FileFormatException class indicates that there was an error
 *      in processing a file (such as a scene file), due to a syntactic or
 *      semantic error.
 *
 *      The InvalidFileTypeException class indicates that a file of some
 *      type was supplied in a context where another type was expected. This
 *      could be caused, for example, by trying to open an image file as a
 *      3D scene file.
 *
 *      The FileAccessException class indicates that there was an error
 *      in accessing a file. This might be caused by the OS's file permissions,
 *      or because the file does not exist or is locked by another program.
 */

#pragma once
#ifndef INCA_IO_FILE_EXCEPTIONS
#define INCA_IO_FILE_EXCEPTIONS

// Include superclass definition
#include <inca/util/StreamException.hpp>


// This is part of the Inca I/O subsystem
namespace inca {
    namespace io {
        // Forward declarations
        class FileException;
        class InvalidFileTypeException;
        class FileAccessException;
        class FileFormatException;
        class InvalidReferenceException;
    };
};


class inca::io::FileException : public StreamException {
public:
    // Constructors
    explicit FileException(const std::string & file,
                           const std::string & msg = "")
        : StreamException(msg), _filename(file) { }

    // Destructor
    ~FileException() throw() { }

    // Accessors
    const std::string & filename() const { return _filename; }

protected:
    std::string _filename;
};


class inca::io::InvalidFileTypeException : public FileException {
public:
    // Constructors
    explicit InvalidFileTypeException(const std::string & file,
                                      const std::string & msg = "")
        : FileException(file, msg) { }

    // Destructor
    ~InvalidFileTypeException() throw() { }
};


class inca::io::FileAccessException : public FileException {
public:
    // Constructors
    explicit FileAccessException(const std::string & file, 
                                 const std::string & msg = "")
        : FileException(file, msg) { }

    // Destructor
    ~FileAccessException() throw() { }
};


class inca::io::FileFormatException : public FileException {
public:
    // Sentinel value indicating that a line/column number is unknown
    static const int UNKNOWN = -1;

    // Constructors
    explicit FileFormatException(const std::string & file,
                                 const std::string & msg = std::string())
        : FileException(file, msg), _line(UNKNOWN), _column(UNKNOWN) { }
    explicit FileFormatException(const std::string & file,
                                 int lNo, int cNo = UNKNOWN,
                                 const std::string & msg = std::string())
        : FileException(file, msg), _line(lNo), _column(cNo) { }

    // Destructor
    ~FileFormatException() throw() { }

    // Accessors
    int line()   const { return _line; }
    int column() const { return _column; }
    void setLine(int ln)   { _line = ln; }
    void setColumn(int cl) { _column = cl; }
    
    // Line/column string
    std::string locationString() const {
        std::ostringstream ss;
        if (line() == UNKNOWN && column() == UNKNOWN) {
            ss << "location unknown";
        } else {
            if (line() != UNKNOWN)
                ss << "line " << line();
            if (line() != UNKNOWN && column() != UNKNOWN)
                ss << ", ";
            if (column() != UNKNOWN)
                ss << "column " << column();
        }
        return ss.str();
    }
    
    // Overriden message function, appending the location string
    std::string message() const {
        return std::string("(") + locationString() + "): " + _ss.str();
    }

protected:
    int _line, _column;
};


class inca::io::InvalidReferenceException : public FileFormatException {
public:
    // Constructors
    explicit InvalidReferenceException(const std::string & file,
                                       const std::string & ID,
                                       const std::string & msg = std::string())
                        : FileFormatException(file, msg), _id(ID) {
        _reference = filename() + '#' + id();
//        *this << reference << ": " << message;
    }

    // Destructor
    ~InvalidReferenceException() throw() { }

    // Accessors
    const std::string & reference() const { return _reference; }
    const std::string & id() const { return _id; }

protected:
    std::string _id, _reference;
};

#endif
