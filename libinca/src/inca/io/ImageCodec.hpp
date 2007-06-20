/*
 * File: ImageCodec.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_IO_IMAGE_CODEC
#define INCA_IO_IMAGE_CODEC


// This is part of the Inca I/O subsystem
namespace inca {
    namespace io {
        // Forward declarations
        class ImageCodec;
    };
};


class inca::io::ImageCodec {
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
