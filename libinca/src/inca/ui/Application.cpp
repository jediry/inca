/*
 * File: Application.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "Application.hpp"
using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Singleton instance access
 *---------------------------------------------------------------------------*/
// The singleton Application instance
Application * Application::_instance;

// Query singleton existence
bool Application::instanceExists() { return _instance != NULL; }

// Return the singleton instance
Application & Application::instance() { return *_instance; }

// Destroy the singleton instance (may have "consequences")
void Application::destroy() {
    if (instanceExists()) {
        delete _instance;
        _instance = NULL;
    }
}


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
Application::Application() : timer(this) {
    if (! instanceExists()) {
        _instance = this;   // Claim singleton-ness
    } else {
        logger << "Application(): There is already an existing "
                  "application instance.";
        logger.error();
        // XXX should this die a painful death?
    }
}

Application::~Application() {
    // Drop our shared references to the windows (shared_ptr will clean up)
    windows.clear();

    // Drop our shared references to the components
    components.clear();
}


void Application::initialize(int &argc, char **argv) {
        // First, let the toolkit initialize itself
        // (and extract/process any relevent  command-line arguments)
        initializeToolkit(argc, argv);

        // Next, we get our own application-specific command-line arguments
        // and do any necessary initialization
        setup(argc, argv);

        // Now, assemble the user interface
        constructInterface();
}


/*---------------------------------------------------------------------------*
 | Toolkit-integration functions
 *---------------------------------------------------------------------------*/
// Cause the application to terminate
void Application::exit(int status, const string &msg) {
    cerr << msg << endl;    // Print out the message...
    ::exit(status);         // ...and bail out of the program
};


/*---------------------------------------------------------------------------*
 | GUI object management functions
 *---------------------------------------------------------------------------*/
void Application::registerComponent(UIComponentPtr c) {
    components.push_back(c);
}

void Application::registerWindow(WindowPtr w) {
    IDType id = w->getID();
    windows.resize(std::max(id + 1, windows.size()));   // Make room
    windows[id] = w;                                    // Claim my ID
}

WindowPtr Application::getWindowForID(IDType id) {
    return windows[id];
}

void Application::destroyWindow(IDType id) {
    windows[id] = WindowPtr();  // Forget about it
}


/*---------------------------------------------------------------------------*
 | Utility functions
 *---------------------------------------------------------------------------*/
// Pull off the first command-line argument and remove it from the list
string Application::shift(int &argc, char **&argv) {
    string result;
    if (argc > 1) {
        result = argv[1];   // Save the first argument into a string
        for (IndexType i = 1; i < argc; ++i)  // Shuffle pointers for other args
            argv[i] = argv[i+1];
        argc--;             // Decrement the count
    }
    return result;
}
