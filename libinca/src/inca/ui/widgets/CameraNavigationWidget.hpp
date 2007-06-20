/** -*- C++ -*-
 *
 * \file    CameraNavigationWidget.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The CameraNavigationWidget class provides simple, interface-level
 *      functions for manipulating a Camera. It also contains default key/mouse
 *      bindings for calling these functions, which may be overridden in
 *      a subclass.
 *
 *      Except for the particular events that control the camera, all input
 *      events (pointer, keyboard, button) are forwarded to the widget chained
 *      after this one (if any), just as in the superclass (PassThruWidget),
 *      with the additional effect that the camera's transformation and
 *      projection are applied before doing so. The transformation/projection
 *      are NOT applied for non-input events (e.g., ComponentEvents).
 *
 *      This currently handles panning in a way that tracks the mouse. How
 *      could we do it so that we can also transform according to some
 *      world-space parameter (e.g., max speed)?
 */

#pragma once
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

// Import augmented enumeration mechanism
#include <inca/util/Enumeration.hpp>


class inca::ui::CameraNavigationWidget : public inca::ui::PassThruWidget {
/*---------------------------------------------------------------------------*
 | Type declarations
 *---------------------------------------------------------------------------*/
private:
    // Set up this class to own properties
    PROPERTY_OWNING_OBJECT(CameraNavigationWidget);

public:
    // Math typedefs
    typedef world::Camera                   Camera;
    typedef world::CameraPtr                CameraPtr;
    typedef world::Transform                Transform;
    typedef world::TransformPtr             TransformPtr;
    typedef world::Transform::scalar_t      scalar_t;
    typedef world::Transform::scalar_arg_t  scalar_arg_t;
    
    // Enumeration of the different ways we can track the pointer
    INCA_ENUM(PointerFollowMode,
                (Ignore,               // Don't react
                (MoveLongitudinally,   // Move along the longitudinal axis
                (MoveLaterally,        // Move along the lateral axis
                (MoveVertically,       // Move along the vertical axis
                (Pan,                  // Move laterally & vertically
                (Roll,                 // Rotate around the longitudinal axis
                (Pitch,                // Rotate around the lateral axis
                (Yaw,                  // Rotate around the vertical axis
                (Look,                 // Rotate laterally & vertically
                (Zoom,                 // Adjust the camera FOV
                 INCA_NIL)))))))))))


/*---------------------------------------------------------------------------*
 | Constructors & properties
 *---------------------------------------------------------------------------*/
public:
    // Constructor with optional component name
    explicit CameraNavigationWidget(const std::string & nm = std::string());

    // Constructor initializing camera, with optional component name
    explicit CameraNavigationWidget(CameraPtr c,
                                    const std::string & nm = std::string());

    // The camera we're controlling
    rw_ptr_property(Camera, camera, NULL);
    
    // Transformation mode toggles
    rw_property(bool, invertVerticalAxis,   false);
    rw_property(bool, invertHorizontalAxis, false);
    
    // Transformation scale factors
    rw_property(scalar_t, lateralMotionScale,       1.0);
    rw_property(scalar_t, longitudinalMotionScale,  1.0);
    rw_property(scalar_t, verticalMotionScale,      1.0);
    rw_property(scalar_t, panScale,                 1.0);
    rw_property(scalar_t, rollScale,    math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, pitchScale,   math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, yawScale,     math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, lookScale,    math::PI<scalar_t>() / 64.0);
    rw_property(scalar_t, zoomScale,    1.01);

    // Transformation enabled toggles
    rw_property(bool, enableLongitudinalMotion, true);
    rw_property(bool, enableLateralMotion,      true);
    rw_property(bool, enableVerticalMotion,     true);
    rw_property(bool, enablePan,                true);
    rw_property(bool, enableRoll,               true);
    rw_property(bool, enablePitch,              true);
    rw_property(bool, enableYaw,                true);
    rw_property(bool, enableLook,               true);
    rw_property(bool, enableZoom,               true);


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
public:
    void moveLongitudinally(int dt);
    void moveLaterally(int dt);
    void moveVertically(int dt);
    void pan(int dx, int dy);
    void roll(int ar);
    void pitch(int ap);
    void yaw(int ay);
    void look(int ay, int ap);
    void zoom(int clicks);


/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
public:
    // Render the sub widget, as viewed through the camera
    void render() const;
    
    // Reshape the camera to maintain the correct projection aspect ratio
    void setSize(Dimension d);
    void setSize(SizeType w, SizeType h);

    // Key events
    void keyPressed(const KeyEvent & e);
    void keyReleased(const KeyEvent & e);
    void keyTyped(const KeyEvent & e);

    // Pointer events
    void pointerTracked(const PointerEvent & e);
    void pointerDragged(const PointerEvent & e);
    void pointerEntered(const PointerEvent & e);
    void pointerExited(const PointerEvent & e);

    // Button events
    void buttonPressed(const ButtonEvent & e);
    void buttonReleased(const ButtonEvent & e);
    void buttonClicked(const ButtonEvent & e);

protected:
    void _pushMatrices() const;     // Transform to the camera's view
    void _popMatrices() const;      // Restore the previous transform state

    // How far has the pointer moved?
    Pixel _pointerMotionDifferential(Pixel px);
    
    // What pointer following mode should we be in?
    PointerFollowMode _selectFollowMode(InputEvent::ControlFlags f);
    
    // Transform the camera according to the current follow-mode
    void _transformCamera(Pixel dp);

    PointerFollowMode   _followMode;        // How do we follow the pointer?
    bool                _followModeLocked;  // Are we locked into the current mode?
    Pixel       _pointerPosition;   // Where was the pointer last?
};

#endif
