/** -*- C++ -*-
 *
 * \file    Widget.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Widget class defines the interface that all lightweight
 *      (non-UI-toolkit-rendered) components must implement. Widgets must be
 *      descendents of a RenderableSurface in order to be displayed.
 */

#pragma once
#ifndef INCA_UI_WIDGET
#define INCA_UI_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Widget;

        // Pointer typedefs
        typedef shared_ptr<Widget>       WidgetPtr;
        typedef shared_ptr<Widget const> WidgetConstPtr;
    };
};

// Import superclass definitions
#include "../Component.hpp"

// Include enclosing heavyweight component
#include "../RenderableSurface.hpp"


class inca::ui::Widget : public inca::ui::Component,
                         public inca::ui::ComponentAdapter,
                         public inca::ui::KeyAdapter,
                         public inca::ui::PointerAdapter,
                         public inca::ui::ButtonAdapter {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    typedef RenderableSurface::Renderer Renderer;
    
    
/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
protected:
    // Non-public constructor with component name
    explicit Widget(const std::string & nm = std::string());

        
/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
public:
    // Implement this function to render the Widget
    virtual void render() const = 0;
    

/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface and other Widgets
 *---------------------------------------------------------------------------*/
protected:
    // The RenderableSurface is allowed to set itself on this object
    friend class RenderableSurface;

    // Claim another Widget as being a child of this one, setting the parent()
    // reference, copying the surface() reference, and calling construct()
    void adopt(WidgetPtr w);
    
    // Stop considering a Widget to be a child, setting the parent() and
    // surface() references to NULL
    void disown(WidgetPtr w);
    
    // Access to the RenderableSurface that holds this Widget
    RenderableSurfacePtr      surface();
    RenderableSurfaceConstPtr surface() const;
    virtual void setSurface(RenderableSurfacePtr s);

    // Access to the Renderer instance for the RenderableSurface.
    // This function throws an IllegalStateException if the Widget is not
    // currently attached to a RenderableSurface.
    Renderer & renderer() const;
    
    // Request that the owning RenderableSurface invoke the render() function
    // on me at its earliest convenience.
    // This function throws an IllegalStateException if the Widget is not
    // currently attached to a RenderableSurface.
    void requestRedisplay() const;
    
    RenderableSurfacePtr _surface;  // The parent RenderableSurface
};

#endif
