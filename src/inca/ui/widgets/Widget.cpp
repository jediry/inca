/** -*- C++ -*-
 *
 * \file    Widget.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the functions belonging to the Widget class.
 */

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "Widget.hpp"

// Import exception definition
#include <inca/util/IllegalStateException.hpp>

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
// Non-public constructor with optional component name
Widget::Widget(const std::string & nm) : Component(nm) { }


/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface and other Widgets
 *---------------------------------------------------------------------------*/
// Claim another widget as a child
void Widget::adopt(WidgetPtr w) {
    if (w) {
        w->setParent(self<Component>());
        w->setSurface(surface());
        w->construct();
    }
}

// Make another widget be no longer a child
void Widget::disown(WidgetPtr w) {
    if (w) {
//        w->destruct();
        w->setParent(ComponentPtr());
        w->setSurface(RenderableSurfacePtr());
    }
}        

// Access to the RenderableSurface that holds this Widget
RenderableSurfacePtr      Widget::surface()       { return _surface; }
RenderableSurfaceConstPtr Widget::surface() const { return _surface; }
void Widget::setSurface(RenderableSurfacePtr s) { _surface = s; }

// Access to the Renderer instance for the RenderableSurface. This function
// throws an IllegalStateException if the Widget is not currently attached
// to a RenderableSurface.
RenderableSurface::Renderer & Widget::renderer() const {
    if (! _surface) {
        IllegalStateException e;
        e << __FUNCTION__ << "Widget '" << this->name() << "' is not attached to "
             "an instance of RenderableSurface";
        throw e;
    }
    return _surface->renderer();
}

// Request that the owning RenderableSurface invoke the render() function
// on me at its earliest convenience.
// This function throws an IllegalStateException if the Widget is not
// currently attached to a RenderableSurface.
void Widget::requestRedisplay() const {
    if (! _surface) {
        IllegalStateException e;
        e << __FUNCTION__ << "Widget '" << this->name() << "' is not attached to "
             "an instance of RenderableSurface";
        throw e;
    }
    _surface->requestRedisplay();
}
