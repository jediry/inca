/** -*- C++ -*-
 *
 * \file    WindowEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The WindowEvent class represents a change to a window state, such as
 *      being iconified, maximized, or closed. The types of WindowEvents that 
 *      can be generated are:
 *          WindowOpened        the window was opened (shown)
 *          WindowClosed        the window was closed (hidden)
 *
 *      The WindowListener class is the corresponding listener interface, and
 *      should be implemented by any class wishing to receive WindowEvents.
 *
 *      The WindowAdapter class provides default (no-op) implementations of
 *      the WindowListener interface functions.
 */

#pragma once
#ifndef INCA_UI_WINDOW_EVENT
#define INCA_UI_WINDOW_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class WindowEvent;
        class WindowListener;
        class WindowAdapter;

        // Pointer typedefs
        typedef shared_ptr<WindowListener>       WindowListenerPtr;
        typedef shared_ptr<WindowListener const> WindowListenerConstPtr;

    };
};

// Import superclass definition
#include "Event.hpp"

// Import Window class definition
#include "Window.hpp"


class inca::ui::WindowEvent : public inca::ui::Event {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Event types that can be indicated by this object
    enum EventType {
        WindowOpened,   // The window was opened
        WindowClosed,   // The window was closed
    };
    

/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit WindowEvent(Timestamp timestamp, EventType type, WindowPtr w)
        : Event(timestamp), _type(type), _window(w) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    EventType type()   const { return _type; }
    WindowPtr window() const { return _window; } 
    
protected:
    EventType  _type;
    WindowPtr  _window;
};


class inca::ui::WindowListener {
public:
    virtual void windowOpened(const WindowEvent & e) = 0;
    virtual void windowClosed(const WindowEvent & e) = 0;
};


class inca::ui::WindowAdapter : public inca::ui::WindowListener {
public:
    void windowOpened(const WindowEvent & e) { }
    void windowClosed(const WindowEvent & e) { }
};

#endif
