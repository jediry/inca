/*
 * File: CompositeWidget.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The CompositeWidget class is a concrete Widget that is essentially
 *      the union of another View and Control (in fact, it's really nothing
 *      but a Widget that inherits PassThruView and PassThruControl).
 *      Using this as a sort of "glue", a separate View and Control may be
 *      combined together into a single Widget. This could be useful, for
 *      example, for creating multiple Views sharing a single set of event
 *      handlers, or conversely a single View with a whole toolbox of user
 *      controls that are swapped in/out as needed.
 */

#ifndef INCA_UI_COMPOSITE_WIDGET
#define INCA_UI_COMPOSITE_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class CompositeWidget;

        // Pointer typedefs
        typedef shared_ptr<CompositeWidget>       CompositeWidgetPtr;
        typedef shared_ptr<CompositeWidget const> CompositeWidgetConstPtr;
    };
};

// Import superclass definitions
#include "Widget.hpp"
#include "PassThruView.hpp"
#include "PassThruControl.hpp"


class inca::ui::CompositeWidget : public Widget,
                                  public PassThruView,
                                  public PassThruControl {
public:
    // Default constructor with an optional component name
    CompositeWidget(const string &nm = "")
        : Widget(nm) { }

    // Constructor with explicit initialization of View/Control
    CompositeWidget(ViewPtr v, ControlPtr c, const string &nm = "")
        : PassThruView(v), PassThruControl(c), Widget(nm) { }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer function to propagate redisplay requests
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartPtr w) { requestRedisplay(); }
};

#endif
