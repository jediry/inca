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
#include "Control.hpp"


class inca::ui::PassThruControl : virtual public Control,
                                  virtual public WidgetPartContainer {
private:
    // Set this class up to contain properties
    PROPERTY_OWNING_OBJECT(PassThruControl);


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor
    PassThruControl() : control(this) { }

    // Constructor with initialization of Control
    PassThruControl(ControlPtr c)
        : control(this, c) { }

    // Constructor with initialization of component name
    PassThruControl(const string &nm)
        : control(this) { name = nm; }

    // Constructor with Control and component name
    PassThruControl(ControlPtr c, const string &nm)
        : control(this, c) { name = nm; }

    // The Control that we're wrapping
    rw_ptr_property_custom_set(Control, control, NULL);

    // Custom setter for the "control" property
    void ptr_property_set(Control, control) {
        releaseWidgetPart(_control);    // Let the old Control go
        _control = value;               // Set the new Control
        acquireWidgetPart(_control);    // Tell him we got 'im
    }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer function to propagate redisplay requests
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartPtr w) { requestRedisplay(); }


/*---------------------------------------------------------------------------*
 | Control-related events (passed-thru to the wrapped Control, if there is one)
 *---------------------------------------------------------------------------*/
public:
    void keyPressed(KeyCode k, Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->keyPressed(k, p);
        }
    }
    void mouseTracked(Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->mouseTracked(p);
        }
    }
    void mouseDragged(Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->mouseDragged(p);
        }
    }
    void buttonPressed(MouseButton b, Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonPressed(b, p);
        }
    }
    void buttonReleased(MouseButton b, Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonReleased(b, p);
        }
    }
    void buttonClicked(MouseButton b, Point p) {
        if (control) {
            control->setControlFlags(getControlFlags());    // Pass in my flags
            control->buttonClicked(b, p);
        }
    }
};

#endif
