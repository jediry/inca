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

#ifndef INCA_FILE_EXCEPTIONS
#define INCA_FILE_EXCEPTIONS

// Include superclass definition
#include <util/IncaException.hpp>


// This is part of the Inca I/O subsystem
namespace Inca {
    namespace IO {
        // Forward declarations
        class FileException;
        class InvalidReferenceException;
        class FileFormatException;
        class InvalidFileTypeException;
        class FileAccessException;
    };
};


class Inca::IO::FileException : public IncaException {
public:
    // Constructors
    FileException(const FileException &e)
        : IncaException(e), filename(e.filename) { }
    FileException(const string &file)
            : IncaException(), filename(file) { }
    FileException(const string &msg, const string &file)
            : IncaException(msg), filename(file) { }

    // Accessors
    const string & getFilename() const { return filename; }

protected:
    string filename;
};


class Inca::IO::InvalidReferenceException : public FileException {
public:
    // Constructors
    InvalidReferenceException(const InvalidReferenceException &e)
        : FileException(e), id(e.id), reference(e.reference) { }
    InvalidReferenceException(const string &msg,
                              const string &file = "",
                              const string &ID = "")
                        : FileException(msg, file), id(ID) {
        reference = filename + '#' + id;
//        *this << reference << ": " << message;
    }

    // Accessors
    const string & getReference() const { return reference; }
    const string & getID() const { return id; }

protected:
    string id, reference;
};


class Inca::IO::FileFormatException : public FileException {
public:
    // Constructors
    FileFormatException(const FileFormatException &e)
        : FileException(e), line(e.line) { }
    FileFormatException(const string &file, int lineNo = 0)
        : FileException(file), line(lineNo) { }
    FileFormatException(const string &msg, const string &file, int lineNo)
        : FileException(msg, file), line(lineNo) { }

    // Accessors
    int getLineNumber() { return line; }

protected:
    int line;
};


class Inca::IO::InvalidFileTypeException : public FileException {
public:
    // Constructors
    InvalidFileTypeException(const InvalidFileTypeException &e)
        : FileException(e) { }
    InvalidFileTypeException(const string &file)
        : FileException(file) { }
    InvalidFileTypeException(const string &msg, const string &file)
        : FileException(msg, file) { }
};


class Inca::IO::FileAccessException : public FileException {
public:
    // Constructors
    FileAccessException(const FileAccessException &e)
        : FileException(e) { }
    FileAccessException(const string &file)
        : FileException(file) { }
    FileAccessException(const string &msg, const string &file)
        : FileException(msg, file) { }
};

#endif
