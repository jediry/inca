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

// Import GLUT
#include <GL/glut.h>


// Initialize a GLUT-based application
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

// Request a GLUT-based window with the specified title
WindowPtr GLUTApplication::createWindow(const string &title) {
    return WindowPtr(new GLUTWindow(title));
}

// Request a GLUT-based window with the specified title containing a widget
WindowPtr GLUTApplication::createWindow(WidgetPtr w, const string &title) {
    return WindowPtr(new GLUTWindow(w, title));
}

