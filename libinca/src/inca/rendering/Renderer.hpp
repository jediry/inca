/*
 * File: Renderers.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Renderer class is an abstract superclass, defining the functionality
 *      common to classes that convert a 3D scene model into a 2D image.
 */

#pragma once
#ifndef INCA_RENDERING_RENDERER
#define INCA_RENDERING_RENDERER

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca rendering subsystem
namespace inca {
    namespace rendering {
        // Forward declarations
        class Renderer;

        // Pointer declarations
        typedef shared_ptr<Renderer> RendererPtr;
    };
};


// Import Inca::World object model
#include <inca/world.hpp>


class inca::rendering::Renderer {
private:
    // Convenience typedefs
    typedef Renderer ThisType;

public:
    // Constructor
    Renderer() : width(this), height(this) { }

    // Basic properties needed by a renderer
    rw_property(SizeType, width, 640);
    rw_property(SizeType, height, 480);

    // Subclasses should (re)implement these functions, which are (hopefully)
    // only called in a valid rendering context
    virtual void initialize() { }
    virtual void resize(SizeType w, SizeType h) {
        width = w;
        height = h;
    }
    virtual void render() = 0;
};

#endif

