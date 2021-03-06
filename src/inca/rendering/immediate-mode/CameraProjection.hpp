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

#pragma once
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
        geometry_t f = inca::math::cot(geometry_t(camera.vertViewAngle) * geometry_t(0.5));
        geometry_t zNear = geometry_t(camera.nearClip),
                   zFar  = geometry_t(camera.farClip),
                   zDiff = zNear - zFar;
        m(0,0) = f / geometry_t(camera.aspectRatio);
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
        geometry_t xDiff = geometry_t(camera.viewWidth),
                   yDiff = geometry_t(camera.viewHeight),
                   zFar  = geometry_t(camera.farClip),
                   zNear = geometry_t(camera.nearClip),
                   zDiff = zFar - zNear;
        m(0,0) = 2/xDiff;
        m(1,1) = 2/yDiff;
        m(2,2) = -2/zDiff;      m(2,3) = (zFar + zNear) / -zDiff;
        m(3,3) = geometry_t(1);
        renderer.projectionMatrix().load(m);
    }
};

#endif
