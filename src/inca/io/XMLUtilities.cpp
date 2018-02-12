/*
 * File: XMLUtilities.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the XML-handling utilities defined in
 *      XMLUtilities.hpp.
 *
 *      The LoggerErrorHandler class is an implementation of the XML-parser's
 *      ErrorHandler, which subclass redirects errors to an Inca::IO Logger.
 *      This is used by the XMLDocument object to report errors.
 */

// Include precompiled header
#include <inca/precomp.h>

// Include the function prototypes and Xerces XML parser
#include "XMLUtilities.hpp"
using namespace Inca;
using namespace Inca::IO;

// Import exception classes for working with files
#include "FileExceptions.hpp"

// Import Unicode string definition
#include <util/ustring.hpp>


#if XML_SUPPORT_ENABLED
#   if USING_XERCES_PARSER
        // Import the Xerces XML parser
#       include <xercesc/parsers/XercesDOMParser.hpp>
#       include <xercesc/util/PlatformUtils.hpp>
#       include <xercesc/framework/LocalFileInputSource.hpp>
#       include <xercesc/framework/LocalFileFormatTarget.hpp>
#       include <xercesc/sax/HandlerBase.hpp>
#   else
#       error "No XML parser specified! Please #define one of the USING_XXX_PARSER macros"
#   endif
#endif


#if XML_SUPPORT_ENABLED     // None of this makes any sense w/o a real XML parser
// This is part of the Inca I/O subsystem
namespace Inca {
    namespace IO {
        /*********************************************************************
        * This implementation of ErrorHandler logs warnings and errors to a
        * Logger instance (usually, the global Logger).
        *********************************************************************/
        class LoggerErrorHandler : public ErrorHandler {
        public:
            // Constructor
            LoggerErrorHandler(Logger &log) : logger(log) { }

            void warning(const SAXParseException &e) {
                logger << e.getSystemId() << ": line " << e.getLineNumber()
                    << ", column " << e.getColumnNumber()
                    << " -- " << e.getMessage();
                logger.warning();
            }

            void error(const SAXParseException &e) {
                logger << e.getSystemId() << ": line " << e.getLineNumber()
                    << ", column " << e.getColumnNumber()
                    << " -- " << e.getMessage();
                logger.error();
            }

            void fatalError(const SAXParseException &e) {
                logger << e.getSystemId() << ": line " << e.getLineNumber()
                    << ", column " << e.getColumnNumber()
                    << " -- " << e.getMessage();
                logger.fatal();
                throw e;
            }

            void resetErrors() { /* No-op */ }

        protected:
            Logger &logger;
        };
    };
};


/*****************************************************************************
 * DOM tree walker functions
 *****************************************************************************/
DOMTreeWalker * Inca::IO::createChildWalker(DOMNode *root, unsigned long whatToShow) {
    ImmediateChildFilter *filter = new ImmediateChildFilter(root);
    DOMDocument *doc = root->getOwnerDocument();
    if (doc == NULL)    // Then this WAS the DOMDocument
        doc = (DOMDocument*)root;
    DOMTreeWalker *walker = doc->createTreeWalker(root, whatToShow, filter, false);
    return walker;
}

void Inca::IO::destroyChildWalker(DOMTreeWalker *walker) {
    if (walker != NULL) {
        DOMNodeFilter *filter = walker->getFilter();
        if (filter != NULL)
            delete filter;      // Filter was ours...delete it
        walker->release();      // Walker was theirs...let them pick it up
    }
}
#endif


/*****************************************************************************
 * Wrapper object for managing the underlying Xerces DOM
 *****************************************************************************/
// Create an empty document
XMLDocument::XMLDocument() {
#if XML_SUPPORT_ENABLED
    XMLPlatformUtils::Initialize(); // Ensure that the parser is initialized
    clear();                        // Create an empty document
#else
    logger << "XMLDocument::XMLDocument(): "
              "XML support disabled at compile time";
    logger.warning();
#endif
}

// Parse an XML file
XMLDocument::XMLDocument(const string &filename, const string &schemas) {
#if XML_SUPPORT_ENABLED
    XMLPlatformUtils::Initialize(); // Ensure that the parser is initialized
    read(filename, schemas);        // Parse the document from the file
#else
    logger << "XMLDocument::XMLDocument(" << filename << ", " << schemas << "): "
              "XML support disabled at compile time";
    logger.warning();
#endif
}

// Free the document resources
XMLDocument::~XMLDocument() {
#if XML_SUPPORT_ENABLED
    if (dom != NULL)
        dom->release();
    XMLPlatformUtils::Terminate();
#endif
}

// Free any old document, and create a blank one
void XMLDocument::clear() {
#if XML_SUPPORT_ENABLED
    // Be sure to clean up any previous document we had
    if (dom != NULL) {
        dom->release();
        dom = NULL;
    }

    // Get an empty document from the DOM implementation
    DOMImplementation *impl = DOMImplementation::getImplementation();
    dom = impl->createDocument(NULL, NULL, NULL);

    // We now have no filename
    filenameString = "";
#endif
}

// Parse an XML file
void XMLDocument::read(const string &filename, const string &schemas) {
#if XML_SUPPORT_ENABLED
    // Be sure to clean up any previous document we had
    if (dom != NULL) {
        dom->release();
        dom = NULL;
    }

    // Create an XML parser
    scoped_ptr<XercesDOMParser> parser(new XercesDOMParser());
    scoped_ptr<ErrorHandler> errorHandler(new LoggerErrorHandler(logger));
    parser->setErrorHandler(errorHandler.get());
    parser->setDoNamespaces(true);
    if (schemas != "") {
        parser->setDoSchema(true);
        parser->setExternalSchemaLocation(schemas.c_str());
    }

    // Now parse the file into a DOMDocument
    try {
        parser->parse(ustring(filename));
        dom = parser->adoptDocument();
        filenameString = filename;  // This is the new filename

    } catch (const XMLException &e) {
        // Rethrow this as a FileFormatException
        ustring file(e.getSrcFile());
        ustring msg(e.getMessage());
        unsigned int line = e.getSrcLine();
        throw FileFormatException(msg, file, line);

    } catch (const SAXException &e) {
        // Rethrow this as a FileAccessException
        ustring msg(e.getMessage());
        throw FileAccessException(msg, filename);

    } catch (const DOMException &e) {
        // Rethrow this as a FileAccessException
        ustring msg(e.msg);
        throw FileAccessException(msg, filename);

    } catch (...) {
        logger << "XMLDocument: Ack! Unknown exception!";
        logger.error();
    }
#else
    logger << "XMLDocument::read(" << filename << ", " << schemas << "): "
              "XML support disabled at compile time";
    logger.error();
#endif
}

// Write the XML document to a local file
void XMLDocument::write(const string &filename) {
#if XML_SUPPORT_ENABLED
    DOMImplementation *impl = DOMImplementation::getImplementation();
    LocalFileFormatTarget file(filename.c_str());
    DOMWriter *writer = impl->createDOMWriter();
    if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    if (writer->canSetFeature(XMLUni::fgDOMWRTCanonicalForm, true))
        writer->setFeature(XMLUni::fgDOMWRTCanonicalForm, true);
    writer->writeNode(&file, *dom);
    writer->release();
#else
    logger << "XMLDocument::write(" << filename << "): "
              "XML support disabled at compile time";
    logger.error();
#endif
}

