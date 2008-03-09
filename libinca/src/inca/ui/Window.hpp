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
        class WindowPeer;

        // Pointer typedefs
        typedef shared_ptr<Window>       WindowPtr;
        typedef shared_ptr<Window const> WindowConstPtr;
    };
};

// Import superclass definitions
#include "HeavyweightComponent"

// Import 3D renderable surface definition
#include "RenderableSurface.hpp"


class inca::ui::Window
    : public inca::ui::HeavyweightComponent<inca::ui::WindowPeer> {
/*---------------------------------------------------------------------------*
 | Native UI toolkit peer
 *---------------------------------------------------------------------------*/
public:
    // The Application is allowed to set my Peer
    friend class Application;


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional name
    explicit Window(const std::string & nm = std::string());

    // Destructor
    virtual ~Window();
    
    // Second-phase initialization
    virtual void construct();


/*---------------------------------------------------------------------------*
 | XXX
 *---------------------------------------------------------------------------*/
public:
    RenderableSurfacePtr      surface();
    RenderableSurfaceConstPtr surface() const;
    void setSurface(RenderableSurface * s);
    void setSurface(RenderableSurfacePtr s);
//    RenderableSurfacePtr add(RenderableSurface * s);
//    RenderableSurfacePtr add(RenderableSurfacePtr s);

protected:
    RenderableSurfacePtr _surface;
    

/*---------------------------------------------------------------------------*
 | Window state control
 *---------------------------------------------------------------------------*/
public:
    // Window title
    std::string title() const;
    void setTitle(const std::string & title);

    // Window visibility state
    bool visible() const;
    void setVisible(bool v);
    void toggleVisible();

    // Window iconification state
    bool iconified() const;
    void setIconified(bool i);
    void toggleIconified();

    // Window maximization state
    bool maximized() const;
    void setMaximized(bool m);
    void toggleMaximized();

    // Window full-screen state
    bool fullScreen() const;
    void setFullScreen(bool f);
    void toggleFullScreen();

    // Window resizable state
    bool resizable() const;
    void setResizable(bool r);
    void toggleResizable();

    // Restore an iconified or full-screened window to its previous state
    void restore();


/*---------------------------------------------------------------------------*
 | Window position & size
 *---------------------------------------------------------------------------*/
public:
    // Window position
    void centerOnScreen();
    
    
/*---------------------------------------------------------------------------*
 | Event-firing functions
 *---------------------------------------------------------------------------*/
protected:
    // The peer is allowed to call these event-firing functions
    friend class WindowPeer;

    void fireWindowOpened() const;
    void fireWindowClosed() const;
};


// Peer interface class definition
class inca::ui::WindowPeer
    : public inca::ui::ComponentPeer<inca::ui::Window> {
/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit WindowPeer(Window * w) : Base(w) { }
    

/*---------------------------------------------------------------------------*
 | WindowPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Window lifecycle management
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual bool valid() const = 0;

    // Window state accessors
    virtual std::string title() const = 0;
    virtual void setTitle(const std::string & title) = 0;
    virtual bool iconified() const = 0;
    virtual void setIconified(bool i) = 0;
    virtual bool maximized() const = 0;
    virtual void setMaximized(bool m) = 0;
    virtual bool fullScreen() const = 0;
    virtual void setFullScreen(bool f) = 0;
    virtual bool resizable() const = 0;
    virtual void setResizable(bool r) = 0;

    
/*---------------------------------------------------------------------------*
 | Pass-thru to the Window's event-firing functions
 *---------------------------------------------------------------------------*/
protected:
    void fireWindowOpened() const;
    void fireWindowClosed() const;
};

#endif
