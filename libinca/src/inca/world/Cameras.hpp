/*
 * File: Cameras.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Camera class represents a view on a scene. A Camera's parameters
 *      affect how the 3D scene will be converted into a 2D image. The actual
 *      rasterization of the image is performed by the rendering subsystem.
 */

#ifndef INCA_WORLD_CAMERAS
#define INCA_WORLD_CAMERAS

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Camera;
        class PerspectiveCamera;
        class OrthographicCamera;

        // Pointer typedefs
        typedef shared_ptr<Camera> CameraPtr;
        typedef shared_ptr<PerspectiveCamera> PerspectiveCameraPtr;
        typedef shared_ptr<OrthographicCamera> OrthographicCameraPtr;
    };
};

// Import the superclass definition
#include "WorldObject.hpp"

// Import the Transform definition
#include "Transform.hpp"


class inca::world::Camera : public WorldObject {
private:
    // Convenience typedefs
    typedef Camera ThisType;

public:
    // Import typedefs for the scalar model we're using for geometry
    typedef double  scalar_t;
    typedef double  scalar_arg_t;


    // Constructor
    Camera() : transform(this), nearClip(this),
               farClip(this), aspectRatio(this) { }

    // Properties common to all camera types
    rw_ptr_property(Transform, transform, new Transform());
    rw_property(scalar_t, nearClip, 1.0);
    rw_property(scalar_t, farClip, 1000.0);
    ro_property(scalar_t, aspectRatio, 1.0);

    // Common polymorphic viewing functions
    virtual void reshape(scalar_arg_t horiz, scalar_arg_t vert) = 0;
    virtual void zoom(scalar_arg_t factor) = 0;
};


class inca::world::PerspectiveCamera : public Camera {
private:
    // Convenience typedefs
    typedef PerspectiveCamera ThisType;

public:
    // Constructor
    PerspectiveCamera() : aperture(this), focalLength(this),
                          horizViewAngle(this), vertViewAngle(this) { }

    // Properties specific to perspective cameras
    rw_property(scalar_t, aperture, 0.0);
    rw_property(scalar_t, focalLength, 1.0);
    rw_property_custom_set(scalar_t, horizViewAngle, math::PI<scalar_t>() / 4.0);
        void property_set(scalar_t, horizViewAngle) {
            _horizViewAngle = value;
            _aspectRatio = horizViewAngle / vertViewAngle;
        }
    rw_property_custom_set(scalar_t, vertViewAngle, math::PI<scalar_t>() / 4.0);
        void property_set(scalar_t, vertViewAngle) {
            _vertViewAngle = value;
            _aspectRatio = horizViewAngle / vertViewAngle;
        }

    // Perspective reshape
    void reshape(scalar_arg_t horiz, scalar_arg_t vert) {
        _aspectRatio = horiz / vert;
        _vertViewAngle = _horizViewAngle / _aspectRatio;
    }

    // Perspective zoom
    void zoom(scalar_arg_t factor) {
        _horizViewAngle *= factor;
        _vertViewAngle *= factor;
    }
};

class inca::world::OrthographicCamera : public Camera {
private:
    // Convenience typedefs
    typedef OrthographicCamera ThisType;

public:
    // Constructor
    OrthographicCamera() : viewWidth(this), viewHeight(this) { }

    // Properties specific to orthographic cameras
    rw_property_custom_set(scalar_t, viewWidth, 1.0);
        void property_set(scalar_t, viewWidth) {
            _viewWidth = value;
            _aspectRatio = _viewWidth / _viewHeight;
        }
    rw_property_custom_set(scalar_t, viewHeight, 1.0);
        void property_set(scalar_t, viewHeight) {
            _viewHeight = value;
            _aspectRatio = _viewWidth / _viewHeight;
        }

    // Orthographic reshape
    void reshape(scalar_arg_t horiz, scalar_arg_t vert) {
        _aspectRatio = horiz / vert;
        _viewHeight = _viewWidth / _aspectRatio;
    }

    // Orthographic zoom
    void zoom(scalar_arg_t factor) {
        _viewWidth /= factor;
        _viewHeight /= factor;
    }
};

#endif

