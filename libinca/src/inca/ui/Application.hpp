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

        // Pointer typedefs (not using shared_ptr, because Application
        // is a singleton and manages its own existence)
        typedef Application *       ApplicationPtr;
        typedef Application const * ApplicationConstPtr;
    };
};

// Import Timer definition
#include <inca/util/Timer>

// Import other UI class definitions
#include "Window.hpp"

// Import container definitions
#include <vector>

/**
 * The Application abstract class represents a generic application, and
 * defines the interface that the top-level object in a program must
 * implement, as well as defining the skeleton application startup procedure.
 * Subclasses must implement the constructInterface() function, and may also
 * wish to implement the setup(argc, argv) function.
 */
class inca::ui::Application {
private:
    // Set this class up to have properties
    PROPERTY_OWNING_OBJECT(Application);

public:
    // XXX HACK! HACK! WHEEZE!
    // Typedef for the timer we'll use to drive animation
    typedef Timer<double, true> Timer;  // double resolution, generate events

    // The application's main timer
    rw_property(Timer, timer, Timer());


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
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    Application();

    // Destructor
    ~Application();

    // Framework initialization function (called by main() after constructor)
    void initialize(int &argc, char **argv);


/*---------------------------------------------------------------------------*
 | Application setup functions: an Application subclass may override either
 | or both of these to specialize its startup behavior.
 *---------------------------------------------------------------------------*/
public:
    // Do any command-line processing and app-specific initialization
    virtual void setup(int &argc, char **argv) { }

    // Construct any necessary user-interface thingies
    virtual void constructInterface() { }


/*---------------------------------------------------------------------------*
 | Toolkit-integration functions
 *---------------------------------------------------------------------------*/
public:
    // Initialize the UI-toolkit and process toolkit-specific arguments
    virtual void initializeToolkit(int &argc, char **argv) = 0;

    // Launch the application's event-handling mechanism (may not return)
    virtual int run() = 0;

    // Cause the application to terminate
    virtual void exit(int status, const string &msg);


/*---------------------------------------------------------------------------*
 | Window management functions
 *---------------------------------------------------------------------------*/
public:
    void registerComponent(UIComponentPtr c);

    virtual void registerWindow(WindowPtr w);
    WindowPtr getWindowForID(IDType id);
    virtual void destroyWindow(IDType id);

protected:
    std::vector<UIComponentPtr> components;
    std::vector<WindowPtr> windows;


/*---------------------------------------------------------------------------*
 | Utility functions
 *---------------------------------------------------------------------------*/
public:
    // Pull off the first command-line argument and remove it from the list
    static string shift(int &argc, char **&argv);
};


/**
 * The APPLICATION(TOOLKIT) macro expands to produce the fully-qualified class
 * name of the Application subclass implemented using the requested toolkit.
 */
#include <boost/preprocessor/cat.hpp>
#define APPLICATION(TOOLKIT) inca::ui:: BOOST_PP_CAT(TOOLKIT, Application)


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
    app.initialize(argc, argv); /* Initialize it                 */         \
    return app.run();           /* Start it running              */         \
}

#endif
