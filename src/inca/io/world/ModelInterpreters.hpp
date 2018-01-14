/*
 * File: ModelInterpreters.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains definitions for interpreters for various model file
 *      formats. Model (de)serialization can be done in either of two modes:
 *      XML and direct. For XML conversion, an interpreter must translate its
 *      data between an XML DOM tree and its specific format. For direct
 *      conversion, an interpreter must create a single PolygonMesh.
 */

#pragma once
#ifndef INCA_IO_MODEL_INTERPRETERS
#define INCA_IO_MODEL_INTERPRETERS

// This is part of the Inca I/O subsystem
namespace Inca {
    namespace IO {
        // Forward declarations
        class ModelInterpreter;
        class OBJInterpreter;

        class ModelHandler;
        class XMLModelHandler;      // Conversion to/from XML
        class DirectModelHandler;   // Conversion to/from live objects
    };
};

// Import superclass definition
#include "../Interpreter.hpp"

// Schema definitions
#define MODEL_SCHEMA "io/scene/inca.xsd"
#define MODEL_NAMESPACE "http://halogyn.com/inca/scene"

// XXX hack
#include <world/Objects.hpp>


/*****************************************************************************
 * ModelHandler is the base class for thingies that handle parsed data
 *****************************************************************************/
class Inca::IO::ModelHandler {
public:
    // Container typedefs
    typedef hash_map<IndexType, IndexType>  IndexMap;
    typedef hash_map<string, IndexType>   MaterialMap;

    static const IndexType NONE = 0;


/*---------------------------------------------------------------------------*
 | Constructor & accessor functions
 *---------------------------------------------------------------------------*/
public:
    ModelHandler()
        : _vertexCount(0), _normalCount(0), _texCoordsCount(0), _faceCount(0),
          currentNormal(NONE), currentTexCoords(NONE), currentMaterial(0) { }

    SizeType vertexCount() const { return _vertexCount; }
    SizeType normalCount() const { return _normalCount; }
    SizeType texCoordsCount() const { return _texCoordsCount; }
    SizeType faceCount() const { return _faceCount; }

protected:
    SizeType _vertexCount, _normalCount, _texCoordsCount, _faceCount;
    IndexType currentNormal, currentTexCoords, currentMaterial;


/*---------------------------------------------------------------------------*
 | Model creation functions
 *---------------------------------------------------------------------------*/
public:
    // Add primitives to the mesh
    virtual void createVertex(double x, double y, double z) = 0;
    virtual void createNormal(double i, double j, double k) = 0;
    virtual void createTexCoords(double u, double v) = 0;
    virtual void createTexCoords(double u, double v, double w) = 0;

    // Put 'em together into faces
    virtual void setMaterial(const string &name) = 0;
    virtual void setGroup(const string &name) = 0;
    virtual void setSmoothingGroup(IndexType sg) = 0;
    virtual void createFace() = 0;
        virtual void setNormal(IndexType vn) { currentNormal = vn; }
        virtual void setTexCoords(IndexType vt) { currentTexCoords = vt; }
        virtual void addVertex(IndexType v) = 0;
    virtual void endFace() = 0;
};


/*****************************************************************************
 * The XMLModelHandler converts model information to/from an XML document.
 *****************************************************************************/
class Inca::IO::XMLModelHandler : public ModelHandler {
public:
    // Constructor
    XMLModelHandler();

    // XMLDocument accessor
    XMLDocumentPtr document() const { return xml; }

    // Add primitives to the mesh
    void createVertex(double x, double y, double z);
    void createNormal(double i, double j, double k);
    void createTexCoords(double u, double v);
    void createTexCoords(double u, double v, double w);

    // Put 'em together into faces
    void setMaterial(const string &name);
    void setGroup(const string &name);
    void setSmoothingGroup(IndexType sg);
    void createFace();
        void addVertex(IndexType v);
    void endFace();

protected:
    XMLDocumentPtr xml;
};


/*****************************************************************************
 * The DirectModelHandler directly creats a model from the OBJ file data.
 *****************************************************************************/
class Inca::IO::DirectModelHandler : public ModelHandler {
public:
    typedef World::SolidObject3D                    SolidObject3D;
    typedef Poly::WEPolygonMesh<GeometricScalar, 3> PolygonMesh;
    typedef shared_ptr<PolygonMesh>                 PolygonMeshPtr;
    typedef PolygonMesh::Point                      Point;
    typedef PolygonMesh::Vector                     Vector;
    typedef PolygonMesh::TexCoords                  TexCoords;
    
    typedef hash_map<IndexType, PolygonMesh::VertexPtr>   VertexMap;


    // Constructor
    DirectModelHandler();

    // Object accessor
    World::ObjectPtr model() const { return _model; }

    // Add primitives to the mesh
    void createVertex(double x, double y, double z);
    void createNormal(double i, double j, double k);
    void createTexCoords(double u, double v);
    void createTexCoords(double u, double v, double w);

    // Put 'em together into faces
    void setMaterial(const string &name);
    void setGroup(const string &name);
    void setSmoothingGroup(IndexType sg);
    void createFace();
        void setNormal(IndexType vn);
        void setTexCoords(IndexType vt);
        void addVertex(IndexType v);
    void endFace();

protected:
    PolygonMeshPtr mesh;
    World::ObjectPtr _model;
    PolygonMesh::FaceVertexPtrList faceVertices;     // Temporary array

    VertexMap vertexMap;
    MaterialMap materialMap;
    IndexMap normalMap, texCoordsMap;
};


/*****************************************************************************
 * The ModelInterpreter class handles aspects common to all model formats.
 *****************************************************************************/
class Inca::IO::ModelInterpreter : public Interpreter {
public:
    // Load the model into an XML document
    void storeXML(XMLDocumentPtr xml, const string &filename) const {
#if XML_SUPPORT_ENABLED
        UnsupportedOperationException e;
        e << "ModelInterpreter::storeXML(\"" << filename
          << "\"): NOT IMPLEMENTED";
        throw e;
#else
        cerr << "ModelInterpreter::storeXML(\"" << filename
             << "\"): XML support disabled at compile time. Sorry.\n";
#endif
    }

    // Load the model into an XML document
    XMLDocumentPtr loadXML(const string &filename) const {
#if XML_SUPPORT_ENABLED
        XMLModelHandler handler;
        parse(handler, filename);
        return handler.document();
#else
        cerr << "ModelInterpreter::loadXML(\"" << filename
             << "\"): XML support disabled at compile time. Sorry.\n";
        return XMLDocumentPtr();
#endif
    }

    // Store the model directly
    void storeModel(const World::ObjectPtr obj, const string &filename) const {
        UnsupportedOperationException e;
        e << "ModelInterpreter::storeModel(\"" << filename
          << "\"): NOT IMPLEMENTED";
        throw e;
    }

    // Load the model directly
    World::ObjectPtr loadModel(const string &filename) const {
        DirectModelHandler handler;
        parse(handler, filename);
        return handler.model();
    }

protected:
    // Subclasses should implement these
    virtual void parse(ModelHandler &handler, const string &filename) const {
        UnsupportedOperationException e;
        e << "ModelInterpreter::parse(\"" << filename
          << "\"): NOT IMPLEMENTED";
        throw e;
    }
};


/*****************************************************************************
 * The OBJInterpreter class reads and writes Wavefront OBJ files.
 *****************************************************************************/
class Inca::IO::OBJInterpreter : public ModelInterpreter {
public:
    // Say what our capabilities are
    bool isReadingSupported() { return true; }
    bool isWritingSupported() { return false; }

protected:
    void parse(ModelHandler &handler, const string &filename) const;
};

#endif
