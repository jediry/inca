/*
 * File: CameraNavigationWidget.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The CameraNavigationWidget class provides simple, interface-level
 *      functions for manipulating a Camera. It also contains default key/mouse
 *      bindings for calling these functions, which may be overridden in
 *      a subclass.
 *
 *      FIXME: This ought to be a single class, in the Inca library, with
 *      remapable controls.
 *
 *      This currently handles panning in a way that tracks the mouse. How
 *      could we do it so that we can also transform according to some
 *      world-space parameter (e.g., max speed)?
 */

#ifndef INCA_UI_CAMERA_NAVIGATION_WIDGET
#define INCA_UI_CAMERA_NAVIGATION_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca user-interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class CameraNavigationWidget;

        // Pointer typedefs
        typedef shared_ptr<CameraNavigationWidget>       CameraNavigationWidgetPtr;
        typedef shared_ptr<CameraNavigationWidget const> CameraNavigationWidgetConstPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"

// Import Camera & Transform definitions
#include <inca/world/Cameras.hpp>


class inca::ui::CameraNavigationWidget : public PassThruWidget {
private:
    // Set up this class to own properties
    PROPERTY_OWNING_OBJECT(CameraNavigationWidget);

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
    explicit CameraNavigationWidget(const std::string & nm = "");

    // Constructor initializing camera
    explicit CameraNavigationWidget(CameraPtr c, const std::string & nm = "");

    // The camera we're controlling
    rw_ptr_property(Camera, camera, NULL);

    // Transformation scale factors
    rw_property(scalar_t, lateralMotionScale, 1.0);
    rw_property(scalar_t, longitudinalMotionScale, 1.0);
    rw_property(scalar_t, verticalMotionScale, 1.0);
    rw_property(scalar_t, panScale, 1.0);
    rw_property(scalar_t, dollyScale, 1.0);
    rw_property(scalar_t, lookScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, rollScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, pitchScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, yawScale, math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, zoomScale, 1.01);

    // Transformation enabled toggles
    rw_property(bool, enableLongitudinalMotion, true);
    rw_property(bool, enableLateralMotion,      true);
    rw_property(bool, enableVerticalMotion,     true);
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
    void resizeView(Dimension d);
    void renderView();

    void keyPressed(KeyCode keycode, Pixel p);
    void mouseTracked(Pixel p);
    void mouseDragged(Pixel p);
    void buttonPressed(MouseButton button, Pixel p);
    void buttonReleased(MouseButton button, Pixel p);
    void buttonClicked(MouseButton b, Pixel p);

protected:
    void pushMatrices();
    void popMatrices();

    Pixel mousePosition;    // Last recorded mouse coordinates
    bool transforming;


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
public:
    void moveLongitudinally(int clicks);
    void moveLaterally(int clicks);
    void moveVertically(int clicks);
    void panCamera(int dx, int dy);
    void dollyCamera(int dz);
    void lookCamera(int ay, int ap);
    void rollCamera(int ar);
    void pitchCamera(int ap);
    void yawCamera(int ay);
    void zoomCamera(int clicks);
};

#endif
