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

#ifndef INCA_IO_FILE_EXCEPTIONS
#define INCA_IO_FILE_EXCEPTIONS

// Include superclass definition
#include <inca/util/StreamException.hpp>


// This is part of the Inca I/O subsystem
namespace inca {
    namespace io {
        // Forward declarations
        class FileException;
        class InvalidReferenceException;
        class FileFormatException;
        class InvalidFileTypeException;
        class FileAccessException;
    };
};


class inca::io::FileException : public StreamException {
public:
    // Constructors
    explicit FileException(const string &file, const string &msg = "")
        : StreamException(msg), _filename(file) { }

    // Accessors
    const string & filename() const { return _filename; }

protected:
    string _filename;
};


class inca::io::InvalidReferenceException : public FileException {
public:
    // Constructors
    explicit InvalidReferenceException(const string &file,
                                       const string &ID,
                                       const string &msg = "")
                        : FileException(file, msg), _id(ID) {
        _reference = filename() + '#' + id();
//        *this << reference << ": " << message;
    }

    // Accessors
    const string & reference() const { return _reference; }
    const string & id() const { return _id; }

protected:
    string _id, _reference;
};


class inca::io::FileFormatException : public FileException {
public:
    // Constructors
    explicit FileFormatException(const string &file, int lNo = 1, int cNo = 1,
                                 const string &msg = "")
        : FileException(file, msg), _line(lNo), _column(cNo) { }

    // Accessors
    int line()   const { return _line; }
    int column() const { return _column; }
    void setLine(int ln)   { _line = ln; }
    void setColumn(int cl) { _column = cl; }

protected:
    int _line, _column;
};


class inca::io::InvalidFileTypeException : public FileException {
public:
    // Constructors
    explicit InvalidFileTypeException(const string &file,
                                      const string &msg = "")
        : FileException(file, msg) { }
};


class inca::io::FileAccessException : public FileException {
public:
    // Constructors
    explicit FileAccessException(const string &file, const string &msg = "")
        : FileException(file, msg) { }
};

#endif
