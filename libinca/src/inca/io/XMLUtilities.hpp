/*
 * File: XMLUtilities.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file defines a set of utilities for isolating the ugliness of
 *      working with the XML parser.
 *
 *      The XMLDocument class is a mostly-opaque wrapper around the XML parser,
 *      allowing us to create or parse a DOM document without having to see
 *      the nasty details.
 *
 *      The ImmediateChildFilter class is a XML-DOM filter useful for
 *      traversing each immediate child node of a particular node. This
 *      is used by the deserialization macros to extract elements and attributes
 *      belonging to a particular node.
 *
 *      Because XML support is an optional feature of Inca, if the macro
 *      XML_SUPPORT_ENABLED is not defined, then a dummy XMLDocument will be
 *      implemented (which basically just complains whenever somebody tries to use it).
 *
 *      Furthermore, as Inca can use any available DOM parser, one of the following macros
 *      must be defined:
 *          USING_XERCES_PARSER -- use the Apache Xerces parser
 */

#ifndef INCA_IO_XML_UTILITIES
#define INCA_IO_XML_UTILITIES

// Import system configuration
#include <inca/inca-common.h>

#if XML_SUPPORT_ENABLED
#   if USING_XERCES_PARSER
#       // Import the Xerces implementation of the XML Document Object Model
#       include <xercesc/dom/DOM.hpp>
#   else
#       error "No XML parser specified! Please #define one of the USING_XXX_PARSER macros"
#   endif
#endif

// This is part of the Inca I/O subsystem
namespace inca {
    namespace io {
        // Forward declarations
        class XMLDocument;

        // Pointer declaration
        typedef shared_ptr<XMLDocument> XMLDocumentPtr;
    };
};        

// These rely on having a real XML parser
#if XML_SUPPORT_ENABLED
namespace inca {
    namespace io {
        // DOM filter for extracting only the direct children of a node
        class ImmediateChildFilter;

        // XML DOM pointer types
        typedef DOMNamedNodeMap * DOMNamedNodeMapPtr;
        typedef DOMDocument * DOMDocumentPtr;
        typedef DOMElement * DOMElementPtr;
        typedef DOMAttr * DOMAttributePtr;

        // Utility function prototypes
        DOMTreeWalker * createChildWalker(DOMNode *root,
                                          unsigned long whatToShow);
        void destroyChildWalker(DOMTreeWalker *walker);
    };
};
#endif


/*****************************************************************************
 * This is a lightweight wrapper around a Xerces DOM document, hiding the
 * unusual creation/deletion semantics behind an object that can be new'd
 * and deleted normally.
 *****************************************************************************/
class inca::io::XMLDocument {
/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Create an empty document
    XMLDocument();

    // Parse an XML file
    XMLDocument(const string &filename, const string &schemas = "");

    // Destructor
    ~XMLDocument();

    // Accessor functions
    const string & filename() { return filenameString; }
    

    // Read/write functions
    void clear();
    void read(const string &filename, const string &schemas);
    void write(const string &filename);

protected:
    string filenameString;  // Where we loaded/stored this from/to
#if XML_SUPPORT_ENABLED
    DOMDocumentPtr dom;     // The DOM document we get from the parser
#endif
};


#if XML_SUPPORT_ENABLED     // None of the rest of this makes sense w/o a real parser
/*****************************************************************************
 * This DOMNodeFilter accepts only immediate children of its root
 * node (and excludes the root node itself). It must be used with
 * a DOMTreeWalker to have any effect.
 *****************************************************************************/
class inca::io::ImmediateChildFilter : public DOMNodeFilter {
public:
    ImmediateChildFilter(DOMNode *root) : rootNode(root) { }
    short acceptNode(const DOMNode *node) const {
        if (node == rootNode) {
            return FILTER_SKIP;
        } else if (node->getParentNode() == rootNode) {
            return FILTER_ACCEPT;
        } else {
            return FILTER_REJECT;
        }
    }

private:
    DOMNode *rootNode;
};


/*****************************************************************************
 * These macros are used to simplify the implementation of (de)serialization
 * functions, making both the C++ code and the XSLT stylesheet shorter and
 * more readable.
 *      VERIFY_TAG( ELEMENT, PARENT_TAG ) verifies that the parent-level
 *          element (ELEMENT) has the appropriate tag (PARENT_TAG), and
 *          prints a log message either way. In the case where the tag
 *          is unexpected, it also forces an early return.
 *
 *      PROCESS_ATTRIBUTES( ELEMENTS ) sets up a loop to iterate over each
 *          attribute of ELEMENT.
 *
 *      DESERIALIZE_ATTRIBUTE( PARENT, TAG, TYPE, PARAM ) creates a handler
 *          for a particular XML attribute. The macro will expand to
 *          a call to PARENT.setPARAM(<attribute-value>).
 *
 *      HANDLE_OTHER_ATTRIBUTES( PARENT_TAG ) should be placed after any
 *          DESERIALIZE_ATTRIBUTE macros, since it prints a log message 
 *          specifying that the current attribute is invalid in this context.
 *
 *      PROCESS_CHILDREN( ELEMENT ) sets up a loop to iterate over each
 *          direct child element of ELEMENT.
 *
 *      DESERIALIZE_RECURRING_ELEMENT( PARENT, TAG, TYPE, PARAM ) creates a
 *          handler for a particular XML element. If the child element's name
 *          matches TAG, then the PARENT's "createPARAM()" method is called
 *          (it is expected to return a TYPEPtr). This pointer's target is
 *          used for recursively calling the >> deserialization
 *          operator, which should handle initializing the TYPE and any
 *          subobjects. If deserialization fails, the PARENT's
 *          "destroyTYPE(TYPEPtr)"method is called, in order to clean up the
 *          partially-initialized object. This macro should be used for
 *          elements that may have multiple instances.
 *
 *      DESERIALIZE_UNIQUE_ELEMENT( PARENT, TAG, TYPE, PARAM ) creates a handler
 *          for a particular XML element. In contrast to the above macro,
 *          this macro is intended for elements that must be unique amonst
 *          their sibling elements.
 *
 *      HANDLE_OTHER_ELEMENTS( PARENT_TAG ) should be placed after any
 *          DESERIALIZE_XXX_ELEMENT macros, since it prints a log message 
 *          specifying that the current tag is invalid in this context.
 *
 *      CLEAN_UP destroys any objects created by PROCESS_CHILDREN
 */

#define SINGLE_CONTEXT_ELEMENT( TAG_NAME ) do {             \
        /* Adjust our indentation appropriately */          \
        logger.increaseIndentLevel();                       \
                                                            \
        /* Check and see if this is what we think */        \
        ustring checkTag(e->getTagName());                  \
        if (checkTag != TAG_NAME) {                         \
            logger << "Expected <" << TAG_NAME              \
                << ">, found <" << checkTag                 \
                << "> -- Ignoring element";                 \
            logger.warning();                               \
            return;                                         \
        } else {                                            \
            /* OK! We're good to go! */                     \
            logger << DESERIALIZATION << TAG_NAME;          \
            logger.info();                                  \
        }                                                   \
    } while (false) /* no trailing ';', supplied by caller */

#define MULTI_CONTEXT_ELEMENT do {                          \
        /* Adjust our indentation appropriately */          \
        logger.increaseIndentLevel();                       \
                                                            \
        /* OK! We're good to go! */                         \
        logger << DESERIALIZATION << e->getTagName();       \
        logger.info();                                      \
    } while (false) /* no trailing ';', supplied by caller */

#define FOREACH_ATTRIBUTE                                   \
    /* Iterate over each attribute */                       \
    DOMNamedNodeMapPtr map = e->getAttributes();            \
    DOMAttributePtr attr;                                   \
    ustring attrName;                                       \
    XMLSize_t size = map->getLength();                      \
    int j = -1;                                             \
    while (++j < size                                       \
        && (attr = (DOMAttributePtr)map->item(j)) != NULL   \
        && (attrName = attr->getName()) != "")              \
    /* no '{', supplied by caller */

#define BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, TARGET )     \
    if (attrName == TAG_NAME) {                             \
        ustring value = attr->getValue();                   \
        logger.increaseIndentLevel();                       \
        logger << DESERIALIZATION << #TARGET << " = "       \
               << value;                                    \
        logger.info();

#define END_DESERIALIZE_ATTRIBUTE                           \
        continue;                                           \
    }

#define DESERIALIZE_ID_ATTRIBUTE( TAG_NAME, PROPERTY )      \
    BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, PROPERTY );      \
    /* Construct the reference string */                    \
    ustring filename = e->getOwnerDocument()->getDocumentURI(); \
    value = filename + "#" + value;                         \
                                                            \
    /* Perform the property assignment */                   \
    n->PROPERTY = value;                                    \
    END_DESERIALIZE_ATTRIBUTE;

#define DESERIALIZE_REFERENCE_ATTRIBUTE( TAG_NAME, PROPERTY )
    /* We ignore this, for right now */

#define DESERIALIZE_BOOLEAN_ATTRIBUTE( TAG_NAME, PROPERTY ) \
    BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, PROPERTY );      \
    /* Perform the property assignment */                   \
    n->PROPERTY = (bool)value;                              \
    END_DESERIALIZE_ATTRIBUTE;

#define DESERIALIZE_INTEGER_ATTRIBUTE( TAG_NAME, PROPERTY ) \
    BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, PROPERTY );      \
    /* Perform the property assignment */                   \
    n->PROPERTY = (int)value;                               \
    END_DESERIALIZE_ATTRIBUTE;

#define DESERIALIZE_SCALAR_ATTRIBUTE( TAG_NAME, PROPERTY )  \
    BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, PROPERTY );      \
    /* Perform the property assignment */                   \
    n->PROPERTY = (double)value;                            \
    END_DESERIALIZE_ATTRIBUTE;

#define DESERIALIZE_STRING_ATTRIBUTE( TAG_NAME, PROPERTY )  \
    BEGIN_DESERIALIZE_ATTRIBUTE( TAG_NAME, PROPERTY );      \
    /* Perform the property assignment */                   \
    n->PROPERTY = (string)value;                            \
    END_DESERIALIZE_ATTRIBUTE;

#define HANDLE_UNRECOGNIZED_ATTRIBUTE( TAG_NAME ) do {      \
        logger << "Expected <" << TAG_NAME << ">-level "    \
                  "attribute, not @" << attrName            \
               << " -- Ignoring attribute";                 \
        logger.warning();                                   \
    } while (false) /* no trailing ';', supplied by caller */

#define FOREACH_ELEMENT( ELEMENT )                          \
    /* Iterate over each direct child element */            \
    DOMTreeWalker *i = createChildWalker(ELEMENT,           \
            DOMNodeFilter::SHOW_ELEMENT);                   \
    DOMElementPtr child;                                    \
    ustring childTag;                                       \
    while ((child = (DOMElementPtr)i->nextNode()) != NULL   \
            && (childTag = child->getTagName()) != "")      \
    /* no '{', supplied by caller */

#define DESERIALIZE_RECURRING_ELEMENT( PARENT, TAG, TYPE, PARAM ) \
    if (tagName == TAG) {                                   \
        /* Create an object */                              \
        TYPE & instance = PARENT.create##PARAM();           \
                                                            \
        /* Deserialize -- if we fail, clean up the mess */  \
        if (!(child >> instance)) {                         \
            logger << "Deserialization failed";             \
            logger.warning();                               \
        }                                                   \
                                                            \
        continue; /* Next tag, please */                    \
    }                                                       \
    while(false) /* no trailing ';', supplied by caller */

#define DESERIALIZE_UNIQUE_ELEMENT( PARENT, TAG, TYPE, PARAM ) \
    if (tagName == TAG) {                                   \
        /* See if we already have an instance */            \
        TYPE & instance = PARENT.get##PARAM();              \
                                                            \
        /* Deserialize -- if we fail, clean up */           \
        if (!(child >> instance)) {                         \
            logger << "Deserialization failed";             \
            logger.warning();                               \
        }                                                   \
                                                            \
        continue; /* Next tag, please */                    \
    }                                                       \
    while(false) /* no trailing ';', supplied by caller */

#define HANDLE_UNRECOGNIZED_ELEMENT( TAG_NAME ) do {        \
        logger << "Expected <" << TAG_NAME << ">-level "    \
                  "element, not <" << childTag              \
               << "> -- Ignoring element";                  \
        logger.warning();                                   \
    } while (false) /* no trailing ';', supplied by caller */

#define CLEAN_UP do {                                       \
        destroyChildWalker(i);  /* Clean up attr walker */  \
        logger.decreaseIndentLevel();                       \
        return true;                                        \
    } while (false) /* no trailing ';', supplied by caller */

#endif  // XML_SUPPORT_ENABLED

#endif  // Multi-include protection
