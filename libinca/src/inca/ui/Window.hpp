/*
 * File: Window.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *
 * FIXME: you CANNOT add widgets inside the subclass constructor, since no
 * shared_ptr has been constructed yet, and the self() function is not valid.
 */

#ifndef INCA_UI_WINDOW
#define INCA_UI_WINDOW

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
// These have been moved to WidgetPart.hpp, in order to resolve a circular
// dependency issue. Too bad.

//        // Forward declarations
//        class Window;
//
//        // Pointer typedefs
//        typedef shared_ptr<Window>       WindowPtr;
//        typedef shared_ptr<Window const> WindowConstPtr;
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
    explicit Window(const std::string & nm = "")
        : widget(this) { name = nm; }

    // Constructor specifying the Widget to display
    explicit Window(WidgetPtr w, const std::string & nm = "")
        : widget(this, w) { name = nm; }

    // This is the widget that we're displaying
    rw_ptr_property_custom_set(Widget, widget, NULL);

    // Custom setter for property "widget"
    void ptr_property_set(Widget, widget) {
//        releaseWidgetPart(_widget); // Tell the old guy we're leaving him...
        _widget = value;            // ...get the new guy...
//        acquireWidgetPart(_widget); // ...and tell him that he's ours

        // Tell the child Widget about its context
        if (_widget) {
            _widget->setRenderer(this->_renderer);
        }
    }

    Renderer & renderer() {
        if (_renderer)
            return *_renderer;
        else
            return * static_cast<Renderer*>(NULL);
    }
    const Renderer & renderer() const {
        if (_renderer)
            return *_renderer;
        else
            return * static_cast<Renderer*>(NULL);
    }

protected:
    // HACK
    RendererPtr _renderer;


/*---------------------------------------------------------------------------*
 | WidgetPartContainer functions
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartConstPtr w) const { requestRedisplay(); }

    // My sub-widget has same dimensions as me
    Dimension getSize(WidgetPartConstPtr w) const { return getSize(); }

    // Return self as the containing Window
    WindowPtr getContainingWindow() const {
        return const_cast<Window *>(this)->self();
    }


/*---------------------------------------------------------------------------*
 | Window interface functions (must be implemented for specific GUI toolkit)
 *---------------------------------------------------------------------------*/
public:
    // Window ID
    virtual IDType getID() const = 0;

    // Window title
    virtual std::string getTitle() const = 0;
    virtual void setTitle(const std::string & title) = 0;

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
    virtual Pixel getPosition() const = 0;
    virtual void setPosition(Pixel p) = 0;
    void setPosition(IndexType x, IndexType y) { setPosition(Pixel(x, y)); }
    virtual void centerOnScreen() {
        Dimension scr = getScreenSize();
        Dimension sz = getSize();
        setPosition(Pixel((scr[0] - sz[0]) / 2, (scr[1] - sz[1]) / 2));
    }

    // Current size
    virtual Dimension getSize() const = 0;
    virtual void setSize(Dimension d) = 0;
    void setSize(SizeType w, SizeType h) { setSize(Dimension(w, h)); }

    // Minimum allowable size
    virtual Dimension getMinimumSize() const = 0;
    virtual void setMinimumSize(Dimension d) = 0;
    void setMinimumSize(SizeType w, SizeType h) { setMinimumSize(Dimension(w, h)); }

    // Maximum allowable size
    virtual Dimension getMaximumSize() const = 0;
    virtual void setMaximumSize(Dimension d) = 0;
    void setMaximumSize(SizeType w, SizeType h) { setMaximumSize(Dimension(w, h)); }

    // Force aspect ratio (0.0 to allow unrestricted A/R)
    virtual float getAspectRatio() const = 0;
    virtual void setAspectRatio(float ratio) = 0;

    // Query screen size
    virtual Dimension getScreenSize() const = 0;

    // Request redisplay of the entire Window
    virtual void requestRedisplay() const = 0;
};


/**
 * The WINDOW(TOOLKIT) macro expands to produce the fully-qualified class
 * name of the Window subclass implemented using the requested toolkit.
 */
#include <boost/preprocessor/cat.hpp>
#define WINDOW(TOOLKIT) inca::ui:: BOOST_PP_CAT(TOOLKIT, Window)


#endif
