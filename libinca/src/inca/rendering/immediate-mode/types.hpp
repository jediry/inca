/*
 * File: types.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_RENDERING_TYPES
#define INCA_RENDERING_TYPES

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca rendering subsystem
namespace inca {
    namespace rendering {
        // Enumeration of the different primitive types
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
//            BillboardSprite     = 10,
        };
        const unsigned int ALL_PRIMITIVE_TYPES = Polygon + 1;
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
//                case BillboardSprite:   return o << "BillboardSprite";
                default:                return o << "UNKNOWN PrimitiveType"
                                                 << int(type);
            }
        }


        // Enumeration of different ways of handling of normal vectors
        enum NormalType {
            NoNormal    = 0,
            Flat        = 1,
            Smooth      = 2,
        };
        const unsigned int ALL_NORMAL_TYPES = Smooth + 1;
        inline ostream & operator<<(ostream &o, NormalType type) {
            switch (type) {
                case NoNormal:      return o << "NoNormal";
                case Flat:          return o << "Flat";
                case Smooth:        return o << "Smooth";
                default:            return o << "UNKNOWN NormalType"
                                             << int(type);
            }
        }


        // Enumeration of different sorts of texture coordinates
        enum TextureType {
            NoTexture   = 0,
            Texture1D   = 1,
            Texture2D   = 2,
            Texture3D   = 3,
        };
        const unsigned int ALL_TEXTURE_TYPES = Texture3D + 1;
        inline ostream & operator<<(ostream &o, TextureType type) {
            switch (type) {
                case NoTexture:     return o << "NoTexture";
                case Texture1D:     return o << "Texture1D";
                case Texture2D:     return o << "Texture2D";
                case Texture3D:     return o << "Texture3D";
                default:            return o << "UNKNOWN TextureType"
                                             << int(type);
            }
        }


        // Enumeration of the different rendering passes
        enum RenderPass {
            // "Standard" render passes
            Opacity      = 0,   // Render only opaque parts
//            Wireframe    = 1,   // Render as wireframe
            Transparency = 2,   // Render only transparent parts

            // Special render passes
            Selection    = 3,   // Render for selection
        };
        const unsigned int STD_RENDER_PASSES = Transparency + 1;
        const unsigned int ALL_RENDER_PASSES = Selection + 1;
        inline ostream & operator<<(ostream &o, RenderPass pass) {
            switch (pass) {
                case Opacity:       return o << "Opacity";
                case Transparency:  return o << "Transparency";
//                case Wireframe:     return o << "Wireframe";
                case Selection:     return o << "Selection";
                default:            return o << "UNKNOWN RenderPass "
                                             << int(pass);
            }
        }


        // Enumeration of the different ways to shade objects
        enum ShadingModel {
            SmoothShade = 0,    // Smooth shade objects
            FlatShade   = 1,    // Flat shade objects
            Paint       = 2,    // Draw objects w/o lighting
        };
        const unsigned int ALL_SHADING_MODELS = Paint + 1;
        inline ostream & operator<<(ostream &o, ShadingModel model) {
            switch (model) {
                case SmoothShade:   return o << "SmoothShade";
                case FlatShade:     return o << "FlatShade";
                case Paint:         return o << "Paint";
                default:            return o << "UNKNOWN ShadingModel "
                                             << int(model);
            }
        }


        // Enumeration of the different ways to calculate fog
        enum FogModel {
            Linear             = 0,     // fog = (end - z) / (end - start)
            Exponential        = 1,     // fog = e^-(density * z)
            ExponentialSquared = 2,     // fog = e^-(density * z)^2
        };
        const unsigned int ALL_FOG_MODELS = ExponentialSquared + 1;
        inline ostream & operator<<(ostream &o, FogModel model) {
            switch (model) {
                case Linear:                return o << "Linear";
                case Exponential:           return o << "Exponential";
                case ExponentialSquared:    return o << "Exponential Squared";
                default:                    return o << "UNKNOWN FogModel "
                                                     << int(model);
            }
        }


        // Enumeration of different ways to fill polygons
        enum FillMode {
            Filled      = 0,
            Outlined    = 1,
        };
        const unsigned ALL_FILL_MODES = Outlined + 1;
        inline ostream & operator<<(ostream &o, FillMode mode) {
            switch (mode) {
                case Filled:    return o << "Filled";
                case Outlined:  return o << "Outlined";
                default:            return o << "UNKNOWN FillMode "
                                             << int(mode);
            }
        }


        // Enumeration of different ways to classify polygon faces
        enum PolygonFace {
            Front   = 0,
            Back    = 1,
        };
        const unsigned ALL_POLYGON_FACES = Back + 1;
        inline ostream & operator<<(ostream &o, PolygonFace face) {
            switch (face) {
                case Front: return o << "Front";
                case Back:  return o << "Back";
                default:    return o << "UNKNOWN PolygonFace "
                                             << int(face);
            }
        }
    };
};

#endif
