/*
 * File: QTApplication.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The QTApplication class implements the basic functionality for an
 *      Inca application built using the QT toolkit. A concrete QT-based
 *      application need only implement the constructInterface function.
 */

#ifndef INCA_QT_APPLICATION
#define INCA_QT_APPLICATION

// Import superclass definition
#include "../generic/Application.hpp"

// Import QT stuff
#include <qapplication.h>


// This is part of the Inca interface layer
namespace Inca {
    namespace UI {
        // Forward declarations
        class QTApplication;
    };
};


class Inca::UI::QTApplication : public Application {
/*---------------------------------------------------------------------------*
 | Toolkit-integration functions
 *---------------------------------------------------------------------------*/
public:
    // Initialize the UI-toolkit and process toolkit-specific arguments
    virtual void initializeToolkit(int &argc, char **argv) {
        new QApplication(argc, argv);
    }

    // Launch the application's event-handling mechanism (may not return)
    virtual int run() {
        return qApp->exec();
    }

    // Cause the application to terminate
    virtual void exit(int status, const string &msg) {
        if (msg != "")
            cerr << msg << endl;
        qApp->exit(status);
    }
};

#endif
