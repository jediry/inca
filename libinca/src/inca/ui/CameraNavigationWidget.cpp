/*
 * File: CameraNavigationWidget.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
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
using namespace inca::math;


// Default constructor with optional component name
CameraNavigationWidget::CameraNavigationWidget(const std::string & nm)
    : PassThruWidget(nm), camera(this),
      lateralMotionScale(this), longitudinalMotionScale(this), verticalMotionScale(this),
      panScale(this), dollyScale(this), lookScale(this), zoomScale(this),
      yawScale(this), pitchScale(this), rollScale(this),
      enableLongitudinalMotion(this), enableLateralMotion(this), enableVerticalMotion(this),
      enablePan(this), enableDolly(this), enableLook(this), enableRoll(this),
      enablePitch(this), enableYaw(this), enableZoom(this) { }

// Constructor initializing camera
CameraNavigationWidget::CameraNavigationWidget(CameraPtr c,
                                               const std::string & nm)
    : PassThruWidget(nm), camera(this, c),
      lateralMotionScale(this), longitudinalMotionScale(this), verticalMotionScale(this),
      panScale(this), dollyScale(this), lookScale(this), zoomScale(this),
      yawScale(this), pitchScale(this), rollScale(this),
      enableLongitudinalMotion(this), enableLateralMotion(this), enableVerticalMotion(this),
      enablePan(this), enableDolly(this), enableLook(this), enableRoll(this),
      enablePitch(this), enableYaw(this), enableZoom(this) { }


// Widget event handlers
void CameraNavigationWidget::resizeView(Dimension sz) {
    if (sz != size) {   // Don't do anything if we're already the right size
        camera->reshape(sz[0], sz[1]);      // Reshape the camera
        PassThruWidget::resizeView(sz);     // Do superclass impl.
    }

    // Call superclass implementation (pass on to sub-widget)
    PassThruWidget::resizeView(sz);
}

void CameraNavigationWidget::renderView() {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        // Call superclass implementation (pass on to sub-widget)
        PassThruWidget::renderView();

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::keyPressed(KeyCode keycode, Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        switch (keycode) {
        case KeyA:
            moveLaterally(-1);
            requestRedisplay();
            break;
        case KeyD:
            moveLaterally(1);
            requestRedisplay();
            break;
        case KeyS:
            moveLongitudinally(-1);
            requestRedisplay();
            break;
        case KeyW:
            moveLongitudinally(1);
            requestRedisplay();
            break;
        case KeyE:
            moveVertically(1);
            requestRedisplay();
            break;
        case KeyQ:
            moveVertically(-1);
            requestRedisplay();
            break;
        default:
            // This event doesn't concern us, so just pass it along
            PassThruWidget::keyPressed(keycode, p);
        }

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::mouseTracked(Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        // This event doesn't concern us, so just pass it along
        PassThruWidget::mouseTracked(p);

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::mouseDragged(Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        bool transforming = false;  // Set to true if we're doin' stuff

        // Figure out how far we've moved (flipping Y)
        int dx = p[0] - mousePosition[0],
            dy = mousePosition[1] - p[1];

        // Update our saved coordinates
        mousePosition = p;

        // If this event belongs to us, Autobots TRANSFORM!!!
        if (theseFlagsActive(ControlModifier | LeftButton)) {
            dollyCamera(dx + dy);
            transforming = true;
        } else if (theseFlagsActive(ControlModifier | AltModifier | RightButton)) {
            zoomCamera(dx + dy);
            transforming = true;
        } else if (theseFlagsActive(ControlModifier | AltModifier | LeftButton)) {
            lookCamera(dx, dy);
            transforming = true;
        } else if (theseFlagsActive(ControlModifier | MiddleButton)) {
            panCamera(dx, dy);
            transforming = true;
        } else if (theseFlagsActive(ControlModifier | RightButton)) {
            rollCamera(dx + dy);
            transforming = true;
        }

        // If this event didn't concern us, just pass it along
        if (! transforming)
            PassThruWidget::mouseDragged(p);

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::buttonPressed(MouseButton button, Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        // Store these as the current mouse coordinates
        mousePosition = p;

        // Let the superclass do its thing
        PassThruWidget::buttonPressed(button, p);

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::buttonReleased(MouseButton button, Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        // Store these as the current mouse coordinates
        mousePosition = p;

        // Let the superclass do its thing
        PassThruWidget::buttonReleased(button, p);

    // Clean up the transformation stack
    popMatrices();
}

void CameraNavigationWidget::buttonClicked(MouseButton button, Pixel p) {
    // Set up the camera transformation on behalf of the sub-widgets
    pushMatrices();

        // Let the superclass do its thing
        PassThruWidget::buttonClicked(button, p);

    // Clean up the transformation stack
    popMatrices();
}


/*---------------------------------------------------------------------------*
 | High-level camera controls (values correspond to pixels)
 *---------------------------------------------------------------------------*/
void CameraNavigationWidget::pushMatrices() {
    // Save the prior projection matrix and setup a new one
    renderer().projectionMatrix().push();     // Save prior state
    CameraProjection cp;
    cp(renderer(), *camera);

    // Save the prior transformation matrix and setup a new one
    Renderer::MatrixStack & viewMat = _renderer->viewMatrix();
    viewMat.push();
    viewMat.reset();
    viewMat.untransform(*camera->transform());
}

void CameraNavigationWidget::popMatrices() {
    renderer().projectionMatrix().pop();
    renderer().viewMatrix().pop();
}

void CameraNavigationWidget::moveLongitudinally(int clicks) {
    if (camera && enableLongitudinalMotion) {
        camera->transform->moveLongitudinally(clicks * longitudinalMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::moveVertically(int clicks) {
    if (camera && enableVerticalMotion) {
        camera->transform->moveVertically(clicks * verticalMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::moveLaterally(int clicks) {
    if (camera && enableLateralMotion) {
        camera->transform->moveLaterally(clicks * lateralMotionScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::panCamera(int dx, int dy) {
//    INCA_DEBUG("Pan " << dx << ", " << dy << " clicks")

    typedef Renderer::Point3D Point3D;      // Shortcuts
    typedef Renderer::geometry_t scalar_t;

    scalar_t hPan, vPan;    // Amount to translate by

    if (camera && enablePan) {
        if (typeid(*camera) == typeid(inca::world::OrthographicCamera)) {
            // Unproject 3 points one pixel apart onto the image plane (z = 1.0)
            Point3D pixel(size[0] / 2, size[1] / 2, 1.0);
            Point3D local = renderer().screenToLocal(pixel);
            local /= local[2];
            Point3D po = renderer().localToWorld(local);
            ++(pixel[0]);
            local = renderer().screenToLocal(pixel);
            local /= local[2];
            Point3D px = renderer().localToWorld(local);
            --(pixel[0]); ++(pixel[1]);
            local = renderer().screenToLocal(pixel);
            local /= local[2];
            Point3D py = renderer().localToWorld(local);

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
            hPan = -dx * panScale;
            vPan = -dy * panScale;
        }

        // Translate the camera
        camera->transform->pan(hPan, vPan);

        // ...and redraw.
        requestRedisplay();
    }
}

void CameraNavigationWidget::dollyCamera(int dz) {
//    INCA_DEBUG("Dolly " << dz << " clicks")
    if (camera && enableDolly) {
        camera->transform->dolly(dz * dollyScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::lookCamera(int ay, int ap) {
//    INCA_DEBUG("Look " << ay << ", " << ap << " clicks")
    if (camera && enableLook) {
        camera->transform->pitch(-ap * lookScale());
        // XXX HACK
        camera->transform->rotateZ(-ay * lookScale());
//        camera->transform->look(-ay * lookScale(), -ap * lookScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::rollCamera(int ar) {
//    INCA_DEBUG("Roll " << ar << " clicks")
    if (camera && enableRoll) {
        camera->transform->roll(ar * rollScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::pitchCamera(int ap) {
//    INCA_DEBUG("Pitch " << ap << " clicks")
    if (camera && enablePitch) {
        camera->transform->pitch(ap * pitchScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::yawCamera(int ay) {
//    INCA_DEBUG("Yaw " << ay << " clicks")
    if (camera && enableYaw) {
        camera->transform->yaw(ay * yawScale());
        requestRedisplay();
    }
}

void CameraNavigationWidget::zoomCamera(int clicks) {
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
