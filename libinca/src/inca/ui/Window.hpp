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
        releaseWidgetPart(_widget); // Tell the old guy we're leaving him...
        _widget = value;            // ...get the new guy...
        acquireWidgetPart(_widget); // ...and tell him that he's ours

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


    // Window ID
    IDType getID() const;

    // Query screen size
    Dimension getScreenSize() const;

    // Request redisplay of the entire Window
    void requestRedisplay() const;

/*---------------------------------------------------------------------------*
 | Window state control
 *---------------------------------------------------------------------------*/
public:
    // Window title
    std::string title() const;
    void setTitle(const std::string & title);

    // Window visibility state
    bool visible() const;
    void setVisible(bool vs);

    // Window iconification state
    bool iconified() const;
    void setIconified(bool icf);

    // Window maximization state
    bool maximized() const;
    void setMaximized(bool max);

    // Window full-screen state
    bool fullScreen() const;
    void setFullScreen(bool fs);

    // Restore an iconified or full-screened window to its previous state
    void restore();


/*---------------------------------------------------------------------------*
 | Window size & position
 *---------------------------------------------------------------------------*/
public:
    // Window position
    Pixel position() const;
    void setPosition(Pixel p);
    void setPosition(IndexType x, IndexType y);
    void centerOnScreen();

    // Current size
    Dimension size() const;
    void setSize(Dimension d);
    void setSize(SizeType w, SizeType h);

    // Minimum allowable size
    Dimension minimumSize() const;
    void setMinimumSize(Dimension d);
    void setMinimumSize(SizeType w, SizeType h);

    // Maximum allowable size
    Dimension maximumSize() const;
    void setMaximumSize(Dimension d);
    void setMaximumSize(SizeType w, SizeType h);

    // Force aspect ratio (0.0 to allow unrestricted A/R)
    float aspectRatio() const;
    void setAspectRatio(float ratio);
};


/**
 * The WINDOW(TOOLKIT) macro expands to produce the fully-qualified class
 * name of the Window subclass implemented using the requested toolkit.
 */
#include <boost/preprocessor/cat.hpp>
#define WINDOW(TOOLKIT) inca::ui:: BOOST_PP_CAT(TOOLKIT, Window)


#endif
