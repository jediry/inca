/*
 * File: Application.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Application abstract class defines the interface that the top-level
 *      object in a program must implement, as well as defining the skeleton
 *      application startup procedure. Subclasses should implement the
 *      constructInterface() function.
 *
 *      Finally,
 */

#ifndef INCA_UI_APPLICATION
#define INCA_UI_APPLICATION

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Application;
        class ApplicationPeer;

        // Pointer typedefs (not using shared_ptr, because Application
        // is a singleton and manages its own existence)
        typedef Application *       ApplicationPtr;
        typedef Application const * ApplicationConstPtr;
    };
};

// Import superclass definition
#include "HeavyweightComponent"

// Import other UI class definitions
#include "Window.hpp"


/**
 * The Application abstract class represents a generic application, and
 * defines the interface that the top-level object in a program must
 * implement, as well as defining the skeleton application startup procedure.
 * Subclasses must implement the constructInterface() function, and may also
 * wish to implement the setup(argc, argv) function.
 */
class inca::ui::Application
    : public inca::ui::HeavyweightComponent<inca::ui::ApplicationPeer> {
/*---------------------------------------------------------------------------*
 | Native UI toolkit peer
 *---------------------------------------------------------------------------*/
public:
    friend class ApplicationPeer;

    
/*---------------------------------------------------------------------------*
 | There can be only one. Application behaves (almost) as a singleton object.
 *---------------------------------------------------------------------------*/
public:
    static bool instanceExists();       // Do we have an Application?
    static Application & instance();    // Get it.
    static void destroy();              // Destroy it (danger!)

protected:
    // Singleton pointer to THE application
    static Application * _instance;


/*---------------------------------------------------------------------------*
 | Constructors, destructor & main function
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    Application();

    // Destructor
    virtual ~Application();

    // Application main function (should be called by main() after constructor)
    virtual int main(int & argc, char **& argv);

    // Cause the application to terminate
    virtual void exit(int status, const std::string & msg);


/*---------------------------------------------------------------------------*
 | Application setup functions: normally, an Application subclass will override
 | either or both of these to specialize its startup behavior.
 *---------------------------------------------------------------------------*/
public:
    // Do any command-line processing and app-specific initialization
    virtual void setup(int &argc, char **& argv) { }

    // Construct any necessary user-interface thingies
    virtual void construct() { }


/*---------------------------------------------------------------------------*
 | Input device state functions
 *---------------------------------------------------------------------------*/
public:
    // NumLock key state
    bool numLockActive() const;
    void setNumLockActive(bool a);
    void toggleNumLock();

    // ScrollLock key state
    bool scrollLockActive() const;
    void setScrollLockActive(bool a);
    void toggleScrollLock();

    // CapsLock key state
    bool capsLockActive() const;
    void setCapsLockActive(bool a);
    void toggleCapsLock();


/*---------------------------------------------------------------------------*
 | Utility functions
 *---------------------------------------------------------------------------*/
public:
    // Pull off the first command-line argument and remove it from the list
    static std::string shift(int & argc, char **& argv);


/*---------------------------------------------------------------------------*
 | Window management functions
 *---------------------------------------------------------------------------*/
public:
    // List of Windows in the application
    typedef std::vector<WindowPtr> WindowList;

    // Add/remove a Window to/from the Application
    WindowPtr add(Window * w);
    WindowPtr add(WindowPtr w);
    void remove(WindowPtr w);

protected:
    WindowList _windows;
};


class inca::ui::ApplicationPeer
    : public inca::ui::ComponentPeer<inca::ui::Application> {
/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit ApplicationPeer(Application * a) : Base(a) { }

    
/*---------------------------------------------------------------------------*
 | ComponentPeer interface functions (no-ops)
 *---------------------------------------------------------------------------*/
public:
    Pixel position() const { return Pixel(0); }
    void setPosition(Pixel) { }
    Dimension size() const { return Dimension(0); }
    void setSize(Dimension) { }
    Dimension minimumSize() const { return Dimension(0); }
    void setMinimumSize(Dimension) { }
    Dimension maximumSize() const { return Dimension(0); }
    void setMaximumSize(Dimension) { }
    bool visible() const { return false; }
    void setVisible(bool v) { }


/*---------------------------------------------------------------------------*
 | ApplicationPeer interface functions
 *---------------------------------------------------------------------------*/
public:
    // Application lifecycle functions
    virtual void initialize(int & argc, char **& argv) = 0;
    virtual int run() = 0;
    virtual void exit(int code) = 0; 
    
    // Stateful key functions
    virtual bool lockKeyState(LockKey k) const = 0;
    virtual void setLockKeyState(LockKey k, bool a) = 0;
};


/**
 * The APPLICATION_MAIN macro expands to produce a stock main() function
 * that creates an instance of the requested Application subclass and
 * launches it. Under most circumstances, this is all you need in main(), as
 * the Application's implementations of setup(argc, argv) and
 * constructInterface() should do the real work of setting up the Application.
 */
#define APPLICATION_MAIN(APP_CLASS)                                         \
int main(int argc, char **argv) {                                           \
    APP_CLASS app;              /* Create an instance of the app */         \
    return app.main(argc, argv);/* Run it                        */         \
}

#endif
