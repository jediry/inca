/*
 * File: Framework.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the generic application framework classesApplicationa class, which is a generic
 *      top-level object incorporating functionality common to any
 *      Inca user-interface.
 */

// Import class definitions
#include "Framework.hpp"
using namespace inca;
using namespace inca::ui;


/*****************************************************************************
 * Application stuff
 *****************************************************************************/
// The singleton Application instance
Application * Application::_instance = NULL;


/*---------------------------------------------------------------------------*
 | Initialization
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


/*---------------------------------------------------------------------------*
 | Common initialization/utility functions
 *---------------------------------------------------------------------------*/
// Call all of the other initialization functions
void Application::initialize(int &argc, char **argv) {
    // First, let the toolkit initialize itself (and extract cmd-line args)
    initializeToolkit(argc, argv);

    // Next, get our own command-line arguments and do our own initialization
    setup(argc, argv);

    // Now, assemble the UI
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


/*****************************************************************************
 * View functions
 *****************************************************************************/
void View::requestRedisplay() const {
    if (container() != NULL)
        container()->requestRedisplay(this);
}

/*****************************************************************************
 * Control functions
 *****************************************************************************/
void Control::requestRedisplay() const {
    if (container() != NULL)
        container()->requestRedisplay(NULL);
}

string Control::printControlFlags() const {
    string result(8, ' ');
    if (controlFlags & LEFT_BUTTON)     result[0] = 'L';
    if (controlFlags & MIDDLE_BUTTON)   result[1] = 'M';
    if (controlFlags & RIGHT_BUTTON)    result[2] = 'R';
    if (controlFlags & WHEEL_UP)        result[3] = '^';
    if (controlFlags & WHEEL_DOWN)      result[4] = 'v';
    if (controlFlags & SHIFT_KEY)       result[5] = 'S';
    if (controlFlags & CTRL_KEY)        result[6] = 'C';
    if (controlFlags & ALT_KEY)         result[7] = 'A';
    return result;
}

