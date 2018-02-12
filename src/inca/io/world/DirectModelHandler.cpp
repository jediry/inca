/*
 * File: DirectModelHandler.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "ModelInterpreters.hpp"
using namespace Inca;
using namespace Inca::IO;

using Inca::World::Material;
using Inca::World::MaterialPtr;

// Constructor
DirectModelHandler::DirectModelHandler() : mesh(new PolygonMesh()) {
    SolidObject3D * solid = new SolidObject3D();
    solid->addApproximation(mesh);
    _model.reset(solid);
}


/*---------------------------------------------------------------------------*
| Creation functions
*---------------------------------------------------------------------------*/
// Add a new vertex to the mesh
void DirectModelHandler::createVertex(double x, double y, double z) {
    PolygonMesh::VertexPtr v = mesh->createVertex(Point(x, y, z));

    // Increment our count and record this vertex
    _vertexCount++;
    vertexMap[vertexCount()] = v;

//    cerr << " + Vertex " << vertexCount() << " (id " << v->getID() << "): "
//         << "[" << x << ", " << y << ", " << z << "]\n";
}

// Add a new normal to the mesh
void DirectModelHandler::createNormal(double i, double j, double k) {
    IndexType index = mesh->createNormal(Vector(i, j, k));

    // Increment our count and record this normal
    _normalCount++;
    normalMap[normalCount()] = index;

//    cerr << "Normal " << normalCount()
//         << " [" << i << ", " << j << ", " << k << "]\n";
}

// Add a new 2D texture coordinate to the mesh
void DirectModelHandler::createTexCoords(double u, double v) {
    IndexType index = mesh->createTexCoords(TexCoords(u, v, 0.0));

    // Increment our count and record this tex-coords
    _texCoordsCount++;
    texCoordsMap[texCoordsCount()] = index;

//    cerr << "Tex Coords " << texCoordsCount()
//         << " [" << u << ", " << v << "]\n";
}

// Add a new 3D texture coordinate to the mesh
void DirectModelHandler::createTexCoords(double u, double v, double w) {
    IndexType index = mesh->createTexCoords(TexCoords(u, v, w));

    // Increment our count and record this tex-coords
    _texCoordsCount++;
    texCoordsMap[texCoordsCount()] = index;

//    cerr << "Tex Coords " << texCoordsCount()
//         << " [" << u << ", " << v << ", " << w << "]\n";
}

// Set the material that will be used for new faces
void DirectModelHandler::setMaterial(const string &name) {
//    MaterialMap::const_iterator it = materialMap.find(name);
    IndexType matIndex = 0;
//    for (IndexType i = 0; i < _model->materials().size(); i++)
//        if (_model->material(i)->id == name)
//            matIndex = i;
    currentMaterial = matIndex;
}

// Set the group that new faces will be put in
void DirectModelHandler::setGroup(const string &name) {

}

// Set the smoothing group that new faces will be put in
void DirectModelHandler::setSmoothingGroup(unsigned int sg) {

}

// Start a new face
void DirectModelHandler::createFace() {
//    cerr << "Face " << faceCount() << " {";
    faceVertices.clear();
}

void DirectModelHandler::setNormal(IndexType vn) {
    currentNormal = normalMap[vn];
}

void DirectModelHandler::setTexCoords(IndexType vt) {
    currentTexCoords = texCoordsMap[vt];
}

void DirectModelHandler::addVertex(unsigned int v) {
//    cerr << ' ' << v << '/' << currentTexCoords << '/' << currentNormal;

    faceVertices.push_back(mesh->createFaceVertex(vertexMap[v]));
    
    // Retrieve the normal & tex-coords
    faceVertices.back()->setNormalIndex(currentNormal);
    faceVertices.back()->setTexCoordsIndex(currentTexCoords);
}

// End the face
void DirectModelHandler::endFace() {
//    cerr << " * ";
    PolygonMesh::FacePtr f = mesh->createFace(faceVertices);
    f->setMaterial(currentMaterial);
    _faceCount++;
//    cerr << "}\n";
}
