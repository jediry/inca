/*
 * File: Interpreter.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 * FIXME:
 *   rewrite this comment
 *   throw UnsupportedOperationException by default
 *      The Interpreter class is the superclass for classes that read and/or
 *      write various file formats. Converters for non-Inca formats must
 *      translate their data between an XML DOM tree and their specific
 *      non-Inca format.
 *
 *      The XML DOM document is used as an intermediary representation.
 *      The actual scenei/model objects are reconstituted from this DOM tree
 *      representation.
 *
 *      By default, the Interpreter owns the XML DOM. This can be changed by
 *      calling setDOMOwnership(false). Setting this flag to false disallows
 *      the Interpreter from freeing the XML parser resources occupied by
 *      the DOMDocument (specifically, the destructor and the destroyDOM()
 *      method will only nullify the Interpreter's pointer to the DOM, and
 *      will not actually release the resources allocated to the DOM object).
 */

#pragma once
#ifndef INCA_IO_INTERPRETER
#define INCA_IO_INTERPRETER

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca I/O subsystem
namespace inca {
    namespace io {
        // Forward declarations
        class Interpreter;
    };
};

// Import our XMLDocument & utilities
#include "XMLUtilities.hpp"

// Import exception classes
#include "FileExceptions.hpp"
#include <util/UnsupportedOperationException.hpp>


class inca::io::Interpreter {
public:
    // Filetype-specific subclasses must override these functions
    virtual void storeXML(XMLDocumentPtr xml, const string &filename) const {
        UnsupportedOperationException e;
        e << "Interpreter::storeToFile(\"" << filename << "\"): NOT IMPLEMENTED";
        throw e;
    }

    virtual XMLDocumentPtr loadXML(const string &filename) const {
        UnsupportedOperationException e;
        e << "Interpreter::loadFromFile(\"" << filename << "\"): NOT IMPLEMENTED";
        throw e;
    }

    virtual bool isReadingSupported() { return false; }
    virtual bool isWritingSupported() { return false; }

protected:
    // Add this schema to the list
    void useSchema(const string &ns, const string &schema) {
        schemaList += ns + " " + schema + " ";
    }

    string schemaList;
};

#endif
