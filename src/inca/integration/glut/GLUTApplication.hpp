/*
 * File: GLUTApplication.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The GLUTApplication class is the top-level class in an application,
 *      specialized to use the GLUT toolkit.
 */

#pragma once
#ifndef INCA_INTEGRATION_GLUT_APPLICATION
#define INCA_INTEGRATION_GLUT_APPLICATION

// Import library configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class GLUTApplication;

        // Pointer typedefs
        typedef GLUTApplication *       GLUTApplicationPtr;
        typedef GLUTApplication const * GLUTApplicationConstPtr;
    };
};

// Import superclass definition
#include <inca/ui/Application.hpp>


class inca::ui::GLUTApplication : virtual public Application {
/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    GLUTApplication();

    // Destructor
    ~GLUTApplication();


/*---------------------------------------------------------------------------*
 | Application-inherited functions
 *---------------------------------------------------------------------------*/
public:
    // Initialize the application
    void initializeToolkit(int & argc, char ** argv);

    // Launch the application
    int run();

    // Exit the application
    void exit(int status, const std::string & msg);
};

#endif
