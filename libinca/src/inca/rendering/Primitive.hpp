/*
 * File: Primitive.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_RENDERING_PRIMITIVE
#define INCA_RENDERING_PRIMITIVE

// Import system configuration
#include <inca-common.h>


// This is part of the Inca rendering subsystem
namespace Inca {
    namespace Rendering {
        // Enumeration of the different primitives
        enum PrimitiveType {
            Points              = 0,
            Lines               = 1,
            LineStrip           = 2,
            LineLoop            = 3,
            Triangles           = 4,
            TriangleStrip       = 5,
            TriangleFan         = 6,
            Quads               = 7,
            QuadStrip           = 8,
            Polygon             = 9,
            BillboardSprite     = 10,
        };
        const unsigned ALL_PRIMITIVE_TYPES = BillboardSprite + 1;


        // Enumeration of different ways of handling of normal vectors
        enum NormalType {
            NoNormal    = 0,
            Flat        = 1,
            Smooth      = 2,
        };
        const unsigned ALL_NORMAL_TYPES = Smooth + 1;


        // Enumeration of different sorts of texture coordinates
        enum TextureType {
            NoTexture   = 0,
            Texture1D   = 1,
            Texture2D   = 2,
            Texture3D   = 3,
        };
        const unsigned ALL_TEXTURE_TYPES = Texture3D + 1;


        // ostream writers for enumerated types
        inline ostream & operator<<(ostream &o, PrimitiveType type) {
            switch (type) {
                case Points:            return o << "Points";
                case Lines:             return o << "Lines";
                case LineStrip:         return o << "LineStrip";
                case LineLoop:          return o << "LineLoop";
                case Triangles:         return o << "Triangles";
                case TriangleStrip:     return o << "TriangleStrip";
                case TriangleFan:       return o << "TriangleFan";
                case Quads:             return o << "Quads";
                case QuadStrip:         return o << "QuadStrip";
                case Polygon:           return o << "Polygon";
                case BillboardSprite:   return o << "BillboardSprite";
                default:                return o << "UNKNOWN PrimitiveType";
            }
        }

        ostream & operator<<(ostream &o, NormalType type);
        ostream & operator<<(ostream &o, TextureType type);


        // Forward declarations
        template <PrimitiveType p, NormalType n, TextureType t> class Primitive;
    };
};

#endif
