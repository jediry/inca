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

#pragma once
#ifndef INCA_INTEGRATION_GLUT_WINDOW
#define INCA_INTEGRATION_GLUT_WINDOW

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class GLUTWindow;

        // Pointer typedefs
        typedef shared_ptr<GLUTWindow>       GLUTWindowPtr;
        typedef shared_ptr<GLUTWindow const> GLUTWindowConstPtr;
    };
};

// Import superclass definition
#include <inca/ui/Window.hpp>

// Import Timer definition
#include <inca/util/Timer>


/**
 * The GLUTWindow class is the superclass for all GLUT windows used by the
 * application. It handles the details of switching between windows, setting
 * up event-handler callbacks, and resizing.
 *
 * This class implements the inca::ui::Window interface and may contain an
 * Inca Widget, or it may be subclassed and used standalone, in which case
 * the subclass should override any necessary callbacks (e.g., display). As
 * long as the "widget" property inherited from Window is NULL, each of the
 * non-overridden event-handlers will do nothing.
 */
class inca::ui::GLUTWindow : public Window {
/*---------------------------------------------------------------------------*
 | Window layout defaults
 *---------------------------------------------------------------------------*/
protected:
    // Defaults for new windows (if unspecified)
    static const std::string    DEFAULT_TITLE;
    static const Pixel          DEFAULT_POSITION;
    static const Dimension      DEFAULT_SIZE;
    static const Dimension      DEFAULT_MINIMUM_SIZE;
    static const Dimension      DEFAULT_MAXIMUM_SIZE;
    static const float          DEFAULT_ASPECT_RATIO;
    static const bool           DEFAULT_IS_FULL_SCREEN;

protected:
    // Button click timing
    static const SizeType        SUPPORTED_BUTTONS = 10; // How many buttons?
    typedef Timer<float, false>  Timer;  // Float scalar, no events produced
    static const Timer::scalar_t CLICK_DURATION;    // How long can a click be?


/*---------------------------------------------------------------------------*
 | Static functions to patch thru the GLUT event-handling mechanism
 *---------------------------------------------------------------------------*/
private:
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
    static void registerWindow(GLUTWindow& window);
    static void unregisterWindow(GLUTWindow& window);


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
public:
    // Default Constructor
    GLUTWindow(const std::string & title = DEFAULT_TITLE);

    // Widget-containing Constructor
    GLUTWindow(WidgetPtr w, const std::string & title = DEFAULT_TITLE);

    // Destructor
    virtual ~GLUTWindow();

    // The window's GLUT window-id
    IDType getWindowID() const { return windowID; }

protected:
    IDType windowID;  // The ID of the GLUT window we own


/*---------------------------------------------------------------------------*
 | GLUT-specific event-handlers
 *---------------------------------------------------------------------------*/
public:
    // GLUT window callbacks
    virtual void reshape(int width, int height);
    virtual void entry(int state);
    virtual void visibility(int visible);

    // GLUT input callbacks
    virtual void mouseButton(int button, int state, int x, int y);
    virtual void mouseMotion(int x, int y);
    virtual void passiveMotion(int x, int y);
    virtual void key(unsigned char k, int x, int y);
    virtual void special(int key, int x, int y);

    // GLUT display callbacks
    virtual void display();
    virtual void overlayDisplay();

    // GLUT idle callback
    virtual void idle();

protected:
    // Click timers, to determine whether to send the 'buttonClicked' event
    Timer buttonTimer[SUPPORTED_BUTTONS];

    // Whether we've called initializeView on the Widget yet
    bool widgetInitialized;

    // Have we called the widget's construct() function yet?
    bool constructed;


/*---------------------------------------------------------------------------*
 | GLUT -> Inca event translation functions
 *---------------------------------------------------------------------------*/
protected:
    // Translate a GLUT mouse button into an Inca ButtonCode
    ButtonCode translateMouseButton(int button);

    // Translate a GLUT key into an Inca KeyCode
    KeyCode translateNormalKey(unsigned char key);
    KeyCode translateSpecialKey(int key);


/*---------------------------------------------------------------------------*
 | Window interface functions
 *---------------------------------------------------------------------------*/
public:
    // Window ID
    IDType getID() const { return windowID; }

    // Window title
    std::string getTitle() const { return title; }
    void setTitle(const std::string & title);

    // Window visibility state
    bool isVisible() const { return visible; }
    void setVisible(bool vis);

    // Window iconification state
    bool isIconified() const { return iconified; }
    void setIconified(bool icon);

    // Window full-screen state
    bool isFullScreen() const { return fullScreen; }
    void setFullScreen(bool fs);

    // Restore an iconified or full-screened window to its previous state
    void restore();

    // Window position
    Pixel getPosition() const { return position; }
    void setPosition(Pixel p);

    // Current size
    Dimension getSize() const { return size; }
    void setSize(Dimension d);

    // Minimum allowable size
    Dimension getMinimumSize() const { return minSize; }
    void setMinimumSize(Dimension d);

    // Maximum allowable size
    Dimension getMaximumSize() const { return maxSize; }
    void setMaximumSize(Dimension d);

    // Force aspect ratio (0.0 to allow unrestricted A/R)
    float getAspectRatio() const { return aspectRatio; }
    void setAspectRatio(float ratio);

    // Query screen size
    Dimension getScreenSize() const;

    // Request redisplay of the entire Window
    void requestRedisplay() const;

protected:
    std::string title;      // The current window title
    Pixel position;         // On-screen postion of the window
    Dimension size,         // How big it is
              minSize,      // How small it can be
              maxSize,      // How big it can be
              restoreSize;  // How big to restore to from full-screen mode
    float aspectRatio;      // An aspect ratio to constrain to (or 0.0 for none)
    bool fullScreen;        // Is this window full-screened?
    bool iconified;         // Is this window an icon?
    bool visible;           // Is this window visible?
};

#endif
