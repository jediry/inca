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


// The singleton Application instance
Application * Application::_instance;


/*---------------------------------------------------------------------------*
 | Initialization functions
 *---------------------------------------------------------------------------*/
Application::Application() : timer(this) {
    if (_instance == NULL)
        _instance = this;
    else {
        logger << "Application(): There is already an existing "
                  "application instance.";
        logger.error();
        // XXX should this die a painful death?
    }
}

// Call all of the other initialization functions
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
 | GUI management functions
 *---------------------------------------------------------------------------*/
void Application::registerComponent(UIComponentPtr uic) {
    components.push_back(uic);
}
