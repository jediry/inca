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
        // Rendering API marker classes
        class OpenGL { };   // Use the system's OpenGL drivers
//        class Direct3D { }  // Use Microsoft Direct3D API

        // Convenient aliases for known renderer configurations
        typedef ImmediateModeRenderer<OpenGL>   OpenGLRenderer;
//        typedef ImmediateModeRenderer<Direct3D> Direct3DRenderer;
    };
};

#endif

