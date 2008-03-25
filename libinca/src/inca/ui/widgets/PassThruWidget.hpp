/*
 * File: PassThruWidget.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PassThruWidget class is a Widget implementation that simply wraps
 *      another Widget, and passes events through to the wrapped Widget. This
 *      facilitates the easy construction of "filtering" widgets, which
 *      intercept and/or react to certain events (e.g., the Ctrl-Q sequence for
 *      quitting the application) and pass along other events for consumption
 *      by the wrapped widget. Such widgets can be chained to an arbitrary
 *      degree, allowing interactive applications to be built in a highly
 *      modular fashion.
 *
 *      By default, PassThruWidgets are "shrink-wrapped" to fit the Widgets
 *      they wrap--
 */

#pragma once
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


class inca::ui::PassThruWidget : public inca::ui::BasicWidget {
private:
    // Set this class up to contain properties
    PROPERTY_OWNING_OBJECT(PassThruWidget);


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with an optional component name
    explicit PassThruWidget(const std::string & nm = std::string());

    // The Widget that we're wrapping
    rw_ptr_property_custom_set(Widget, widget, NULL);

    // Custom setter for the "widget" property
    void ptr_property_set(Widget, widget);


/*---------------------------------------------------------------------------*
 | Component size & position (passed-thru to the wrapped Widget)
 *---------------------------------------------------------------------------*/
public:
    // Current position
    void setPosition(Pixel p);
    void setPosition(IndexType x, IndexType y);

    // Current size
    void setSize(Dimension d);
    void setSize(SizeType w, SizeType h);

    // Minimum allowable size
    void setMinimumSize(Dimension d);
    void setMinimumSize(SizeType w, SizeType h);

    // Maximum allowable size
    void setMaximumSize(Dimension d);
    void setMaximumSize(SizeType w, SizeType h);

    // Visibility
    bool visible() const;
    void setVisible(bool v);
    

/*---------------------------------------------------------------------------*
 | Widget-related events (passed-thru to the wrapped Widget, if there is one)
 *---------------------------------------------------------------------------*/
public:
    // Render function
    void render() const;

    // ComponentEvent handlers
    void componentMoved(const ComponentEvent & e);
    void componentResized(const ComponentEvent & e);
    void componentHidden(const ComponentEvent & e);
    void componentRevealed(const ComponentEvent & e);
    
    // KeyEvent handlers
    void keyPressed(const KeyEvent & e);
    void keyReleased(const KeyEvent & e);
    void keyTyped(const KeyEvent & e);
    
    // PointerEvent handlers
    void pointerDragged(const PointerEvent & e);
    void pointerTracked(const PointerEvent & e);
    void pointerEntered(const PointerEvent & e);
    void pointerExited(const PointerEvent & e);

    // ButtonEvent handlers
    void buttonPressed(const ButtonEvent & e);
    void buttonReleased(const ButtonEvent & e);
    void buttonClicked(const ButtonEvent & e);


/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface
 *---------------------------------------------------------------------------*/
protected:
    void setSurface(RenderableSurfacePtr s);
};

#endif
