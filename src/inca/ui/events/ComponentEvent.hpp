/** -*- C++ -*-
 *
 * \file    ComponentEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The ComponentEvent class represents a change to a component's state,
 *      for example, when it is moved, resized or hidden. The types of
 *      ComponentEvents that can be generated are:
 *          ComponentMoved      the component was moved
 *          ComponentResized    the component was resized
 *          ComponentHidden     the component was made invisible
 *          ComponentRevealed   the component was made visible
 *
 *      The ComponentListener class is the corresponding listener interface, and
 *      should be implemented by any class wishing to receive ComponentEvents.
 *
 *      The ComponentAdapter class provides default (no-op) implementations of
 *      the ComponentListener interface functions.
 */

#pragma once
#ifndef INCA_UI_COMPONENT_EVENT
#define INCA_UI_COMPONENT_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class ComponentEvent;
        class ComponentListener;
        class ComponentAdapter;

        // Pointer typedefs
        typedef shared_ptr<ComponentListener>       ComponentListenerPtr;
        typedef shared_ptr<ComponentListener const> ComponentListenerConstPtr;

    };
};

// Import superclass definition
#include "Event.hpp"

// Import Component class definition
#include "../Component.hpp"


class inca::ui::ComponentEvent : public inca::ui::Event {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Event types that can be indicated by this object
    enum EventType {
        ComponentMoved,     // The component was moved
        ComponentResized,   // The component was resized
        ComponentHidden,    // The component was made invisible
        ComponentRevealed,  // The component was made visible
    };
    

/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit ComponentEvent(Timestamp timestamp, EventType type, ComponentPtr c,
                            Pixel position, Dimension size, bool visible)
        : Event(timestamp), _type(type), _component(c),
          _position(position), _size(size), _visible(visible) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    EventType type() const { return _type; }
    ComponentPtr component() const { return _component; }
    Pixel position() const { return _position; }
    Dimension size() const { return _size; }
    bool visible()   const { return _visible; }
    
protected:
    EventType       _type;          // What sort of ComponentEvent is this?
    ComponentPtr    _component;     // Which component does it concern?
    Pixel           _position;      // Where is the component's new position?
    Dimension       _size;          // What is the component's new size?
    bool            _visible;       // What is the component's current visibilty?
};


class inca::ui::ComponentListener {
public:
    virtual void componentMoved(const ComponentEvent & e) = 0;
    virtual void componentResized(const ComponentEvent & e) = 0;
    virtual void componentHidden(const ComponentEvent & e) = 0;
    virtual void componentRevealed(const ComponentEvent & e) = 0;
};


class inca::ui::ComponentAdapter : public inca::ui::ComponentListener {
public:
    void componentMoved(const ComponentEvent & e) { }
    void componentResized(const ComponentEvent & e) { }
    void componentHidden(const ComponentEvent & e) { }
    void componentRevealed(const ComponentEvent & e) { }
};

#endif
