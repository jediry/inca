/*
 * File: rendering.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      rendering classes and functions.
 */

#ifndef INCA_RENDERING
#define INCA_RENDERING

// Import Renderer supertype definition
#include "rendering/Renderer.hpp"

// Import (usually) hardware-accelerated renderer definition
#include "rendering/ImmediateModeRenderer"

// Import Raytracer Renderer
//#include "raytracing/RaytracingRenderer.hpp"

// Declare known specializations of the renderer types
namespace inca {
    namespace rendering {
        // Rendering API traits class for using the system's OpenGL driver
        struct OpenGLTraits {
            typedef float geometry_t;   // Geometry/matrix precision
            typedef float color_t;      // Color component precision
            typedef float tex_coord_t;  // Texture coordinate precision
            typedef float timer_t;      // Timer precision

            typedef math::Matrix<geometry_t, 4, 4, true, false>     Matrix;
            typedef math::Color<color_t, math::sRGBA>               Color;
            typedef math::Point<int, 2>                             Pixel;
            typedef math::Vector<int, 2>                            Dimension;

            // Number of internally cached properties
            static const int numBoolProperties   = 5;
            static const int numIntProperties    = 1;
            static const int numFloatProperties  = 3;
            static const int numDoubleProperties = 1;
            static const int numColorProperties  = 1;
        };

//        class Direct3D { }  // Use Microsoft Direct3D API

        // Convenient aliases for known renderer configurations
        typedef ImmediateModeRenderer<OpenGLTraits>   OpenGLRenderer;
//        typedef ImmediateModeRenderer<Direct3D> Direct3DRenderer;
    };
};

#endif

