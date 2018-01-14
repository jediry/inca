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

#pragma once
#ifndef INCA_RENDERING
#define INCA_RENDERING

// Import Renderer supertype definition
#include "rendering/Renderer.hpp"

// Import (usually) hardware-accelerated renderer definition and cache policies
#include "rendering/immediate-mode/ImmediateModeRenderer"
#include "rendering/immediate-mode/ExclusiveAccessCachePolicy"
#include "rendering/immediate-mode/OpenGLTraits.hpp"

// Import Raytracer Renderer
//#include "raytracing/RaytracingRenderer.hpp"

// Declare known specializations of the renderer types
namespace inca {
    namespace rendering {
        // Convenient aliases for known renderer configurations
        typedef ImmediateModeRenderer<OpenGLTraits, ExclusiveAccessCachePolicy>
            OpenGLRenderer;
    };
};

#endif

