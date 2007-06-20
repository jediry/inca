/** -*- C++ -*-
 *
 * \file    RenderableSurface.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The RenderableSurface class is a heavyweight component containing a
 *      (hopefully) hardware-accelerated 3D-renderable framebuffer. A
 *      RenderableSurface may be used in either of two ways. First, it may be
 *      subclassed (the subclass should override at least the render() function,
 *      and possibly some of the event handler functions). Second, it may have
 *      a Widget assigned to it, in which case the widget will receive all
 *      render() calls and events.
 */

#pragma once
#ifndef INCA_UI_RENDERABLE_SURFACE
#define INCA_UI_RENDERABLE_SURFACE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class RenderableSurface;
        class RenderableSurfacePeer;

        class Widget;   // This is here to get around a circular dependency

        // Pointer typedefs
        typedef shared_ptr<RenderableSurface>       RenderableSurfacePtr;
        typedef shared_ptr<RenderableSurface const> RenderableSurfaceConstPtr;
    }
}


// HACK import renderer definition
#include <inca/rendering.hpp>

// Import superclass definitions
#include "HeavyweightComponent"


class inca::ui::RenderableSurface
    : public inca::ui::HeavyweightComponent<inca::ui::RenderableSurfacePeer>,
      public inca::ui::ComponentAdapter,
      public inca::ui::KeyAdapter,
      public inca::ui::PointerAdapter,
      public inca::ui::ButtonAdapter {
/*---------------------------------------------------------------------------*
 | Type declarations
 *---------------------------------------------------------------------------*/
public:
    // XXX HORRIBLY HACKED!!! Help me, Obi-Wan Kenobi!!!
    typedef rendering::ImmediateModeRenderer<rendering::OpenGLTraits,
                                             rendering::ExclusiveAccessCachePolicy> Renderer;

    typedef shared_ptr<Widget>       WidgetPtr;
    typedef shared_ptr<Widget const> WidgetConstPtr;
    

/*---------------------------------------------------------------------------*
 | Native UI toolkit peer
 *---------------------------------------------------------------------------*/
public:
    friend class RenderableSurfacePeer;

    // Widgets need access to the rendering functions
    friend class Widget;


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit RenderableSurface(const std::string & nm = std::string());

    // Second-phase initialization
    void construct();


/*---------------------------------------------------------------------------*
 | Overrides from Component/ComponentAdapter (HACKED for GLUT??)
 *---------------------------------------------------------------------------*/
public:
    void setParent(ComponentPtr p);
    void componentResized(const ComponentEvent & e);


/*---------------------------------------------------------------------------*
 | Embedded Widget accessors
 *---------------------------------------------------------------------------*/
public:
    WidgetPtr      widget();
    WidgetConstPtr widget() const;
    void setWidget(WidgetPtr w);
    
protected:
    WidgetPtr _widget;


/*---------------------------------------------------------------------------*
 | Rendering functions
 *---------------------------------------------------------------------------*/
public:
    // Render the embedded Widget(s) (called by the peer class)
    void render() const;

protected:
    // Access to the Renderer for this surface
    Renderer & renderer() const;

    // Request that the surface be re-rendered as soon as possible. This
    // function is called by the embedded Widget(s) whenever they wish to be
    // redrawn.
    void requestRedisplay() const;
};


class inca::ui::RenderableSurfacePeer
    : public inca::ui::ComponentPeer<inca::ui::RenderableSurface> {
/*---------------------------------------------------------------------------*
 | Type declarations
 *---------------------------------------------------------------------------*/
public:
    typedef RenderableSurface::Renderer Renderer;
    

/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit RenderableSurfacePeer(RenderableSurface * s) : Base(s) { }


/*---------------------------------------------------------------------------*
 | Peer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Object lifecycle management
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual bool valid() const = 0;

    // Rendering
    virtual Renderer & renderer() const = 0;
    virtual void requestRedisplay() const = 0;
};  


// Now that RenderableSurface is fully defined, we can include the definition
// for Widget, if we haven't already
#include "widgets/Widget.hpp"      

#endif
