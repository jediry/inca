/*
 * File: INCAInterpreter.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The INCAInterpreter class handles reading and writing scene data in
 *      the INCA-native XML file format. The contents of the .inca file are
 *      converted to/from an XML DOM tree.
 *
 *      The XML DOM document is used as an intermediary representation between
 *      the actual scene objects and the scene file.
 *
 *      FIXME: We don't do any error checking on file-write.
 */

// Import class definition
#include "SceneInterpreters.hpp"
using namespace Inca;
using namespace Inca::IO;

// Import Unicode string definition
#include <util/ustring.hpp>

void INCAInterpreter::storeXML(XMLDocumentPtr xml, const string &filename) const {
    ustring s(filename);

    if (xml == NULL) {
        logger << "INCAInterpreter::storeToFile() -- XML document is NULL!";
        logger.error();
    } else {
        logger << "Writing Inca-native file \"" << filename << "\"";
        xml->write(filename);
        logger << " -- Success";
        logger.info();
    }
}

XMLDocumentPtr INCAInterpreter::loadXML(const string &filename) const {
    logger << "Reading Inca-native file \"" << filename << "\"";
    XMLDocumentPtr xml(new XMLDocument(filename));
    if (xml != NULL) {
        logger << " -- Success";
        logger.info();
    } else {
        logger << " -- FAILURE";
        logger.error();
    }
    return xml;
}
