/*
 * File: GLUTApplication.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The GLUTApplication class is the top-level class in an application,
 *      specialized to use the GLUT toolkit.
 */

// Import class definition
#include "GLUTApplication.hpp"

// Import related Window subclass
#include "GLUTWindow.hpp"
using namespace inca::ui;
using namespace inca;

// Import GLUT
#include <GL/glut.h>


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
// Constructor
GLUTApplication::GLUTApplication() { }

// Destructor
GLUTApplication::~GLUTApplication() { }

// Initialize GLUT
void GLUTApplication::initializeToolkit(int &argc, char **argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}

// Launch the application
int GLUTApplication::run() {
    glutMainLoop();
    return 0;       // Never actually returns
}

// Exit the application
void GLUTApplication::exit(int status, const string &msg) {
    cerr << msg << endl;
    ::exit(status);
}
