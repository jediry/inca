/*
 * File: Window.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_WINDOW
#define INCA_UI_WINDOW

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Window;

        // Pointer typedefs
        typedef shared_ptr<Window>       WindowPtr;
        typedef shared_ptr<Window const> WindowConstPtr;
    };
};

// Import superclass definition
#include "WidgetPart.hpp"

// Import Widget definition
#include "Widget.hpp"


/**
 * The Window class specifies the functions that a toolkit-specific
 * window must implement.
 */
class inca::ui::Window : virtual public WidgetPartContainer {
private:
    // Set up this object to contain properties
    PROPERTY_OWNING_OBJECT(Window);

    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(Window);


/*---------------------------------------------------------------------------*
 | Constructor and properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional component name
    Window(const string &nm = "") : WidgetPartContainer(nm), widget(this) { }

    // Constructor specifying the Widget to display
    Window(WidgetPtr w, const string &nm = "")
        : WidgetPartContainer(nm), widget(this, w) { }

    // This is the widget that we're displaying
    rw_ptr_property_custom_set(Widget, widget, NULL);

    // Custom setter for property "widget"
    void ptr_property_set(Widget, widget) {
        releaseWidgetPart(_widget); // Tell the old guy we're leaving him...
        _widget = value;            // ...get the new guy...
        acquireWidgetPart(_widget); // ...and tell him that he's ours
    }


/*---------------------------------------------------------------------------*
 | Window interface functions (must be implemented for specific GUI toolkit)
 *---------------------------------------------------------------------------*/
public:
    // Window title
    virtual string getTitle() const = 0;
    virtual void setTitle(const string &title) = 0;

    // Window visibility state
    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    // Window iconification state
    virtual bool isIconified() const = 0;
    virtual void setIconified(bool iconified) = 0;

    // Window full-screen state
    virtual bool isFullScreen() const = 0;
    virtual void setFullScreen(bool fs) = 0;

    // Restore an iconified or full-screened window to its previous state
    virtual void restore() = 0;

    // Window position
    virtual Point getPosition() const = 0;
    virtual void setPosition(Point p) = 0;
    void setPosition(index_t x, index_t y) { setPosition(Point(x, y)); }
    virtual void centerOnScreen() {
        Dimension scr = getScreenSize();
        Dimension sz = getSize();
        setPosition(Point((scr[0] - sz[0]) / 2, (scr[1] - sz[1]) / 2));
    }

    // Current size
    virtual Dimension getSize() const = 0;
    virtual void setSize(Dimension d) = 0;
    void setSize(size_t w, size_t h) { setSize(Dimension(w, h)); }

    // Minimum allowable size
    virtual Dimension getMinimumSize() const = 0;
    virtual void setMinimumSize(Dimension d) = 0;
    void setMinimumSize(size_t w, size_t h) { setMinimumSize(Dimension(w, h)); }

    // Maximum allowable size
    virtual Dimension getMaximumSize() const = 0;
    virtual void setMaximumSize(Dimension d) = 0;
    void setMaximumSize(size_t w, size_t h) { setMaximumSize(Dimension(w, h)); }

    // Force aspect ratio (0.0 to allow unrestricted A/R)
    virtual float getAspectRatio() const = 0;
    virtual void setAspectRatio(float ratio) = 0;

    // Query screen size
    virtual Dimension getScreenSize() const = 0;

    // Request redisplay of the entire Window
    virtual void requestRedisplay() = 0;
    virtual void redisplay(WidgetPartPtr w) { requestRedisplay(); }
};

#endif
