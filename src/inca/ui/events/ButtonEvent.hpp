/** -*- C++ -*-
 *
 * \file    ButtonEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The ButtonEvent class represents a button press or release event, such
 *      as from the system's pointer device, a joystick, or an auxillary button
 *      device. The types of ButtonEvents that can be generated are:
 *          ButtonPressed       the user pushed down the button
 *          ButtonReleased      the user let go of the button
 *          ButtonClicked       the user pressed/released the button quickly
 *
 *      The ButtonListener class is the corresponding listener interface, and
 *      should be implemented by any class wishing to receive ButtonEvents.
 *
 *      The ButtonAdapter class provides default (no-op) implementations of
 *      the ButtonListener interface functions.
 */

#pragma once
#ifndef INCA_UI_BUTTON_EVENT
#define INCA_UI_BUTTON_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class ButtonEvent;
        class ButtonListener;
        class ButtonAdapter;

        // Pointer typedefs
        typedef shared_ptr<ButtonListener>       ButtonListenerPtr;
        typedef shared_ptr<ButtonListener const> ButtonListenerConstPtr;

    };
};

// Import superclass definition
#include "InputEvent.hpp"


class inca::ui::ButtonEvent : public inca::ui::InputEvent {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Event types that can be indicated by this object
    enum EventType {
        ButtonPressed,  // A button was pressed
        ButtonReleased, // A button was released
        ButtonClicked,  // A button was pressed & released quickly
    };
    

/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit ButtonEvent(Timestamp timestamp, ControlFlags flags, Pixel coords,
                         EventType type, ButtonCode code)
        : InputEvent(timestamp, flags, coords),
          _type(type), _buttonCode(code) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    EventType  type()       const { return _type; }
    ButtonCode buttonCode() const { return _buttonCode; }
    
protected:
    EventType  _type;
    ButtonCode _buttonCode;
};


class inca::ui::ButtonListener {
public:
    virtual void buttonPressed(const ButtonEvent & e)  = 0;
    virtual void buttonReleased(const ButtonEvent & e) = 0;
    virtual void buttonClicked(const ButtonEvent & e)  = 0;
};


class inca::ui::ButtonAdapter : public inca::ui::ButtonListener {
public:
    void buttonPressed(const ButtonEvent & e) { }
    void buttonReleased(const ButtonEvent & e) { }
    void buttonClicked(const ButtonEvent & e) { }
};

#endif
