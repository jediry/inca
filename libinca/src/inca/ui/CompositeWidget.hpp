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
    explicit CompositeWidget(const string &nm = "")
        : Widget(nm) { }

    // Constructor with explicit initialization of View/Control
    explicit CompositeWidget(ViewPtr v, ControlPtr c, const string &nm = "")
        : PassThruView(v), PassThruControl(c), Widget(nm) { }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer functions
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartConstPtr w) const { requestRedisplay(); }

    // My sub-widget has same dimensions as me
    Dimension getSize(WidgetPartConstPtr wp) const { return BasicView::size; }


/*---------------------------------------------------------------------------*
 | Event-handling is passed up to the PassThru* superclasses (Widget is only
 | around so that this can be used as a Widget subclass).
 *---------------------------------------------------------------------------*/
public:
    // Things handled by PassThruView
    void initializeView()           { PassThruView::initializeView(); }
    void resizeView(Dimension size) { PassThruView::resizeView(size); }
    void renderView()               { PassThruView::renderView(); }

    // Things handled by PassThruControl
    void keyPressed(KeyCode key, Pixel p) { PassThruControl::keyPressed(key, p); }
    void mouseTracked(Pixel p) { PassThruControl::mouseTracked(p); }
    void mouseDragged(Pixel p) { PassThruControl::mouseDragged(p); }
    void buttonPressed(MouseButton b, Pixel p)  { PassThruControl::buttonPressed(b, p); }
    void buttonReleased(MouseButton b, Pixel p) { PassThruControl::buttonReleased(b, p); }
    void buttonClicked(MouseButton b, Pixel p)  { PassThruControl::buttonClicked(b, p); }
};

#endif
