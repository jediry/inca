/*
 * File: GLViewControl.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The GLViewControl class implements event-handling and rendering
 *      pass-thrus to the basic functionality for a QT-based View/Control
 *      widget. It is intended to be multiple-inherited along with one each of
 *      the more specialized generic views & controls.
 */

#ifndef INCA_QT_GL_VIEW_CONTROL
#define INCA_QT_GL_VIEW_CONTROL

// Import superclass definitions
#include <ui/Views.hpp>
#include <ui/Control.hpp>

// Import QT OpenGL module
#include <qgl.h>

// This is part of the Inca interface layer
namespace Inca {
    namespace UI {
        // Forward declarations
        class QGLViewControl;
    };
};


class Inca::UI::QGLViewControl : public QGLWidget,
                                        virtual public Control,
                                        virtual public View {
/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Default initializer
    QGLViewControl(QWidget *parent = NULL, const char *name = NULL,
                   const QGLWidget * sharedWidget = NULL, WFlags f = 0)
        : QGLWidget(parent, name, sharedWidget, f) { }


/*---------------------------------------------------------------------------*
 | QT OpenGL functions
 *---------------------------------------------------------------------------*/
protected:
    void initializeGL() { initializeView(); }
    void resizeGL(int width, int height) { resizeView(width, height); }
    void paintGL() { renderView(); }

    void requestRedisplay() { updateGL(); } // Required by View


/*---------------------------------------------------------------------------*
 | QT Event-handling functions
 *---------------------------------------------------------------------------*/
protected:
    // Pass through a mouse button press
    void mousePressEvent(QMouseEvent *e) {
        setControlFlags(translateControlFlags(e->state()));
        buttonPressed(translateMouseButton(e->button()), e->x(), e->y());

        e->accept();    // We claim responsibility for this event
    }

    // Pass through a mouse button release
    void mouseReleaseEvent(QMouseEvent *e) {
        setControlFlags(translateControlFlags(e->state()));
        buttonReleased(translateMouseButton(e->button()), e->x(), e->y());

        e->accept();    // We claim responsibility for this event
    }

    // Pass through a mouse movement
    void mouseMoveEvent(QMouseEvent *e) {
        setControlFlags(translateControlFlags(e->state()));
        if (getButtonFlags() == NO_BUTTON)      // We're tracking, not dragging
            mouseTracked(e->x(), e->y());
        else
            mouseDragged(e->x(), e->y());

        e->accept();    // We claim responsibility for this event
    }

    // Translate a QT ButtonState into Inca ControlFlags
    MouseButton translateMouseButton(Qt::ButtonState button) {
        if (button & Qt::LeftButton)    return LEFT_BUTTON;
        if (button & Qt::MidButton)     return MIDDLE_BUTTON;
        if (button & Qt::RightButton)   return RIGHT_BUTTON;
        return NO_BUTTON;
    }

    ControlFlags translateControlFlags(Qt::ButtonState state) {
        ControlFlags flags = NO_MODIFIER | NO_BUTTON;
        if (state & Qt::ShiftButton)    flags |= SHIFT_KEY;
        if (state & Qt::ControlButton)  flags |= CTRL_KEY;
        if (state & Qt::AltButton)      flags |= ALT_KEY;
        if (state & Qt::LeftButton)     flags |= LEFT_BUTTON;
        if (state & Qt::MidButton)      flags |= MIDDLE_BUTTON;
        if (state & Qt::RightButton)    flags |= RIGHT_BUTTON;
        return flags;
    }
};

#endif
