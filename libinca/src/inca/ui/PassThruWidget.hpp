/*
 * File: PassThruWidget.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PassThruWidget class is a concrete Widget that simply wraps
 *      another Widget, and passes events through to the wrapped Widget.
 *      This makes a convenient base class for Widgets that want to intercept
 *      certain events (e.g., the Ctrl-Q sequence for quitting the application)
 *      and pass through the rest of them (mouse clicks, other key presses).
 */

#ifndef INCA_UI_PASS_THRU_WIDGET
#define INCA_UI_PASS_THRU_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class PassThruWidget;

        // Pointer typedefs
        typedef shared_ptr<PassThruWidget>       PassThruWidgetPtr;
        typedef shared_ptr<PassThruWidget const> PassThruWidgetConstPtr;
    };
};

// Import superclass definition
#include "BasicWidget.hpp"


class inca::ui::PassThruWidget : public BasicWidget,
                                 virtual public WidgetPartContainer {
private:
    // Set this class up to contain properties
    PROPERTY_OWNING_OBJECT(PassThruWidget);


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with an optional component name
    explicit PassThruWidget(const string &nm = "")
        : widget(this) { name = nm; }

    // Constructor with explicit initialization of Widget
    explicit PassThruWidget(WidgetPtr w, const string &nm = "")
        : widget(this, w) { name = nm; }

    // The Widget that we're wrapping
    rw_ptr_property_custom_set(Widget, widget, NULL);

    // Custom setter for the "widget" property
    void ptr_property_set(Widget, widget) {
        releaseWidgetPart(_widget); // Let the old Widget go
        _widget = value;            // Set the new Widget
        acquireWidgetPart(_widget); // Tell him we got 'im

        // Make sure the widget knows what size he is
        if (_widget) _widget->resizeView(size);
    }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer functions
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartConstPtr w) const { requestRedisplay(); }
    
    // My sub-widget has same dimensions as me
    Dimension getSize(WidgetPartConstPtr w) const { return size; }

    // Ask my parent who the big daddy Window is
    WindowPtr getContainingWindow() const {
        return WidgetPart::getContainingWindow();
    }


/*---------------------------------------------------------------------------*
 | Widget-related events (passed-thru to the wrapped Widget, if there is one)
 *---------------------------------------------------------------------------*/
public:
    // View-inherited events
    void initializeView() {
        BasicWidget::initializeView();
        if (widget) widget->initializeView();
    }
    void resizeView(Dimension size) {
        BasicWidget::resizeView(size);
        if (widget) widget->resizeView(size);
    }
    void renderView() {
        BasicWidget::renderView();
        if (widget) widget->renderView();
    }

    // Control-inherited events
    void keyPressed(KeyCode k, Pixel p) {
        BasicWidget::keyPressed(k, p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->keyPressed(k, p);
        }
    }
    void mouseTracked(Pixel p) {
        BasicWidget::mouseTracked(p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->mouseTracked(p);
        }
    }
    void mouseDragged(Pixel p) {
        BasicWidget::mouseDragged(p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->mouseDragged(p);
        }
    }
    void buttonPressed(MouseButton b, Pixel p) {
        BasicWidget::buttonPressed(b, p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->buttonPressed(b, p);
        }
    }
    void buttonReleased(MouseButton b, Pixel p) {
        BasicWidget::buttonReleased(b, p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->buttonReleased(b, p);
        }
    }
    void buttonClicked(MouseButton b, Pixel p) {
        BasicWidget::buttonClicked(b, p);
        if (widget) {
            widget->setControlFlags(getControlFlags());    // Pass in my flags
            widget->buttonClicked(b, p);
        }
    }
};

#endif
