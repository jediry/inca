/*
 * File: OpenGLTraits.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      rendering classes and functions.
 */

#ifndef INCA_RENDERING_IMMEDIATE_MODE_OPENGL_TRAITS
#define INCA_RENDERING_IMMEDIATE_MODE_OPENGL_TRAITS

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca rendering subsystem
namespace inca {
    namespace rendering {
        // Forward declarations
        class OpenGLTraits;
    };
};

// Import Inca scalar, color, and linear-algebra types
#include <inca/math/scalar.hpp>
#include <inca/math/color.hpp>
#include <inca/math/linalg.hpp>


class inca::rendering::OpenGLTraits {
public:
    typedef float geometry_t;   // Geometry/matrix precision
    typedef float color_t;      // Color component precision
    typedef float tex_coord_t;  // Texture coordinate precision
    typedef float timer_t;      // Timer precision

    typedef math::Point<geometry_t, 2>                      Point2D;
    typedef math::Vector<geometry_t, 2>                     Vector2D;
    typedef math::Point<geometry_t, 3>                      Point3D;
    typedef math::Vector<geometry_t, 3>                     Vector3D;
    typedef math::Point<geometry_t, 4>                      Point4D;
    typedef math::Vector<geometry_t, 4>                     Vector4D;
    typedef math::Quaternion<geometry_t>                    Quaternion;
    typedef math::Matrix<geometry_t, 4, 4, true, false>     Matrix;
    typedef math::Color<color_t, math::sRGB<true> >         Color;
    typedef math::Point<int, 2>                             Pixel;
    typedef math::Vector<int, 2>                            Dimension;
    typedef math::Vector<geometry_t, 3>                     Normal;
    typedef math::Point<geometry_t, 2>                      TexCoord;

    // Enumeration of properties for use with get/setHardwareState
    enum Property {
        DepthBuffering, AlphaBlending, FaceCulling, Lighting,
        PointSmoothing, LineSmoothing, PolygonSmoothing,
        BackgroundColor,
        CurrentShadingModel, CurrentColor, CurrentNormal, CurrentTexCoord,
        CurrentEdgeFlag,
    };


protected:
    template <Property property, typename Type>
    void getHardwareState(Type & var);

    template <Property property, typename Type>
    void setHardwareState(const Type & var);
};

#endif
