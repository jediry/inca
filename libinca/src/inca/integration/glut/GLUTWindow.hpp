/*
 * Filename: GLUTWindow.hpp
 *
 * Author:  Ryan L. Saunders <rls@tamu.edu>
 *          Concept for this design was derived from the "glutwrapper" code,
 *          written by Kyle Girard <kgirard@chat.carleton.ca>
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The GLUTWindow class provides a C++ wrapper around the (C-based) GLUT
 *      library toolkit. All windows in the application should subclass this.
 *      It contains high-level GL- and GLUT-related methods for managing
 *      windows and graphics modes.
 */

#ifndef GLUT_WINDOW
#define GLUT_WINDOW

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class GLUTWindow;
    };
};


// Import GL Utilities Toolkit (GLUT)
#include <GL/glut.h>


/*****************************************************************************
 * The GLUTWindow class is the superclass for all GLUT windows used by the
 * application. It handles the details of switching between windows, setting
 * up event-handler callbacks, and resizing.
 *****************************************************************************/
class inca::ui::GLUTWindow {
public:
    // Static functions for C++ wrapper for GLUT callbacks
    static void reshapeFunc(int width, int height);
    static void entryFunc(int state);
    static void visibilityFunc(int visible);
    static void mouseFunc(int button, int state, int x, int y);
    static void motionFunc(int x, int y);
    static void passiveMotionFunc(int x, int y);
    static void keyboardFunc(unsigned char key, int x, int y);
    static void specialFunc(int key, int x, int y);
    static void displayFunc();
    static void overlayDisplayFunc();
    static void idleFunc();

protected:
    // The list of existing GLUT windows
    static vector<GLUTWindow *> windowList;

public:
    // Constructor
    GLUTWindow(const string &title);

    // Destructor
    virtual ~GLUTWindow() { }

    // Window size functions
    void setMinimumSize(size_t width, size_t height);
    void setMaximumSize(size_t width, size_t height);
    void setSize(size_t width, size_t height);
    void setSquare(bool square);
    bool isSquare() const { return keepSquare; }

    // Window position functions
    void centerOnScreen();
    void setPosition(unsigned int x, unsigned int y);
    void setFullScreen(bool fs);
    bool isFullScreen() const { return fullScreen; }

    // GLUT window functions
    virtual void reshape(int width, int height);    // We implement this
    virtual void entry(int state);
    virtual void visibility(int visible);

    // GLUT input functions
    virtual void mouseButton(int button, int state, int x, int y);
    virtual void mouseMotion(int x, int y);
    virtual void passiveMotion(int x, int y);
    virtual void key(unsigned char k, int x, int y);
    virtual void special(int key, int x, int y);

    // GLUT display functions
    virtual void display() = 0;     // All subclasses MUST implement this
    virtual void overlayDisplay();

    // Force a redisplay
    void postRedisplay() const;

    // GLUT idle function
    virtual void idle();

protected:
    // Draw a text string into this window
    void drawString(GLint x, GLint y, const char *str);
    void drawString(GLint x, GLint y, const string &str) {
        drawString(x, y, str.c_str());
    }

    unsigned int windowID;				// The GLUT window ID

    unsigned int windowX, windowY,		// The position of the window
				 height, width,			// The window's current size
				 minHeight, minWidth,   // The window's minimum size
				 maxHeight, maxWidth,   // The window's maximum size
				 restoreHeight, restoreWidth;   // How big to restore to
    bool keepSquare;            // Should this window be kept square?
    bool fullScreen;            // Is this window full-screened?
};

#endif
