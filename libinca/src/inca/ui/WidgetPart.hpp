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
    };
};

// Import superclass definition
#include "UIComponent.hpp"

// Import collection of weakly-held shared pointers
#include <inca/util/weak_ptr_set>

// Import hash_map definition
#include <inca/util/hash_container>


/**
 * The WidgetPart class handles the details of being contained by one or more
 * Widgets. It maintains a set of weak_ptrs to the Widget instances that
 * currently contain it, and passes along redraw requests to each Widget that
 * is still alive at the time of the request.
 */
class inca::ui::WidgetPart : virtual public UIComponent {
public:
    // Container typedefs
    typedef weak_ptr_set<WidgetPartContainer>     WidgetPartContainerSet;
    typedef hash_map<WidgetPartContainer *, bool> WidgetPartContainerBoolMap;

private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(WidgetPart);

protected:
    // Protected constructor -- only allow subclasses
    explicit WidgetPart() { }


/*---------------------------------------------------------------------------*
 | GUI event handlers and fire-ers
 *---------------------------------------------------------------------------*/
public:
    // Event-handler functions for reacting to how the containers are
    // treating this WidgetPart.
    void acquiredBy(WidgetPartContainerPtr w);
    void releasedBy(WidgetPartContainerPtr w);
    void suspendedBy(WidgetPartContainerPtr w);
    void resumedBy(WidgetPartContainerPtr w);

    // This tells each container that we've changed and need displayin'
    void requestRedisplay() const;

protected:
    // Weak pointers to the Widgets who own me
    mutable WidgetPartContainerSet containers;// XXX This shouldn't have to be mutable!!

    // Whether I am an active WidgetPart in each Widget who owns me
    mutable WidgetPartContainerBoolMap isActiveIn;
};


class inca::ui::WidgetPartContainer : virtual public UIComponent {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(WidgetPartContainer);

protected:
    // Non-public default constructor
    explicit WidgetPartContainer() { }

    // Non-public constructor with component name
    explicit WidgetPartContainer(const string &nm) { name = nm; }

    // WidgetPart event-firing functions for notifying WidgetParts when and how
    // we are using them.
    void acquireWidgetPart(WidgetPartPtr wp) { if (wp) wp->acquiredBy(self()); }
    void releaseWidgetPart(WidgetPartPtr wp) { if (wp) wp->releasedBy(self()); }
    void suspendWidgetPart(WidgetPartPtr wp) { if (wp) wp->suspendedBy(self()); }
    void resumeWidgetPart(WidgetPartPtr wp)  { if (wp) wp->releasedBy(self()); }


public:
    // This is called by the contained WidgetParts when they want to be
    // redisplayed by us. Subclasses must implement this.
    virtual void redisplay(WidgetPartConstPtr wp) const = 0;
    
    // This is called by the WidgetParts in order to figure out the
    // dimensions of the display area allotted to them.
    virtual Dimension getSize(WidgetPartConstPtr wp) const = 0;
};

#endif
