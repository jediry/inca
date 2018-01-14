/** -*- C++ -*-
 *
 * \file    PointerEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PointEvent class represents a movement of the system's pointer
 *      device (i.e., mouse, trackball, tablet, etc.). The types of
 *      PointerEvents that can be generated are:
 *          PointerDragged  the user moved the pointer with a button held down
 *          PointerTracked  the user moved the pointer without a button held down
 *          PointerEntered  the user moved the pointer into a component
 *          PointerExited   the user moved the pointer out of a component
 *
 *      The PointerListener class is the corresponding listener interface, and
 *      should be implemented by any class wishing to receive PointerEvents.
 *
 *      The PointerAdapter class provides default (no-op) implementations of
 *      the PointerListener interface functions.
 */

#pragma once
#ifndef INCA_UI_POINTER_EVENT
#define INCA_UI_POINTER_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class PointerEvent;
        class PointerListener;
        class PointerAdapter;

        // Pointer typedefs
        typedef shared_ptr<PointerListener>       PointerListenerPtr;
        typedef shared_ptr<PointerListener const> PointerListenerConstPtr;
    };
};

// Import superclass definition
#include "InputEvent.hpp"


class inca::ui::PointerEvent : public inca::ui::InputEvent {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Event types that can be indicated by this object
    enum EventType {
        PointerDragged, // The pointer was moved with a button held down
        PointerTracked, // The pointer was moved without a button held down
        PointerEntered, // The pointer entered a component bounds
        PointerExited,  // The pointer left a component's bounds
    };
    

/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit PointerEvent(Timestamp timestamp, ControlFlags flags, Pixel coords,
                          EventType type)
        : InputEvent(timestamp, flags, coords), _type(type) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    EventType  type()       const { return _type; }
    
protected:
    EventType  _type;
};


class inca::ui::PointerListener {
public:
    virtual void pointerDragged(const PointerEvent & e) = 0;
    virtual void pointerTracked(const PointerEvent & e) = 0;
    virtual void pointerEntered(const PointerEvent & e) = 0;
    virtual void pointerExited(const PointerEvent & e)  = 0;
};


class inca::ui::PointerAdapter : public inca::ui::PointerListener {
public:
    void pointerDragged(const PointerEvent & e) { }
    void pointerTracked(const PointerEvent & e) { }
    void pointerEntered(const PointerEvent & e) { }
    void pointerExited(const PointerEvent & e) { }
};

#endif
