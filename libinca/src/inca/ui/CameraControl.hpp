/*
 * File: CameraControl.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The CameraControl class provides simple, interface-level functions
 *      for manipulating a Camera. It also contains default key/mouse
 *      bindings for calling these functions, which may be overridden in
 *      a subclass.
 */

#ifndef INCA_INTERFACE_GENERIC_CAMERA_CONTROL
#define INCA_INTERFACE_GENERIC_CAMERA_CONTROL

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class CameraControl;
    };
};

// Import superclass definitions
#include "Framework.hpp"

// Import Camera & Transform definitions
#include <inca/world/Cameras.hpp>


class inca::ui::CameraControl : virtual public Control {
public:
    // Convenience typedefs
    typedef CameraControl ThisType;

    // Math typedefs
    typedef world::Camera                   Camera;
    typedef world::CameraPtr                CameraPtr;
    typedef world::Transform                Transform;
    typedef world::TransformPtr             TransformPtr;
    typedef world::Transform::scalar_t      scalar_t;
    typedef world::Transform::scalar_arg_t  scalar_arg_t;


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    CameraControl(CameraPtr c);

    rw_property(scalar_t, panScale, 1.0);
    rw_property(scalar_t, dollyScale, 1.0);
    rw_property(scalar_t, lookScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, rollScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, pitchScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, yawScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, zoomScale, 1.01);
    rw_ptr_property(Camera, camera, /* */);


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
public:
    void zoomCamera(int clicks);
    void panCamera(int dx, int dy);
    void dollyCamera(int dz);
    void lookCamera(int ay, int ap);
    void rollCamera(int ar);
    void pitchCamera(int ap);
    void yawCamera(int ay);


/*---------------------------------------------------------------------------*
 | Event-handling functions
 *---------------------------------------------------------------------------*/
public:
    void mouseDragged(index_t x, index_t y);
    void buttonPressed(MouseButton button, index_t x, index_t y);

protected:
    index_t mouseX, mouseY;    // Last recorded mouse coordinates
};

#endif
