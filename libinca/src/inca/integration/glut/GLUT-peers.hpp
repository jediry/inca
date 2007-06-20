/** -*- C++ -*-
 *
 * \file    GLUT-peers.hpp
 *
 * \author  Ryan L. Saunders <rls@tamu.edu>
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains the definitions of the Peer classes implementing
 *      the Inca UI framework for the GL Utilities Toolkit (GLUT).
 */

#ifndef INCA_INTEGRATION_GLUT_PEERS
#define INCA_INTEGRATION_GLUT_PEERS

// This is part of the Inca user-interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class GLUTApplicationPeer;
        class GLUTWindowPeer;
        class GLUTRenderableSurfacePeer;
    }
}


// Import Peer interface class definitions
#include <inca/ui/Application.hpp>
#include <inca/ui/Window.hpp>
#include <inca/ui/RenderableSurface.hpp>

// Import container definitions
#include <vector>
#include <inca/util/FlagSet>


// Peer class for the Application object
class inca::ui::GLUTApplicationPeer
    : public inca::ui::ApplicationPeer {
/*---------------------------------------------------------------------------*
 | Constructor & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit GLUTApplicationPeer(Application * a);
    
    // Destructor
    ~GLUTApplicationPeer();

#if 0
/*---------------------------------------------------------------------------*
 | Interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
public:
    static void reshapeFunc(int width, int height);
    static void visibilityFunc(int visible);

protected:
    static void registerPeer(GLUTWindowPeer * peer);
    static void deregisterPeer(GLUTWindowPeer * peer);

    // List of Window::Peers, indexed by GLUT window ID
    typedef std::vector<GLUTWindowPeer *> PeerList;
    static PeerList _peers;
#endif    

/*---------------------------------------------------------------------------*
 | ApplicationPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Application lifecycle functions
    void initialize(int & argc, char **& argv);
    int run();
    void exit(int code);
    
    // Input device state functions
    bool lockKeyState(LockKey k) const;
    void setLockKeyState(LockKey k, bool a);
};


// Peer class for the Window object
class inca::ui::GLUTWindowPeer : public inca::ui::WindowPeer {
/*---------------------------------------------------------------------------*
 | Interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
public:
    static void reshapeFunc(int width, int height);
    static void visibilityFunc(int visible);
    
protected:
    // Peer (de)registration for receiving events
    static void registerPeer(GLUTWindowPeer * peer);
    static void deregisterPeer(GLUTWindowPeer * peer);

    // Static peer object registry, indexed by GLUT window ID
    typedef std::vector<GLUTWindowPeer *> PeerList;
    static PeerList _peers;
    
    // The GLUTRenderableSurfacePeer needs access to this stuff
    friend class GLUTRenderableSurfacePeer;
    

/*---------------------------------------------------------------------------*
 | Constructor & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit GLUTWindowPeer(Window * w);
    
    // Destructor
    ~GLUTWindowPeer();

    // The numeric ID of the managed GLUT window
    IDType id() const;
    
    // Pointer to the GLUTRenderableSurfacePeer owned by this GLUTWindowPeer
    GLUTRenderableSurfacePeer       * renderableSurfacePeer();
    GLUTRenderableSurfacePeer const * renderableSurfacePeer() const;

protected:
    static const IDType NO_WINDOW;  // ID indicating no managed window
    IDType _id;                     // The ID of the managed GLUT window
    bool _visible,                  // Is the window visible?
         _iconified,                // Is the window iconified?
         _fullScreen;               // Is the window full-screen?
    std::string _title;             // The current title of the window
    Pixel       _restoreToPosition; // Position to restore to from FS/iconifiged
    Dimension   _restoreToSize;     // Size to restore to from FS/iconified
    

/*---------------------------------------------------------------------------*
 | ComponentPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    Pixel position() const;
    void setPosition(Pixel);
    Dimension size() const;
    void setSize(Dimension);
    Dimension minimumSize() const;
    void setMinimumSize(Dimension);
    Dimension maximumSize() const;
    void setMaximumSize(Dimension);
    bool visible() const;
    void setVisible(bool v);


/*---------------------------------------------------------------------------*
 | WindowPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Window lifecycle management
    void create();
    void destroy();
    bool valid() const;

    // Window state accessors
    std::string title() const;
    void setTitle(const std::string & title);
    bool iconified() const;
    void setIconified(bool i);
    bool maximized() const;
    void setMaximized(bool m);
    bool fullScreen() const;
    void setFullScreen(bool f);
    bool resizable() const;
    void setResizable(bool r);
};


class inca::ui::GLUTRenderableSurfacePeer
    : public inca::ui::RenderableSurfacePeer {
/*---------------------------------------------------------------------------*
 | Interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
public:
    static void displayFunc();
    static void overlayDisplayFunc();
    static void keyboardFunc(unsigned char key, int x, int y);
    static void mouseFunc(int button, int state, int x, int y);
    static void motionFunc(int x, int y);
    static void passiveMotionFunc(int x, int y);
    static void entryFunc(int state);
    static void specialFunc(int key, int x, int y);
    
protected:
    // The GLUTWindowPeer needs access to this stuff
    friend class GLUTWindowPeer;

    // Peer (de)registration for receiving events
    static void registerPeer(GLUTRenderableSurfacePeer * p);
    static void deregisterPeer(GLUTRenderableSurfacePeer * p);

    // Static peer object registry, indexed by GLUT window ID
    typedef std::vector<GLUTRenderableSurfacePeer *> PeerList;
    static PeerList _peers;
    

    // Translate a GLUT mouse button into an Inca MouseButton
    static ButtonCode translateButton(int button);

    // Translate a GLUT key into an Inca KeyCode
    static KeyCode translateNormalKey(unsigned char key);
    static KeyCode translateSpecialKey(int key);

    // Timers for each supported button, for detecting a "click"
    typedef std::vector< inca::Timer<float, false> > TimerList;
    static const float CLICK_DURATION;          // How long can a click be?
    static TimerList _buttonTimers;             // Timers for each button

    // Last known set of active modifier keys, stateful keys, and buttons
    static FlagSet<unsigned int> _inputStateFlags;


/*---------------------------------------------------------------------------*
 | Constructor & destructor, and access to owning GLUTWindowPeer
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit GLUTRenderableSurfacePeer(RenderableSurface * s);
    
    // Destructor
    ~GLUTRenderableSurfacePeer();

    // The numeric ID of the managed GLUT window
    IDType id() const;
    
    GLUTWindowPeer       * windowPeer();
    GLUTWindowPeer const * windowPeer() const;

protected:
    static const IDType NO_WINDOW;  // ID indicating no managed window
    IDType _id;                     // The ID of the managed GLUT window


/*---------------------------------------------------------------------------*
 | ComponentPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    Pixel position() const;
    void setPosition(Pixel);
    Dimension size() const;
    void setSize(Dimension);
    Dimension minimumSize() const;
    void setMinimumSize(Dimension);
    Dimension maximumSize() const;
    void setMaximumSize(Dimension);
    bool visible() const;
    void setVisible(bool v);


/*---------------------------------------------------------------------------*
 | RenderableSurfacePeer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Object lifecycle management
    void create();
    void destroy();
    bool valid() const;

    // Rendering
    Renderer & renderer() const;
    void requestRedisplay() const;
    
protected:
    Renderer * _renderer;
};

#endif
