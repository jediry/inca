/*
 * File: PassThruControl.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PassThruControl class is a concrete Control that simply wraps
 *      another Control, and passes events through to the wrapped Control.
 *      This makes a convenient base class for Controls that intercept
 *      certain events (e.g., the Ctrl-Q sequence for quitting the application)
 *      and pass through the rest of them (mouse clicks, other key presses).
 */

#ifndef INCA_UI_PASS_THRU_CONTROL
#define INCA_UI_PASS_THRU_CONTROL

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class PassThruControl;

        // Pointer typedefs
        typedef shared_ptr<PassThruControl>       PassThruControlPtr;
        typedef shared_ptr<PassThruControl const> PassThruControlConstPtr;
    };
};

// Import superclass definition
#include "BasicControl.hpp"


class inca::ui::PassThruControl : public BasicControl,
                                  virtual public WidgetPartContainer {
private:
    // Set this class up to contain properties
    PROPERTY_OWNING_OBJECT(PassThruControl);


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional component name
    explicit PassThruControl(const std::string & nm = "")
        : control(this) { name = nm; }

    // Constructor with initialization of Control
    explicit PassThruControl(ControlPtr c, const std::string & nm = "")
        : control(this, c) { name = nm; }

    // The Control that we're wrapping
    rw_ptr_property_custom_set(Control, control, NULL);

    // Custom setter for the "control" property
    void ptr_property_set(Control, control) {
        releaseWidgetPart(_control);    // Let the old Control go
        _control = value;               // Set the new Control
        acquireWidgetPart(_control);    // Tell him we got 'im

        // Tell the wrapped Control about its context
        if (_control) {
            _control->setRenderer(this->_renderer);
        }
    }

    // XXX HACK!
    void setRenderer(RendererPtr r) {
        BasicControl::setRenderer(r);
        if (_control)
            _control->setRenderer(r);
    }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer functions
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartConstPtr w) const { requestRedisplay(); }

    // My sub-widget has same dimensions as me
    Dimension getSize(WidgetPartConstPtr wp) const { return size; }

    // Ask my parent who the big daddy Window is
    WindowPtr getContainingWindow() const {
        return WidgetPart::getContainingWindow();
    }


/*---------------------------------------------------------------------------*
 | Control-related events (passed-thru to the wrapped Control, if there is one)
 *---------------------------------------------------------------------------*/
public:
    void keyPressed(KeyCode k, Pixel p) {
        BasicControl::keyPressed(k, p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->keyPressed(k, p);
        }
    }
    void mouseTracked(Pixel p) {
        BasicControl::mouseTracked(p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->mouseTracked(p);
        }
    }
    void mouseDragged(Pixel p) {
        BasicControl::mouseDragged(p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->mouseDragged(p);
        }
    }
    void buttonPressed(MouseButton b, Pixel p) {
        BasicControl::buttonPressed(b, p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonPressed(b, p);
        }
    }
    void buttonReleased(MouseButton b, Pixel p) {
        BasicControl::buttonReleased(b, p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonReleased(b, p);
        }
    }
    void buttonClicked(MouseButton b, Pixel p) {
        BasicControl::buttonClicked(b, p);
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonClicked(b, p);
        }
    }
};

#endif
