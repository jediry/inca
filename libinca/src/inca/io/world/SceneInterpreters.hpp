/*
 * File: Interpreters.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains definitions for interpreters for various scene file
 *      formats. Converters to non-Inca formats must translate their data
 *      between an XML DOM tree and their specific non-Inca format.
 */

#ifndef INCA_IO_SCENE_INTERPRETERS
#define INCA_IO_SCENE_INTERPRETERS

// Include superclass definition
#include "../Interpreter.hpp"

// Schema definitions
#define SCENE_SCHEMA "/home/jediry/Documents/Projects/inca/src/io/scene/inca.xsd"
#define SCENE_NAMESPACE "http://halogyn.com/inca/scene"


// This is part of the Inca I/O subsystem
namespace Inca {
    namespace IO {
        // Forward declarations
        class SceneInterpreter;
        class INCAInterpreter;
        class POVInterpreter;
    };
};


class Inca::IO::SceneInterpreter : public Interpreter {
public:
    // Constructor
    SceneInterpreter() {
        useSchema(SCENE_NAMESPACE, SCENE_SCHEMA);
    }
};


class Inca::IO::INCAInterpreter : public SceneInterpreter {
public:
    // Convert between a file and an XML document
    void storeXML(XMLDocumentPtr xml, const string &filename) const;
    XMLDocumentPtr loadXML(const string &filename) const;

    // Say what our capabilities are
    bool isReadingSupported() const { return true; }
    bool isWritingSupported() const { return true; }
};


class Inca::IO::POVInterpreter : public SceneInterpreter {
public:
    // Barf the XML DOM tree out to a POV-Ray scene file
//    void storeXML(XMLDocumentPtr xml, const string &filename) const;

    // Slurp a file into the XML DOM tree from a POV-Ray scene file
//    XMLDocumentPtr loadXML(const string &filename) const;

    // Say what our capabilities are
//    bool isReadingSupported() const { return false; }
//    bool isWritingSupported() const { return false; }
};

#endif
