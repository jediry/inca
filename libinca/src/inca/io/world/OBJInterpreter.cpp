/*
 * File: OBJInterpreter.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The OBJInterpreter class handles reading and writing model data in
 *      the Wavefront OBJ file format. The contents of the .obj file are
 *      converted to/from an XML DOM tree.
 *
 *      The XML DOM document is used as an intermediary representation.
 *      The actual scene objects are reconstituted from this DOM tree
 *      representation.
 */

//XXX maybe do checking for separately specified, but identical normals/vertices?


// Import class definition
#include "ModelInterpreters.hpp"

// Import XML document model
#include "../XMLUtilities.hpp"

using namespace Inca;
using namespace Inca::IO;

// We're using StreamTokenizer to tokenize the input stream
#include <util/StreamTokenizer.hpp>


// This is part of the Inca I/O subsystem
namespace Inca {
    namespace IO {
        // Parsing states
        enum PrimaryStateType { // What kind of line we're parsing
            ExpectingDeclaration,  // We expect a 'v', 'f', etc. declaration
            ParsingVertex,
            ParsingFace,
            ParsingNormal,
            ParsingTexCoords,
            ParsingMaterial,
            ParsingGroup,
            ParsingSmoothingGroup,
        };

        enum SecondaryStateType {   // Where we're at on the line
            ExpectingVertex,
            ExpectingVertexOrTextureDelimiter,
            ExpectingVertexOrNormalDelimiter,
            ExpectingTextureOrNormalDelimiter,
            ExpectingNormal,
            ExpectingName,
            ExpectingID,
        };
    };
};


void OBJInterpreter::parse(ModelHandler &modelHandler,
                           const string &filename) const {

    // Make this 'true' to see state transitions in this function
    bool debug = false;

    ifstream file(filename.c_str());

    if (!file) {
        FileAccessException e(filename);
        e << "Unable to read Wavefront OBJ file \"" << filename << '"';
        throw e;
    }

    // Prepare to parse the file
    StreamTokenizer strTok(&file);
    strTok.setEOLisSignificant(true);
    strTok.setCaseConversion(StreamTokenizer::CASE_CONVERT_TO_UPPER);
    strTok.setCommentCharacter('#');     // Standard UNIX comments
    strTok.setDelimiterCharacter('/');   // '/'s can divide numbers
    strTok.setParseNumbers(true);

    short currentState[3], nextState[3];    // State vector
    currentState[0] = ExpectingDeclaration;
    nextState[0]    = ExpectingDeclaration;
    currentState[1] = ExpectingVertex;
    nextState[1]    = ExpectingVertex;
    unsigned int vertexToAdd, index;
    short valuesRead = 0;
    double values[3];       // Where we store the coordinates as we parse
//    unsigned int vertexCount = 0, normalCount = 0,
//                 texCoordCount = 0, faceCount = 0;
//    unsigned int faceVertices[MAX_VERTICES],
//                 faceTexCoords[MAX_VERTICES],
//                 faceNormals[MAX_VERTICES];
//    unsigned int faceVertexCount;

    StreamTokenizer::TokenType tokenType = strTok.nextToken();
    while (tokenType != StreamTokenizer::TOKEN_TYPE_EOF) {
        // Make this exception, just in case we need to throw it
        FileFormatException e(filename, strTok.getLineNumber());

        switch (tokenType) {

        case StreamTokenizer::TOKEN_TYPE_WORD:  // This mostly occurs at the start of the line
            if (currentState[0] == ExpectingDeclaration) { // Starting a new line
                if (strTok.wordToken == "V") {           // Parsing a vertex
                    nextState[0] = ParsingVertex;
                    valuesRead = 0;
                } else if (strTok.wordToken == "VN") {   // Parsing a vertex normal
                    nextState[0] = ParsingNormal;
                    valuesRead = 0;
                } else if (strTok.wordToken == "VT") {   // Parsing a texture coordinate
                    nextState[0] = ParsingTexCoords;
                    valuesRead = 0;
                } else if (strTok.wordToken == "G") {       // Face group
                    strTok.skipToEOL();
//                    nextState[0] = ParsingGroup;
//                    nextState[1] = ExpectingName;
                } else if (strTok.wordToken == "S") {       // Smoothing group
 //                   cerr << "Skipping S\n";
                    strTok.skipToEOL();
//                    nextState[0] = ParsingSmoothingGroup;
//                    nextState[1] = ExpectingID;
                } else if (strTok.wordToken == "USEMTL") {  // Material
                    nextState[0] = ParsingMaterial;
                    nextState[1] = ExpectingName;
                } else if (strTok.wordToken == "MTLLIB") {  // Material library
                    cerr << "Skipping USEMTL\n";
                    strTok.skipToEOL();
                } else if (strTok.wordToken == "F") {    // Parsing a face
                    nextState[0] = ParsingFace;
                    nextState[1] = ExpectingVertex;
                    modelHandler.createFace();  // Start building a new face

                } else {            // Um...what IS this??
                    // Awww, crap.
                    e << "Unrecognized token \"" + strTok.wordToken + '"';
                    throw e;
                }
            } else if (currentState[0] == ParsingMaterial
                       && currentState[1] == ExpectingName) {
                modelHandler.setMaterial(strTok.wordToken);
                nextState[0] = ExpectingDeclaration;    // We're done.
            } else if (currentState[0] == ParsingGroup
                       && currentState[1] == ExpectingName) {
                modelHandler.setGroup(strTok.wordToken);
                nextState[0] = ExpectingDeclaration;    // We're done.
            } else {
                // No, no, NO! Not NOW, you fool!!
                e << "Unexpected token \"" + strTok.wordToken + '"';
                throw e;
            }
            break;

        case StreamTokenizer::TOKEN_TYPE_NUMBER:
            switch (currentState[0]) {
            case ExpectingDeclaration:     // Uh oh! What is this??
                // Nooooooooo!!!!
                e << "Unexpected numeric value " << strTok.numberToken;
                throw e;
                break;
            case ParsingVertex: case ParsingTexCoords: case ParsingNormal:
                // Store the number for later
                values[valuesRead++] = strTok.numberToken;
                break;
            case ParsingFace:      // Faces have to be handled separately
                // This is the index of...something...
                index = static_cast<unsigned>(strTok.numberToken);

                switch (currentState[1]) {
                case ExpectingVertexOrNormalDelimiter:  /* Fall thru */
                case ExpectingVertexOrTextureDelimiter:
                    // Go ahead and stick in the previous vertex -- it's done
                    modelHandler.addVertex(vertexToAdd);
                case ExpectingVertex:
                    // Does this vertex exist?
                    if (index > modelHandler.vertexCount()) {
                        // Die, Bart! Die!
                        e << "Vertex #" << index << " does not exist!";
                        throw e;
                    } else {
                        // We can't add this yet, 'cause we need the normal/tex
                        vertexToAdd = index;
                        nextState[1] = ExpectingVertexOrTextureDelimiter;
                    }
                    break;
                case ExpectingTextureOrNormalDelimiter:
                    // Do these tex-coords exist?
                    if (index > modelHandler.texCoordsCount()) {
                        // The world is not enough...
                        e << "Tex Coord#" << index << " does not exist!";
                        throw e;
                    } else {
                        // OK! Set this as our current tex coords
                        modelHandler.setTexCoords(index);
                        nextState[1] = ExpectingVertexOrNormalDelimiter;
                    }
                    break;
                case ExpectingNormal:
                    // Does this normal exist?
                    if (index > modelHandler.normalCount()) {
                        // Stimpy! You EEEE-diot!
                        e << "Normal #" << index << " does not exist!";
                        throw e;
                    } else {
                        // OK! Set this as our current normal
                        modelHandler.setNormal(index);

                        // We're done w/ this vertex, so add it in
                        modelHandler.addVertex(vertexToAdd);

                        nextState[1] = ExpectingVertex;
                    }
                    break;
                default:
                    cerr << "Parsing face: Illegal secondary state "
                        << currentState[1] << endl;
                    break;
                }
                break;

            case ParsingSmoothingGroup:
                modelHandler.setSmoothingGroup((unsigned int)strTok.numberToken);
                nextState[0] = ExpectingDeclaration;    // We're done.
                break;
            default:
                cerr << "Unexpected numeric token " << strTok.numberToken
                     << " in state " << currentState[0] << ", "
                     << currentState[1] << endl;
            }
            break;

        case StreamTokenizer::TOKEN_TYPE_DELIMITER:
            // Make sure we're in the right parsing state for a delimiter
            if (currentState[0] != ParsingFace) {
                // Horrible, bloody death!
                e << "Unexpected delimiter '" << strTok.delimiterToken << "'";
                throw e;
            }

            switch (currentState[1]) {
            case ExpectingVertexOrTextureDelimiter:
                nextState[1] = ExpectingTextureOrNormalDelimiter;
                break;
            case ExpectingTextureOrNormalDelimiter:
            case ExpectingVertexOrNormalDelimiter:
                nextState[1] = ExpectingNormal;
                break;
            }
            break;

        case StreamTokenizer::TOKEN_TYPE_EOL:
            switch (currentState[0]) {
            case ExpectingDeclaration:     // Hmmm...empty line? Comment?
                /* No Op */
                break;
            case ParsingTexCoords:
                if (valuesRead == 2) {
                    modelHandler.createTexCoords(values[0], values[1]);
                    nextState[0] = ExpectingDeclaration;
//                    cerr << "  Texture Coordinates (2D) <"
//                         << values[0] << "," << values[1] << ">\n";
                } else if (valuesRead == 3) {   // Hey! a THREE-DEE coordinate!
                    modelHandler.createTexCoords(values[0], values[1], values[2]);
                    nextState[0] = ExpectingDeclaration;
//                    cerr << "  Texture Coordinates (3D) <" << values[0]
//                         << "," << values[1] << "," << values[2] << ">\n";
                } else {
                    // Aaaaarrrrrrgggggghhhhhhh!
                    e << "Tex Coord: 2 or 3 coordinates required";
                    throw e;
                }
                break;
            case ParsingVertex:
                if (valuesRead == 3) {                      // We got a vertex!
                    modelHandler.createVertex(values[0], values[1], values[2]);
                    nextState[0] = ExpectingDeclaration;
//                    cerr << "  Vertex <" << values[0] << "," << values[1]
//                         << "," << values[2] << ">\n";
                } else {
                    // Scream bloody murder
                    e << "Vertex: 3 coordinates required";
                    throw e;
                }
                break;            
            case ParsingNormal:
                if (valuesRead == 3) {                      // We got a normal!
                    modelHandler.createNormal(values[0], values[1], values[2]);
                    nextState[0] = ExpectingDeclaration;
//                    cerr << "  Normal <" << values[0] << "," << values[1]
//                         << "," << values[2] << ">\n";
                } else {
                    // Uh oh, something has gone horribly wrong
                    e << "Normal: 3 coordinates required";
                    throw e;
                }
                break;            
            case ParsingFace:
                if (currentState[1] != ExpectingVertex)
                    // Add in the last vertex
                    modelHandler.addVertex(vertexToAdd);

                // Finish out the face
                modelHandler.endFace();

                // Go back to the ready state
                nextState[0] = ExpectingDeclaration;
                break;
//                if (faceVertexCount <= 2) {
//                    cerr << "Warning: " << filename << "(line "
//                         << strTok.getLineNumber() << "): Degenerate face\n";
//                }

                // Here, we do some consistency & data reduction checks. We
                // check to see if all the vertices with normals specifed are
                // using the same normal. We also check here to ensure that no
                // out-of-range vertices/normals/tex coords are specified.
//                bool mergedNormals = true;
//                int firstSpecifiedNormal = NONE;
//                for (unsigned int i = 0; i < faceVertexCount; i++) {
                    // Does this vertex exist?
/*                    if (faceVertices[i] > vertexCount) {
                        FileFormatException e(filename, strTok.getLineNumber());
                        e << "Vertex #" << faceVertices[i] << " does not exist!";
                        throw e;
                    }
                    // Does this normal exist?
                    if (faceNormals[i] != NONE && faceNormals[i] > normalCount) {
                        FileFormatException e(filename, strTok.getLineNumber());
                        e << "Normal #" << faceNormals[i] << " does not exist!";
                        throw e;
                    }
                    // Does this tex-coord exist?
                    if (faceTexCoords[i] != NONE && faceTexCoords[i] > texCoordCount) {
                        FileFormatException e(filename, strTok.getLineNumber());
                        e << "Tex Coord#" << faceTexCoords[i] << " does not exist!";
                        throw e;
                    }
                    // Can we still merge the face normals?
                    if (faceNormals[i] != NONE) {
                        if (firstSpecifiedNormal == NONE) { //... this is the first
                            firstSpecifiedNormal = i;
                        } else {                            //...we're not first
                            if (faceNormals[i] != faceNormals[firstSpecifiedNormal]) {
                                mergedNormals = false;  //...hmmm multiple normals...
                            }
                        }
                    }
                }
                
                modelHandler.beginFace();

                // If there was only one normal, or they're all identical, we store only one
                if (mergedNormals && firstSpecifiedNormal != NONE) {
                    // What do we do here?
                }

                for (unsigned int i = 0; i < faceVertexCount; i++) {
                    // If there was more than one normal, then we don't merge them
                    if (! mergedNormals && faceNormals[i] != NONE)
                        modelHandler.setNormal(faceNormals[i]);

                    // If there's a texture coordinate, add that too
                    if (faceTexCoords[i] != NONE)
                        modelHandler.setTexCoord(faceTexCoords[i]);

                    // Now create the vertex
                    modelHandler.addVertex(faceVertices[i]);
                }

                endFace();

                nextState[0] = ExpectingDeclaration;
                break;
            */
            }
            break;
        case StreamTokenizer::TOKEN_TYPE_NONE:
            cerr << "StreamTokenizer error: TOKEN_TYPE_NONE\n";
            break;
        default:
            cerr << "StreamTokenizer error: bad token type " << tokenType << endl;
            break;
        }

        // Print debug/error messages & make state transitions
        if (currentState[0] != nextState[0] || currentState[1] != nextState[1]) {
            if (debug)
                cerr << "OBJInterpreter: transition [" << currentState[0]
                    << "," << currentState[1] << "] -> [" << nextState[0]
                    << "," << nextState[1] << "]\n";
            currentState[0] = nextState[0];
            currentState[1] = nextState[1];
        }

        // Get the next token and do it again
        tokenType = strTok.nextToken();
    }

    cerr << "OBJInterpreter: Parsed \"" << filename << "\"  Successfully\n";
}
