/*
 * File: CameraProjection.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      FIXME: This class needs a different name. This is sorta just a hack until I figure out the "right" way to do this.
 */

#ifndef INCA_RENDERING_IMMEDIATE_MODE_CAMERA_PROJECTION
#define INCA_RENDERING_IMMEDIATE_MODE_CAMERA_PROJECTION

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca rendering subsystem
namespace inca {
    namespace rendering {
        // Forward declarations
        class CameraProjection;
    };
};

// Import camera definitions
#include <inca/world/Cameras.hpp>


class inca::rendering::CameraProjection {
public:
    template <typename Renderer>
    void operator()(Renderer & renderer, const world::Camera & camera) const {
        const world::Camera * cam = &camera;
        if (dynamic_cast<const world::PerspectiveCamera *>(cam) != NULL)
            (*this)(renderer, static_cast<const world::PerspectiveCamera &>(camera));
        else if (dynamic_cast<const world::OrthographicCamera *>(cam) != NULL)
            (*this)(renderer, static_cast<const world::OrthographicCamera &>(camera));
        else
            INCA_ERROR("Ack! Unrecognized camera type " << typeid(camera).name())
    }


    // Render function call operator
    template <typename Renderer>
    void operator()(Renderer & renderer, const world::PerspectiveCamera & camera) const {
        typedef typename Renderer::geometry_t geometry_t;
        typename Renderer::Matrix m(0);
        geometry_t f = inca::math::cot(camera.vertViewAngle() * geometry_t(0.5));
        geometry_t zNear = camera.nearClip(),
                   zFar  = camera.farClip(),
                   zDiff = zNear - zFar;
        m(0,0) = f / camera.aspectRatio();
        m(1,1) = f;
        m(2,2) = (zFar + zNear) / zDiff;
        m(2,3) = (2 * zFar * zNear) / zDiff;
        m(3,2) = geometry_t(-1);
        renderer.projectionMatrix().load(m);
    }

    // Render function call operator
    template <typename Renderer>
    void operator()(Renderer & renderer, const world::OrthographicCamera & camera) const {
        typedef typename Renderer::geometry_t geometry_t;
        typename Renderer::Matrix m(0);
        geometry_t xDiff = camera.viewWidth(),
                   yDiff = camera.viewHeight(),
                   zDiff = camera.farClip() - camera.nearClip();
        m(0,0) = xDiff;     m(0,3) = xDiff;
        m(1,1) = yDiff;     m(1,3) = yDiff;
        m(2,2) = zDiff;     m(2,3) = zDiff;
        m(3,3) = geometry_t(1);
        renderer.projectionMatrix().load(m);
    }
};

#endif
