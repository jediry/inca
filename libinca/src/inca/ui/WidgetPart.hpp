/*
 * File: WidgetPart.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_WIDGET_PART
#define INCA_UI_WIDGET_PART

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class WidgetPart;
        class WidgetPartContainer;

        // Pointer typedefs
        typedef shared_ptr<WidgetPart>       WidgetPartPtr;
        typedef shared_ptr<WidgetPart const> WidgetPartConstPtr;
        typedef shared_ptr<WidgetPartContainer>       WidgetPartContainerPtr;
        typedef shared_ptr<WidgetPartContainer const> WidgetPartContainerConstPtr;
        typedef weak_ptr<WidgetPartContainer> WidgetPartContainerWeakPtr;


        // Super-forward declaration of Window stuff, so that we can avoid an
        // ugly circular dependency
        class Window;
        typedef shared_ptr<Window>          WindowPtr;
        typedef shared_ptr<Window const>    WindowConstPtr;
    };
};

// Import superclass definition
#include "UIComponent.hpp"


/**
 * The WidgetPart class handles the details of being contained by one or more
 * Widgets. It maintains a set of weak_ptrs to the Widget instances that
 * currently contain it, and passes along redraw requests to each Widget that
 * is still alive at the time of the request.
 */
class inca::ui::WidgetPart : virtual public UIComponent {
private:
public:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(WidgetPart);


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
protected:
    // Protected constructor -- only allow subclasses
    explicit WidgetPart() { }


/*---------------------------------------------------------------------------*
 | GUI event handlers and fire-ers
 *---------------------------------------------------------------------------*/
public:
    // Event-handler functions for reacting to how the container is
    // treating this WidgetPart.
    virtual void acquired(WidgetPartContainerPtr wpc);
    virtual void released();
    virtual void activated();
    virtual void suspended();

    // This tells our container that we've changed and need displayin'
    void requestRedisplay() const;

    // Ask our parent who the ultimate containing Window is
    WindowPtr getContainingWindow() const;

    // Who is my daddy?
    WidgetPartContainerPtr parent() const;

    // Are we active within our parent?
    bool active() const;

    // HACK Access to our renderer object
    Renderer & renderer();
    const Renderer & renderer() const;

    // HACK Renderer accessor functions
    virtual void setRenderer(RendererPtr r);

protected:
    // The renderer used by this part
    RendererPtr _renderer;

    // Weak pointer to the WidgetPartContainer who owns me
    WidgetPartContainerWeakPtr _parent;

    // Whether or not I am "active" w/r to my parent
    bool _active;
};


class inca::ui::WidgetPartContainer : virtual public UIComponent {
private:
public:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(WidgetPartContainer);

protected:
    // Non-public default constructor
    explicit WidgetPartContainer() { }

    // Non-public constructor with component name
    explicit WidgetPartContainer(const std::string & nm) { name = nm; }

    // This is called before the WidgetPartContainer is first displayed, but
    // after all constructors have been run. Since there may be chicken-and-egg
    // type problems with using shared_ptrs to the WPC inside the constructor,
    // the simplest way around this is to defer construction of objects
    // requiring a shared_ptr to 'this' until this function.
    // FIXME: this is not called for all WPCs...currently just GLUTWindow...
    virtual void construct() { }

    // WidgetPart event-firing functions for notifying WidgetParts when and how
    // we are using them.
    void acquireWidgetPart(WidgetPartPtr wp)  { if (wp) wp->acquired(self()); }
    void releaseWidgetPart(WidgetPartPtr wp)  { if (wp) wp->released(); }
    void activateWidgetPart(WidgetPartPtr wp) { if (wp) wp->activated(); }
    void suspendWidgetPart(WidgetPartPtr wp)  { if (wp) wp->suspended(); }

public:
    // This is called by the contained WidgetParts when they want to be
    // redisplayed by us. Subclasses must implement this.
    virtual void redisplay(WidgetPartConstPtr wp) const = 0;

    // This is called by the WidgetParts in order to figure out the
    // dimensions of the display area allotted to them.
    virtual Dimension getSize(WidgetPartConstPtr wp) const = 0;

    // This is called by the WidgetParts in order to get access to the Window
    // object that ultimately contains them. Subclasses must implement this.
    virtual WindowPtr getContainingWindow() const = 0;
};

#endif
