/*
 * File: CameraControl.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The CameraControl class provides simple, interface-level functions
 *      for manipulating a Camera. It also contains default key/mouse
 *      bindings for calling these functions, which may be overridden in
 *      a subclass.
 */

#ifndef INCA_UI_CAMERA_CONTROL
#define INCA_UI_CAMERA_CONTROL

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class CameraControl;

        // Pointer typedefs
        typedef shared_ptr<CameraControl>       CameraControlPtr;
        typedef shared_ptr<CameraControl const> CameraControlConstPtr;
    };
};

// Import superclass definition
#include "PassThruControl.hpp"

// Import Camera & Transform definitions
#include <inca/world/Cameras.hpp>


class inca::ui::CameraControl : public PassThruControl {
private:
    // Set up this class to own properties
    PROPERTY_OWNING_OBJECT(CameraControl);

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
    // Default constructor with optional component name
    CameraControl(const string &nm = "");

    // Constructor initializing camera
    CameraControl(CameraPtr c, const string &nm = "");

    // The camera we're controlling
    rw_ptr_property(Camera, camera, NULL);

    // Transformation scale factors
    rw_property(scalar_t, panScale, 1.0);
    rw_property(scalar_t, dollyScale, 1.0);
    rw_property(scalar_t, lookScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, rollScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, pitchScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, yawScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, zoomScale, 1.01);

    // Transformation enabled toggles
    rw_property(bool, enablePan,   true);
    rw_property(bool, enableDolly, true);
    rw_property(bool, enableLook,  true);
    rw_property(bool, enableRoll,  true);
    rw_property(bool, enablePitch, true);
    rw_property(bool, enableYaw,   true);
    rw_property(bool, enableZoom,  true);


/*---------------------------------------------------------------------------*
 | Event-handling functions
 *---------------------------------------------------------------------------*/
public:
    void mouseDragged(Point p);
    void buttonPressed(MouseButton button, Point p);

protected:
    Point mousePosition;    // Last recorded mouse coordinates


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
public:
    void panCamera(int dx, int dy);
    void dollyCamera(int dz);
    void lookCamera(int ay, int ap);
    void rollCamera(int ar);
    void pitchCamera(int ap);
    void yawCamera(int ay);
    void zoomCamera(int clicks);
};

#endif
