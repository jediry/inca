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
using namespace inca::ui;


// Constructor
CameraControl::CameraControl(CameraPtr c) : camera(this, c), 
    panScale(this), dollyScale(this), lookScale(this), rollScale(this),
    pitchScale(this), yawScale(this), zoomScale(this) { }

void CameraControl::zoomCamera(int clicks) {
    if (camera != NULL) {
        if (clicks > 0)
            for (int i = 0; i < clicks; i++)
                camera->zoom(zoomScale);
        else if (clicks < 0)
            for (int i = 0; i > clicks; i--)
                camera->zoom(1.0 / zoomScale);
        requestRedisplay();
    }
}

void CameraControl::panCamera(int dx, int dy) {
    if (camera != NULL) {
        camera->transform->pan(-dx * panScale.get(), -dy * panScale.get());
        requestRedisplay();
    }
}

void CameraControl::dollyCamera(int dz) {
    if (camera != NULL) {
        camera->transform->dolly(dz * dollyScale.get());
        requestRedisplay();
    }
}

void CameraControl::lookCamera(int ay, int ap) {
    if (camera != NULL) {
        camera->transform->look(-ay * lookScale.get(), -ap * lookScale.get());
        requestRedisplay();
    }
}

void CameraControl::rollCamera(int ar) {
    if (camera != NULL) {
        camera->transform->roll(ar * rollScale.get());
        requestRedisplay();
    }
}

void CameraControl::pitchCamera(int ap) {
    if (camera != NULL) {
        camera->transform->pitch(ap * pitchScale.get());
        requestRedisplay();
    }
}

void CameraControl::yawCamera(int ay) {
    if (camera != NULL) {
        camera->transform->yaw(ay * yawScale.get());
        requestRedisplay();
    }
}

void CameraControl::mouseDragged(index_t x, index_t y) {
    // Figure out how far we've moved (flipping Y)
    int dx = x - mouseX,
        dy = mouseY - y;

    if (controlFlags == (ALT_KEY | LEFT_BUTTON))
        lookCamera(dx, dy);
    else if (controlFlags == (CTRL_KEY | ALT_KEY | LEFT_BUTTON))
        dollyCamera(dx + dy);
    else if (controlFlags == (ALT_KEY | MIDDLE_BUTTON))
        panCamera(dx, dy);
    else if (controlFlags == (ALT_KEY | RIGHT_BUTTON))
        rollCamera(dx + dy);
    else if (controlFlags == WHEEL_UP)
        zoomCamera(1);
    else if (controlFlags == WHEEL_DOWN)
        zoomCamera(-1);

    // Update our saved coordinates
    mouseX = x;
    mouseY = y;
}

void CameraControl::buttonPressed(MouseButton button, index_t x, index_t y) {
    // Store these as the current mouse coordinates
    mouseX = x;
    mouseY = y;
}
