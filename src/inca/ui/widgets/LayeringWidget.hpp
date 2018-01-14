/*
 * File: LayeringWidget.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The LayeringWidget class acts as a simple "meta-widget", allowing
 *      an arbitrary number of sub-widgets to overlap one another, like a
 *      stack of transparencies. The sub-widgets are rendered back-to-front,
 *      and events are forwarded to all of the sub-widgets.
 *
 *      TODO: should widgets be allowed to "consume" events?
 *      TODO: refactor this w/r to MultiplexorWidget
 *      TODO: terminology is ghetto
 */

#pragma once
#ifndef INCA_UI_LAYERING_WIDGET
#define INCA_UI_LAYERING_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class LayeringWidget;

        // Pointer typedefs
        typedef shared_ptr<LayeringWidget>       LayeringWidgetPtr;
        typedef shared_ptr<LayeringWidget const> LayeringWidgetConstPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"

// Import container definitions
#include <vector>

class inca::ui::LayeringWidget : public PassThruWidget {
public:
    // Container typedefs
    typedef std::vector<WidgetPtr> WidgetList;


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional component name
    explicit LayeringWidget(const std::string & nm = "")
        : PassThruWidget(nm) { }

    // Constructor taking an initial Widget and optional component name
    explicit LayeringWidget(WidgetPtr w, const std::string & nm = "")
        : PassThruWidget(nm) { addWidget(w); }

    // Constructor taking a group of Widgets, and an optional component name
    template <class InputIterator>
    LayeringWidget(InputIterator first, InputIterator last,
                      const std::string & nm = "") : PassThruWidget(nm) {
        while (first != last) { // Move across this iterator range, adding
            addWidget(*first);  // each Widget in sequence
            ++first;
        }
    }

    // Search functions
    IndexType indexOf(WidgetConstPtr w) const; // First index of 'w'
    IndexType indexOf(const std::string & nm) const; // First widget named 'nm'
    IndexType indexBefore(IndexType from) const; // Previous widget before 'from'
    IndexType indexAfter(IndexType from)  const; // Next widget after 'from'

    static const IndexType NOT_FOUND = -1;// The "I couldn't find it" index

    // Add/remove functions
    void addWidget(WidgetPtr w);        // Stick a new Widget into the list
    bool removeWidget(WidgetPtr w);     // Remove the first occurance of 'w'
    bool removeWidget(const std::string & nm);// Remove the widget with name 'nm'
    bool removeWidget(IndexType index);   // Remove the widget at 'index'

    // Toggle individual layer visibility
    void setLayerVisible(int index, bool visible);
    bool isLayerVisible(int index);
    bool toggleLayerVisible(int index);

    // Render function
    void render() const;


/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface
 *---------------------------------------------------------------------------*/
protected:
    void setSurface(RenderableSurfacePtr s);

protected:
    WidgetList widgets;                 // The Widgets we're managing
    std::vector<bool> layerVisibility;
};

#endif
