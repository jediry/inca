/*
 * File: CameraControl.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the generic camera control declared in
 *      CameraControl.hpp.
 */

// Import class definition
#include "CameraControl.hpp"
using namespace inca;
using namespace inca::ui;


// Default constructor with optional component name
CameraControl::CameraControl(const string &nm)
    : PassThruControl(nm), camera(this), 
      panScale(this), dollyScale(this), lookScale(this), rollScale(this),
      pitchScale(this), yawScale(this), zoomScale(this),
      enablePan(this), enableDolly(this), enableLook(this), enableRoll(this),
      enablePitch(this), enableYaw(this), enableZoom(this) { }

// Constructor initializing camera
CameraControl::CameraControl(CameraPtr c, const string &nm)
    : PassThruControl(nm), camera(this, c), 
      panScale(this), dollyScale(this), lookScale(this), rollScale(this),
      pitchScale(this), yawScale(this), zoomScale(this),
      enablePan(this), enableDolly(this), enableLook(this), enableRoll(this),
      enablePitch(this), enableYaw(this), enableZoom(this) { }

void CameraControl::panCamera(int dx, int dy) {
    if (camera && enablePan) {
        camera->transform->pan(-dx * panScale.get(), -dy * panScale.get());
        requestRedisplay();
    }
}

void CameraControl::dollyCamera(int dz) {
    if (camera && enableDolly) {
        camera->transform->dolly(dz * dollyScale.get());
        requestRedisplay();
    }
}

void CameraControl::lookCamera(int ay, int ap) {
    if (camera && enableLook) {
        camera->transform->look(-ay * lookScale.get(), -ap * lookScale.get());
        requestRedisplay();
    }
}

void CameraControl::rollCamera(int ar) {
    if (camera && enableRoll) {
        camera->transform->roll(ar * rollScale.get());
        requestRedisplay();
    }
}

void CameraControl::pitchCamera(int ap) {
    if (camera && enablePitch) {
        camera->transform->pitch(ap * pitchScale.get());
        requestRedisplay();
    }
}

void CameraControl::yawCamera(int ay) {
    if (camera && enableYaw) {
        camera->transform->yaw(ay * yawScale.get());
        requestRedisplay();
    }
}

void CameraControl::zoomCamera(int clicks) {
    if (camera && enableZoom) {
        if (clicks > 0)
            for (int i = 0; i < clicks; i++)
                camera->zoom(zoomScale);
        else if (clicks < 0)
            for (int i = 0; i > clicks; i--)
                camera->zoom(1.0 / zoomScale);
        requestRedisplay();
    }
}

void CameraControl::mouseDragged(Point p) {
    // Figure out how far we've moved (flipping Y)
    int dx = p[0] - mousePosition[0],
        dy = mousePosition[1] - p[1];

    if (theseFlagsActive(AltModifier | LeftButton))
        lookCamera(dx, dy);
    else if (theseFlagsActive(ControlModifier | AltModifier | LeftButton))
        dollyCamera(dx + dy);
    else if (theseFlagsActive(AltModifier | MiddleButton))
        panCamera(dx, dy);
    else if (theseFlagsActive(AltModifier | RightButton))
        rollCamera(dx + dy);
    else if (theseFlagsActive(WheelUp))
        zoomCamera(1);
    else if (theseFlagsActive(WheelDown))
        zoomCamera(-1);

    // Update our saved coordinates
    mousePosition = p;
}

void CameraControl::buttonPressed(MouseButton button, Point p) {
    // Store these as the current mouse coordinates
    mousePosition = p;
}

