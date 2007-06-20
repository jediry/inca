/** -*- C++ -*-
 *
 * \file    RenderableSurface.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "RenderableSurface.hpp"
using namespace inca::ui;

typedef RenderableSurfacePeer   RSP;


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
RenderableSurface::RenderableSurface(const std::string & nm)
    : HeavyweightComponent<RenderableSurfacePeer>(nm) { }

void RenderableSurface::construct() {
    // Register to receive our own events      TODO: implement event filtering
    addComponentListener(self<ComponentListener>());
    addKeyListener(self<KeyListener>());
    addPointerListener(self<PointerListener>());
    addButtonListener(self<ButtonListener>());
    
    // Create the native object
    peer()->create();
}


/*---------------------------------------------------------------------------*
 | Overrides from Component/HeavyweightComponent
 *---------------------------------------------------------------------------*/
void RenderableSurface::setParent(ComponentPtr p) {
    if (parent())
        parent()->removeComponentListener(self<ComponentListener>());
    Component::setParent(p);
    if (parent())
        parent()->addComponentListener(self<ComponentListener>());
}    

void RenderableSurface::componentResized(const ComponentEvent & e) {
    // Resize the viewport
    renderer().viewport().setSizes(e.size());

    // Tell the main Widget its new size
    if (_widget)
        _widget->setSize(e.size());
}


/*---------------------------------------------------------------------------*
 | Embedded Widget accessors
 *---------------------------------------------------------------------------*/
WidgetPtr      RenderableSurface::widget()       { return _widget; }
WidgetConstPtr RenderableSurface::widget() const { return _widget; }
void RenderableSurface::setWidget(WidgetPtr w) {
    // Unregister any previous widget
    if (_widget) {
        removePointerListener(_widget);
        removeKeyListener(_widget);
        removeButtonListener(_widget);
    }
    _widget = w;
    if (_widget) {
        _widget->setParent(self<Component>());
        _widget->setSurface(self<RenderableSurface>());
        _widget->construct();
        
        _widget->setSize(size());
        
        addPointerListener(_widget);
        addKeyListener(_widget);
        addButtonListener(_widget);
    }
}


/*---------------------------------------------------------------------------*
 | Rendering functions
 *---------------------------------------------------------------------------*/
// Render function
void RenderableSurface::render() const {
    if (_widget) {
        Renderer & r = renderer();
        r.beginFrame();
        _widget->render();
        r.endFrame();
    }
}

// Get the Renderer object for this surface
RenderableSurface::Renderer & RenderableSurface::renderer() const { 
    return peer()->renderer();
}

// Request redisplay of the entire Window
void RenderableSurface::requestRedisplay() const {
    peer()->requestRedisplay();
}
