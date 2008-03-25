/** -*- C++ -*-
 *
 * \file    CameraNavigationWidget.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the generic camera control declared in
 *      CameraNavigationWidget.hpp.
 */

// Import class definition
#include "CameraNavigationWidget.hpp"

// Import rendering utilities
#include <inca/rendering/immediate-mode/CameraProjection.hpp>

using namespace inca::rendering;
using namespace inca::world;
using namespace inca::ui;
using namespace inca;

// Distance from boundary at which we warp the pointer
#define BOUNDARY_WARP_DISTANCE 25

// HACK!!! Eliminate dependence on GLUT (pointer warping & hiding)
#define GL_HPP_IMPORT_GLUT 1
#include <inca/integration/opengl/GL.hpp>


/*---------------------------------------------------------------------------*
 | Constructors & properties
 *---------------------------------------------------------------------------*/
// Default constructor with optional component name
CameraNavigationWidget::CameraNavigationWidget(const std::string & nm)
    : PassThruWidget(nm), camera(this),
      _followMode(Ignore), _followModeLocked(false),
      invertHorizontalAxis(this),       invertVerticalAxis(this),
      longitudinalMotionScale(this),    enableLongitudinalMotion(this),
      lateralMotionScale(this),         enableLateralMotion(this),
      verticalMotionScale(this),        enableVerticalMotion(this),
      panScale(this),                   enablePan(this),
      rollScale(this),                  enableRoll(this),
      yawScale(this),                   enableYaw(this),
      pitchScale(this),                 enablePitch(this),
      lookScale(this),                  enableLook(this),
      zoomScale(this),                  enableZoom(this) { }

// Constructor initializing camera
CameraNavigationWidget::CameraNavigationWidget(CameraPtr c,
                                               const std::string & nm)
    : PassThruWidget(nm), camera(this, c),
      _followMode(Ignore), _followModeLocked(false),
      invertHorizontalAxis(this),       invertVerticalAxis(this),
      longitudinalMotionScale(this),    enableLongitudinalMotion(this),
      lateralMotionScale(this),         enableLateralMotion(this),
      verticalMotionScale(this),        enableVerticalMotion(this),
      panScale(this),                   enablePan(this),
      rollScale(this),                  enableRoll(this),
      yawScale(this),                   enableYaw(this),
      pitchScale(this),                 enablePitch(this),
      lookScale(this),                  enableLook(this),
      zoomScale(this),                  enableZoom(this) { }


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
void CameraNavigationWidget::moveLongitudinally(int dt) {
    if (camera && enableLongitudinalMotion) {
        camera->transform->moveLongitudinally(dt * longitudinalMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::moveVertically(int dt) {
    if (camera && enableVerticalMotion) {
        camera->transform->moveVertically(dt * verticalMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::moveLaterally(int dt) {
    if (camera && enableLateralMotion) {
        camera->transform->moveLaterally(dt * lateralMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::pan(int dx, int dy) {
    INCA_DEBUG("Pan " << dx << ", " << dy << " clicks")

    typedef Renderer::Point3D Point3D;      // Shortcuts
    typedef Renderer::geometry_t scalar_t;
    
    Renderer & r = renderer();

    scalar_t hPan, vPan;    // Amount to translate by

    if (camera && enablePan) {
        if (typeid(*camera) == typeid(inca::world::OrthographicCamera)) {
            // Unproject 3 points one pixel apart onto the image plane (z = 1.0)
            _pushMatrices();

            Point3D pixel(scalar_t(_size[0] / 2),
                          scalar_t(_size[1] / 2),
                          scalar_t(1));
            Point3D local = r.screenToLocal(pixel);
            local /= local[2];
            Point3D po = r.localToWorld(local);
            pixel[0] += 1;
            local = r.screenToLocal(pixel);
            local /= local[2];
            Point3D px = r.localToWorld(local);
            pixel[0] -= 1; pixel[1] += 1;
            local = r.screenToLocal(pixel);
            local /= local[2];
            Point3D py = r.localToWorld(local);

            _popMatrices();
            
    //        po = renderer().localToWorld(Point3D(0.0, 0.0, -1.0));
    //        INCA_DEBUG("Projecting adjacent pixels found distances X(" << distance(po,px) << ") and Y(" << distance(po,py) << ")")
//            INCA_DEBUG("po " << po)
//            INCA_DEBUG("px " << px)
//            INCA_DEBUG("py " << py)
//            INCA_DEBUG("Pan by " << (-dx * distance(po,px)) << ", " << (-dy * distance(po,py)))

            hPan = -dx * distance(po, px);
            vPan = -dy * distance(po, py);

        // Perspective (or other) camera
        } else {
            hPan = -dx * scalar_t(panScale);
            vPan = -dy * scalar_t(panScale);
        }

        // Translate the camera
        camera->transform->pan(hPan, vPan);

        // ...and redraw.
        requestRedisplay();
    }
}

void CameraNavigationWidget::roll(int ar) {
//    INCA_DEBUG("Roll " << ar << " clicks")
    if (camera && enableRoll) {
        camera->transform->roll(ar * rollScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::pitch(int ap) {
//    INCA_DEBUG("Pitch " << ap << " clicks")
    if (camera && enablePitch) {
        camera->transform->pitch(ap * pitchScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::yaw(int ay) {
//    INCA_DEBUG("Yaw " << ay << " clicks")
    if (camera && enableYaw) {
        camera->transform->yaw(ay * yawScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::look(int ay, int ap) {
//    INCA_DEBUG("Look " << ay << ", " << ap << " clicks")
    if (camera && enableLook) {
        camera->transform->pitch(-ap * lookScale());
        // XXX HACK
        camera->transform->rotateZ(-ay * lookScale());
//        camera->transform->look(-ay * lookScale(), -ap * lookScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::zoom(int clicks) {
//    INCA_DEBUG("Zoom " << clicks << " clicks")
    if (camera && enableZoom) {
        if (clicks > 0)
            for (int i = 0; i < clicks; i++)
                camera->zoom(zoomScale());
        else if (clicks < 0)
            for (int i = 0; i > clicks; i--)
                camera->zoom(1.0 / zoomScale());

        // ...and redraw
        requestRedisplay();
    }
}


/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
// Render the sub widget, as viewed through the camera
void CameraNavigationWidget::render() const {
    _pushMatrices();
    PassThruWidget::render();
    _popMatrices();
}


// Reshape the camera to maintain the correct projection aspect ratio
void CameraNavigationWidget::setSize(Dimension d) {
    camera->reshape(d[0], d[1]);    // Reshape the camera
    PassThruWidget::setSize(d);     // Pass it on
}
void CameraNavigationWidget::setSize(SizeType w, SizeType h) {
    setSize(Dimension(w, h));
}


// Forward any KeyEvents to the sub-widget, trapping navigation keys
void CameraNavigationWidget::keyPressed(const KeyEvent & e) {
    // Choose how big a move to make
    int steps = (e.modifierKeyActive(ControlModifierKey))   ? 10
              : (e.modifierKeyActive(ShiftModifierKey))     ? 5
              :                                               1;
    switch (e.keyCode()) {
    case KeyA:                          // Move left
        moveLaterally(-steps);
        requestRedisplay();
        break;
    case KeyD:                          // Move right
        moveLaterally(steps);
        requestRedisplay();
        break;
    case KeyS:                          // Move backward
        moveLongitudinally(-steps);
        requestRedisplay();
        break;
    case KeyW:                          // Move forward
        moveLongitudinally(steps);
        requestRedisplay();
        break;
    case KeyE:                          // Move up
        moveVertically(steps);
        requestRedisplay();
        break;
    case KeyQ:                          // Move down
        moveVertically(-steps);
        requestRedisplay();
        break;
    case KeyF12:                        // Lock us into look mode
        _followModeLocked = ! _followModeLocked;
        if (_followModeLocked) {
            _followMode = Look;
            GL::glutSetCursor(GLUT_CURSOR_NONE);
            _pointerPosition = e.coordinates();
        } else {
            _followMode = _selectFollowMode(e.controlFlags());
            GL::glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        break;
    default:
        // This event doesn't concern us, so just pass it along
        _pushMatrices();
        PassThruWidget::keyPressed(e);
        _popMatrices();
    }
}
void CameraNavigationWidget::keyReleased(const KeyEvent & e) {
    _pushMatrices();
    PassThruWidget::keyReleased(e);
    _popMatrices();
}
void CameraNavigationWidget::keyTyped(const KeyEvent & e) {
    _pushMatrices();
    PassThruWidget::keyTyped(e);
    _popMatrices();
}


// Forward PointerEvents to the sub-widget, trapping navigation gestures
void CameraNavigationWidget::pointerDragged(const PointerEvent & e) {
    // If we're following the pointer, Autobots TRANSFORM!!!
    if (_followMode != Ignore) {
        _transformCamera(_pointerMotionDifferential(e.coordinates()));
    
    // Otherwise, pass the event along to next widget
    } else {
        _pushMatrices();
        PassThruWidget::pointerDragged(e);
        _popMatrices();
    }
}
void CameraNavigationWidget::pointerTracked(const PointerEvent & e) {
    // If we're following the pointer, Autobots TRANSFORM!!!
    if (_followMode != Ignore) {
        _transformCamera(_pointerMotionDifferential(e.coordinates()));

    // Otherwise, pass the event along to next widget
    } else {
        _pushMatrices();
        PassThruWidget::pointerTracked(e);
        _popMatrices();
    }
}
void CameraNavigationWidget::pointerEntered(const PointerEvent & e) {
    _pushMatrices();
    PassThruWidget::pointerEntered(e);
    _popMatrices();
}
void CameraNavigationWidget::pointerExited(const PointerEvent & e) {
    _pushMatrices();
    PassThruWidget::pointerExited(e);
    _popMatrices();
}


// Forward ButtonEvents to the sub-widget
void CameraNavigationWidget::buttonPressed(const ButtonEvent & e) {
    // Figure out our new pointer-following mode
    _followMode = _selectFollowMode(e.controlFlags());
    GL::glutSetCursor((_followMode == Ignore) ? GLUT_CURSOR_LEFT_ARROW
                                              : GLUT_CURSOR_NONE);

    // If we're following the pointer in some manner, prepare to do this
    if (_followMode != Ignore) {
        _pointerPosition = e.coordinates();
        
    // Otherwise, pass the button press along to the next widget
    } else {
        _pushMatrices();
        PassThruWidget::buttonPressed(e);
        _popMatrices();
    }
}
void CameraNavigationWidget::buttonReleased(const ButtonEvent & e) {
    // If we're not currently following the pointer, pass the event along
    if (_followMode == Ignore) {
        _pushMatrices();
        PassThruWidget::buttonReleased(e);
        _popMatrices();
    }
    
    // This release event might have put us back into a pointer-follow mode
    _followMode = _selectFollowMode(e.controlFlags());
    GL::glutSetCursor((_followMode == Ignore) ? GLUT_CURSOR_LEFT_ARROW
                                              : GLUT_CURSOR_NONE);
}
void CameraNavigationWidget::buttonClicked(const ButtonEvent & e) {
    _pushMatrices();
    PassThruWidget::buttonClicked(e);
    _popMatrices();
}


// Save the current projection & view matrices, then apply those of the camera
void CameraNavigationWidget::_pushMatrices() const {
    Renderer & r = renderer();

    // Save the current projection matrix and set up the new one
    r.projectionMatrix().push();
    CameraProjection cp;
    cp(renderer(), *camera);

    // Save the current transformation matrix and set up the new one
    Renderer::MatrixStack & viewMat = r.viewMatrix();
    viewMat.push();
    viewMat.reset();
    viewMat.untransform(*camera->transform());
}

// Restore the pre-camera matrices
void CameraNavigationWidget::_popMatrices() const {
    Renderer & r = renderer();
    
    r.projectionMatrix().pop();
    r.viewMatrix().pop();
}

// Store the current pointer coordinates and return change since last call,
// and also warps the pointer back to the center if it gets too close to the edge
Component::Pixel CameraNavigationWidget::_pointerMotionDifferential(Pixel px) {
    // Figure out how far we've moved
    Pixel diff(px[0] - _pointerPosition[0], px[1] - _pointerPosition[1]);

    // HACK!!! This should be subsumed by some sort of peer input-mode logic
    // I'm sure DirectInput already handles this. Hmmm...
    // Warp the pointer if it's too close to the edge
    if (       px[0] < BOUNDARY_WARP_DISTANCE
            || px[1] < BOUNDARY_WARP_DISTANCE
            || px[0] > _size[0] - BOUNDARY_WARP_DISTANCE
            || px[1] > _size[1] - BOUNDARY_WARP_DISTANCE) {
        // Warp to center of Widget
        // TODO Implement Array ops and this can be much simpler
        //_pointerPosition = _size >> 1;
        _pointerPosition = Pixel(_size[0] >> 1, _size[1] >> 1);
        GL::glutWarpPointer(_pointerPosition[0], _pointerPosition[1]);
        
//        INCA_DEBUG("Warping pointer to " << _pointerPosition << ",  size is " << _size)
    // Otherwise, just store the new coordinates
    } else {
        _pointerPosition = px;
    }
    
    // Return the change from last time
    if (invertHorizontalAxis)   diff[0] = -diff[0];
    if (invertVerticalAxis)     diff[1] = -diff[1];
    return diff;
}

// Choose how to follow the pointer based on the current control flags
CameraNavigationWidget::PointerFollowMode
CameraNavigationWidget::_selectFollowMode(InputEvent::ControlFlags f) {
    // If the follow mode is locked, just return the current mode
    if (_followModeLocked)
        return _followMode;
        
    // Otherwise, choose based on the specified control flags
    else
        switch (f) {
            case ControlModifierKey | LeftButton:                   return MoveLongitudinally;
            case ControlModifierKey | AltModifierKey | RightButton: return Zoom;
            case ControlModifierKey | AltModifierKey | LeftButton:  return Look;
            case ControlModifierKey | MiddleButton:                 return Pan;
            case ControlModifierKey | RightButton:                  return Roll;
            case MiddleButton:                                      return Pan;
            case AltModifierKey | LeftButton:                       return Pan;
            default:                                                return Ignore;
        }
}    

// Perform the appropriate camera transformation
void CameraNavigationWidget::_transformCamera(Pixel dp) {
    switch (_followMode) {
    case MoveLongitudinally:
        moveLongitudinally(dp[0] + dp[1]);
        break;
    case MoveLaterally:
        moveLaterally(dp[0] + dp[1]);
        break;
    case MoveVertically:
        moveVertically(dp[0] + dp[1]);
        break;
    case Pan:
        pan(dp[0], dp[1]);
        break;
    case Roll:
        roll(dp[0] + dp[1]);
        break;
    case Pitch:
        pitch(dp[0] + dp[1]);
        break;
    case Yaw:
        yaw(dp[0] + dp[1]);
        break;
    case Look:
        look(dp[0], dp[1]);
        break;
    case Zoom:
        zoom(dp[0] + dp[1]);
        break;
    default:
        // Do nothing...
        break;
    }
}
