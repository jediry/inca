/*
 * Filename: GLUTWindow.cpp
 *
 * Author:  Ryan L. Saunders <rls@tamu.edu>
 *          Concept for this design derived from the "glutwrapper" code,
 *          written by Kyle Girard <kgirard@chat.carleton.ca>
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains the implementation of the GLUTWindow class, which
 *      provides a C++ wrapper around GLUT.
 *
 *      FIXME: This doesn't constrain reshaping like it claims to
 *             Also, window centering is not robust
 */


// Import class definition
#include "GLUTWindow.hpp"
using namespace inca;
using namespace inca::ui;

// Default layout parameters
const int WINDOW_DEFAULT_X = 50;
const int WINDOW_DEFAULT_Y = 50;
const int WINDOW_DEFAULT_W = 400;
const int WINDOW_DEFAULT_H = 400;

// Default window constraints
const int WINDOW_MINIMUM_W = 100;
const int WINDOW_MINIMUM_H = 100;
const int WINDOW_MAXIMUM_W = 800;
const int WINDOW_MAXIMUM_H = 800;

// GLUT C++ wrapper static initialization
vector<GLUTWindow *> GLUTWindow::windowList;


/*---------------------------------------------------------------------------*
 | Static GLUT callbacks -- they just call member functions of the window
 *---------------------------------------------------------------------------*/
void GLUTWindow::reshapeFunc(int width, int height) {
    windowList[glutGetWindow()]->reshape(width, height);
}
void GLUTWindow::entryFunc(int state) {
    windowList[glutGetWindow()]->entry(state);
}
void GLUTWindow::visibilityFunc(int visible) {
    windowList[glutGetWindow()]->visibility(visible);
}
void GLUTWindow::motionFunc(int x, int y) {
    windowList[glutGetWindow()]->mouseMotion(x, y);
}
void GLUTWindow::passiveMotionFunc(int x, int y) {
    windowList[glutGetWindow()]->passiveMotion(x, y);
}
void GLUTWindow::mouseFunc(int button, int state, int x, int y) {
    windowList[glutGetWindow()]->mouseButton(button, state, x, y);
}
void GLUTWindow::keyboardFunc(unsigned char k, int x, int y) {
    windowList[glutGetWindow()]->key(k, x, y);
}
void GLUTWindow::specialFunc(int key, int x, int y) {
    windowList[glutGetWindow()]->special(key, x, y);
}
void GLUTWindow::displayFunc() {
    windowList[glutGetWindow()]->display();
}
void GLUTWindow::overlayDisplayFunc() {
    windowList[glutGetWindow()]->overlayDisplay();
}
void GLUTWindow::idleFunc() {
    windowList[glutGetWindow()]->idle();
}


/*---------------------------------------------------------------------------*
 | Single-window constructor
 *---------------------------------------------------------------------------*/
GLUTWindow::GLUTWindow(const string &title)
        : keepSquare(false), fullScreen(false),
          windowX(WINDOW_DEFAULT_X), windowY(WINDOW_DEFAULT_Y),
          width(WINDOW_DEFAULT_W), height(WINDOW_DEFAULT_H),
          minWidth(WINDOW_MINIMUM_W), minHeight(WINDOW_MINIMUM_H),
          maxWidth(WINDOW_MAXIMUM_W), maxHeight(WINDOW_MAXIMUM_H) {

    // Zero is not a valid GLUT windowID
    if (windowList.size() == 0)
        windowList.push_back(0);    // Prime the list with a dummy

    // Create a GLUT window and add it to window management
    windowID = glutCreateWindow(title.c_str());
    if (windowID != windowList.size()) {    // Then things will go horribly
        cerr << "Window " << windowID << " doesn't match window list index "
             << windowList.size() << ". This will royally muck things up."
             << endl;
        exit(1);
    }
    windowList.push_back(this);

    // Register callbacks for this window
    glutReshapeFunc(reshapeFunc);
    glutEntryFunc(entryFunc);
    glutVisibilityFunc(visibilityFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMouseFunc(mouseFunc);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutDisplayFunc(displayFunc);
//    glutOverlayDisplayFunc(overlayDisplayFunc);
    glutIdleFunc(idleFunc);
}


/*---------------------------------------------------------------------------*
 | Window control functions
 *---------------------------------------------------------------------------*/
void GLUTWindow::setMinimumSize(size_t w, size_t h) {
    minWidth = w;
    minHeight = h;

    // Reshape the window, if necessary
    if (minWidth > width || minHeight > height)
        setSize(w, h);
}

void GLUTWindow::setMaximumSize(size_t w, size_t h) {
    maxWidth = w;
    maxHeight = h;

    // Reshape the window, if necessary
    if (minWidth > width || minHeight > height)
        setSize(w, h);
}

void GLUTWindow::setSquare(bool square) {
    keepSquare = square;
    if (keepSquare && width != height) {
        size_t max = width > height ? width : height;
        setSize(max, max);
    }
}

void GLUTWindow::setFullScreen(bool fs) {
#if (GLUT_API_VERSION >= 3)
    if (fs && ! fullScreen) {
        fullScreen = true;
        restoreWidth = width;
        restoreHeight = height;
        glutPushWindow();
            glutSetWindow(windowID);
            glutFullScreen();
        glutPopWindow();
    } else if (!fs && fullScreen) {
        fullScreen = false;
        setPosition(windowX, windowY);
        setSize(restoreWidth, restoreHeight);
    }
#else
    cerr << "Full-screen mode is not supported by this version of GLUT\n";
#endif
}

void GLUTWindow::centerOnScreen() {
    cerr << "GLUTWindow::centerWindow() not implemented -- need to get"
            " screen size" << endl;
}

void GLUTWindow::setPosition(unsigned int x, unsigned int y) {
    glutPushWindow();           // Save the previous window
        glutSetWindow(windowID);    // Pick this window
        glutPositionWindow(x, y);   // Move it
    glutPopWindow();            // Restore the previous window
}

void GLUTWindow::setSize(size_t w, size_t h) {
    // Clamp the height of the window
    if (w > maxWidth)       width = maxWidth;
    else if (w < minWidth)  width = minWidth;
    else                    width = w;

    // Clamp the height of the window
    if (h > maxHeight)      height = maxHeight;
    else if (h < minHeight) height = minHeight;
    else                    height = h;

    // If this should be square, then make it so
    if (keepSquare) {
        if (height > width)
            width = height;
        else
            height = width;
    }

    glutPushWindow();
        glutSetWindow(windowID);
        glutReshapeWindow(width, height);
    glutPopWindow();
}

void GLUTWindow::postRedisplay() const {
    glutPushWindow();
        glutSetWindow(windowID);
        glutPostRedisplay();
    glutPopWindow();
}

void GLUTWindow::drawString(int x, int y, const char *str) {
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();  
    glLoadIdentity();    
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);  
    glPushMatrix();                  
    glLoadIdentity();                    
    glRasterPos2f(x, y);                     
 
    // Draw the characters                       
    unsigned int length = strlen(str);               
    for (unsigned int i = 0; i < length; i++)            
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);       
 
    // Clean up our mess                                                 
    glMatrixMode(GL_PROJECTION);                                             
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
} 


/*---------------------------------------------------------------------------*
 | Member-function callbacks
 *---------------------------------------------------------------------------*/
// GLUT window callbacks
void GLUTWindow::reshape(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);    // Tell GL that we did it
    glutPostRedisplay();
}
void GLUTWindow::entry(int state) { }
void GLUTWindow::visibility(int visible) { }

// GLUT input callbacks
void GLUTWindow::mouseMotion(int x, int y) { }
void GLUTWindow::passiveMotion(int x, int y) { }
void GLUTWindow::mouseButton(int button, int state, int x, int y) { }
void GLUTWindow::key(unsigned char k, int x, int y) { }
void GLUTWindow::special(int key, int x, int y) { }

// GLUT display callbacks
void GLUTWindow::overlayDisplay() { }
//void GLUTWindow::display() { }        // -- Made abstract in header file

// GLUT idle function
void GLUTWindow::idle() { }
