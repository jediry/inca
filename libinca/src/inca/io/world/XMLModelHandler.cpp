/*
 * File: XMLModelHandler.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "ModelInterpreters.hpp"
using namespace Inca::IO;

#if XML_SUPPORT_ENABLED
XMLModelHandler::XMLModelHandler() : xml(new XMLDocument()) {
    DOMElement *model = doc->createElement(XML_MESH);
}


/*---------------------------------------------------------------------------*
| Creation functions
*---------------------------------------------------------------------------*/
// Add a new vertex to the mesh
void XMLModelHandler::createVertex(double x, double y, double z) {
    DOMElement *vertex = doc->createElement(XML_MESH_VERTEX);
    vertex->setAttribute(XML_X, ustring(x));
    vertex->setAttribute(XML_Y, ustring(y));
    vertex->setAttribute(XML_Z, ustring(z));
    vertex->setAttribute(XML_ID, (OBJ_MESH_VERTEX + vertexCount));
    model->appendChild(vertex);
}

// Add a new normal to the mesh
void XMLModelHandler::createNormal(double i, double j, double k) {
    DOMElement *normal = doc->createElement(XML_MESH_VERTEX_NORMAL);
    normal->setAttribute(XML_I, ustring(i));
    normal->setAttribute(XML_J, ustring(j));
    normal->setAttribute(XML_K, ustring(k));
    normal->setAttribute(XML_ID, (OBJ_MESH_NORMAL + normalCount));
    model->appendChild(normal);
}

// Add a new 2D texture coordinate to the mesh
void XMLModelHandler::createTexCoords(double u, double v) {
    DOMElement *texCoord = doc->createElement(XML_MESH_TEX_COORD);
    texCoord->setAttribute(XML_X, ustring(u));
    texCoord->setAttribute(XML_Y, ustring(v));
    texCoord->setAttribute(XML_ID, (OBJ_MESH_TEX_COORD + texCoordCount));
    model->appendChild(texCoord);
}

// Add a new 3D texture coordinate to the mesh
void XMLModelHandler::createTexCoords(double u, double v, double w) {
    DOMElement *texCoord = doc->createElement(XML_MESH_TEX_COORD);
    texCoord->setAttribute(XML_X, ustring(u));
    texCoord->setAttribute(XML_Y, ustring(v));
    texCoord->setAttribute(XML_Z, ustring(w));
    texCoord->setAttribute(XML_ID, (OBJ_MESH_TEX_COORD + texCoordCount));
    model->appendChild(texCoord);
}

// Start a new face
void XMLModelHandler::createFace() {
    DOMElement *face = doc->createElement(XML_MESH_FACE);
    model->appendChild(face);
}

// End the face
void XMLModelHandler::endFace() {

}

void XMLModelHandler::addVertex(unsigned int v) {

}

void XMLModelHandler::addNormal(unsigned int vn) {

}

void XMLModelHandler::addTexCoord(unsigned int vt) {

}
    // If there was only one normal, or they're all identical, we store only one
    if (mergedNormals && firstSpecifiedNormal != NONE) {
        DOMElement *normal = doc->createElement(XML_MESH_FACE_NORMAL);
        normal->setAttribute(XML_REFERENCE,
            (OBJ_MESH_NORMAL + faceNormals[firstSpecifiedNormal]));
        face->appendChild(normal);
    }

    for (int i = 0; i < faceVertexCount; i++) {
        DOMElement *vRef = doc->createElement(XML_MESH_VERTEX);
        vRef->setAttribute(XML_REFERENCE,
            (OBJ_MESH_VERTEX + faceVertices[i]));
        face->appendChild(vRef);

        // If there was more than one normal, then we don't merge them
        if (! mergedNormals && faceNormals[i] != NONE) {
            DOMElement *normal = doc->createElement(XML_MESH_VERTEX_NORMAL);
            normal->setAttribute(XML_REFERENCE,
                (OBJ_MESH_NORMAL + faceNormals[i]));
            vRef->appendChild(normal);
        }

        // If there's a texture coordinate, add that too
        if (faceTexCoords[i] != NONE) {
            DOMElement *texCoord = doc->createElement(XML_MESH_TEX_COORD);
            texCoord->setAttribute(XML_REFERENCE,
                (OBJ_MESH_TEX_COORD + faceTexCoords[i]));
            vRef->appendChild(texCoord);
        }
    }
#endif
