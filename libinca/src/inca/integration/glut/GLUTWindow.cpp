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

// Import GL Utilities Toolkit (GLUT)
#include <GL/glut.h>


/*---------------------------------------------------------------------------*
 | Static GLUTWindow constants and variables
 *---------------------------------------------------------------------------*/
// GLUT C++ wrapper static initialization
vector<GLUTWindow *> GLUTWindow::windowList;

// Default window parameters
const string                GLUTWindow::DEFAULT_TITLE("Inca GLUT Window");
const GLUTWindow::Pixel     GLUTWindow::DEFAULT_POSITION(50, 50);
const GLUTWindow::Dimension GLUTWindow::DEFAULT_SIZE(400, 400);
const GLUTWindow::Dimension GLUTWindow::DEFAULT_MINIMUM_SIZE(100, 100);
const GLUTWindow::Dimension GLUTWindow::DEFAULT_MAXIMUM_SIZE(800, 800);
const float                 GLUTWindow::DEFAULT_ASPECT_RATIO(0.0f);
const bool                  GLUTWindow::DEFAULT_IS_FULL_SCREEN(false);

// Button click timing
const GLUTWindow::Timer::scalar_t GLUTWindow::CLICK_DURATION(0.5f);


/*---------------------------------------------------------------------------*
 | Static GLUT callbacks -- they just call member functions of the window
 *---------------------------------------------------------------------------*/
void GLUTWindow::reshapeFunc(int width, int height) {
    cerr << "Reshaping\n";
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
 | Constructors and window instance management
 *---------------------------------------------------------------------------*/
// Default constructor
GLUTWindow::GLUTWindow(const string &title)
        : Window(title),
          position(DEFAULT_POSITION), size(DEFAULT_SIZE),
          minSize(DEFAULT_MINIMUM_SIZE), maxSize(DEFAULT_MAXIMUM_SIZE),
          aspectRatio(DEFAULT_ASPECT_RATIO),
          fullScreen(DEFAULT_IS_FULL_SCREEN), widgetInitialized(false) {

    // Get a new window, with this title
    createWindow(title);
}

// Widget-specific constructor
GLUTWindow::GLUTWindow(WidgetPtr w, const string &title)
        : Window(title),
          position(DEFAULT_POSITION), size(DEFAULT_SIZE),
          minSize(DEFAULT_MINIMUM_SIZE), maxSize(DEFAULT_MAXIMUM_SIZE),
          aspectRatio(DEFAULT_ASPECT_RATIO),
          fullScreen(DEFAULT_IS_FULL_SCREEN), widgetInitialized(false) {

    // Get a new window, with this title
    createWindow(title);
          
    // Set our shiny new widget in its place
    widget = w;
}


// Constructors delegate to this in order to create the actual window
// and to set up event handlers
void GLUTWindow::createWindow(const string &title) {
    // Create a GLUT window and add it to window management
    windowID = glutCreateWindow(title.c_str());
    windowList.resize(windowID + 1);                // Make room
    windowList[windowID] = this;                    // Claim my ID

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

// End the window's pitiful existence
GLUTWindow::~GLUTWindow() {
    // Remove us from the list of living windows
    windowList[windowID] = NULL;

    // Clean up the window instance
    glutDestroyWindow(windowID);
}


/*---------------------------------------------------------------------------*
 | GLUT-specific event-handlers
 *---------------------------------------------------------------------------*/
// GLUT window callbacks
void GLUTWindow::reshape(int w, int h) {
    size[0] = w; size[1] = h;           // Update our window dimensions
    if (widget) {
        if (! widgetInitialized) {      // We only call initializeView() once
            widget->initializeView();
            widgetInitialized = true;
        }
        widget->resizeView(size);       // Inform the widget of the change
    }

    glViewport(0, 0, size[0], size[1]); // Tell GL what we did
    requestRedisplay();                 // Now go redraw everything
}

void GLUTWindow::entry(int state) { }
void GLUTWindow::visibility(int visible) { }

// GLUT input callbacks
void GLUTWindow::mouseMotion(int x, int y) {
    // Pass through a mouse movement
    if (widget) widget->mouseDragged(Pixel(x, y));
}

void GLUTWindow::passiveMotion(int x, int y) {
    // Pass through a mouse movement
    if (widget) widget->mouseTracked(Pixel(x, y));
}

void GLUTWindow::mouseButton(int button, int state, int x, int y) {
    // Pass through a mouse button press
    if (widget) {
        widget->setModifierFlags(glutGetModifiers());
        MouseButton b = translateMouseButton(button);
        if (state == GLUT_DOWN) {
            widget->addButtonFlag(b);
            widget->buttonPressed(b, Pixel(x, y));
            buttonTimer[button].reset();
            buttonTimer[button].start();
        } else {
            widget->removeButtonFlag(b);
            widget->buttonReleased(b, Pixel(x, y));
            buttonTimer[button].stop();
            if (buttonTimer[button].time() < CLICK_DURATION)
                widget->buttonClicked(b, Pixel(x, y));
        }
    }
}

void GLUTWindow::key(unsigned char key, int x, int y) {
    // Pass through a normal (ASCII) keypress
    if (widget) {
        KeyCode k = translateNormalKey(key);
        widget->setModifierFlags(glutGetModifiers());
        widget->keyPressed(k, Pixel(x, y));
    }
}

void GLUTWindow::special(int key, int x, int y) {
    // Pass through a special (control) key
    if (widget) {
        KeyCode k = translateSpecialKey(key);
        widget->setModifierFlags(glutGetModifiers());
        widget->keyPressed(k, Pixel(x, y));
    }
}

// GLUT display callbacks
void GLUTWindow::overlayDisplay() { /* Do nothing right now */ }
void GLUTWindow::display() {
    if (widget) {
        widget->renderView();
        glutSwapBuffers();
    }
}

// GLUT idle callback
void GLUTWindow::idle() { }


/*---------------------------------------------------------------------------*
 | GLUT -> Inca event translation functions
 *---------------------------------------------------------------------------*/
// Translate a GLUT mouse button into an Inca MouseButton
MouseButton GLUTWindow::translateMouseButton(int button) {
    if (button == GLUT_LEFT_BUTTON)    return LeftButton;
    if (button == GLUT_MIDDLE_BUTTON)  return MiddleButton;
    if (button == GLUT_RIGHT_BUTTON)   return RightButton;
    if (button == 3)                   return WheelUp;
    if (button == 4)                   return WheelDown;
    return NoButtons;
}

KeyCode GLUTWindow::translateNormalKey(unsigned char key) {
    KeyCode k;

//    cerr << "key is " << int(key) << endl;

//    // Since we're handling the Control key separately, we need to remap the
//    // range '^a'..'^z' (0x01..0x1A) to 'a'..'z', a difference of 64 (0x70)
//    if (key >= 0x01 && key <= 0x1A)         // Control character
//        key |= 0x70;

    // Convert from ASCII to Inca KeyCode
    if (key >= 'A' && key <= 'Z')           // Upper case
        k = KeyCode((key - 'A') + KeyA);
    else if (key >= 'a' && key <= 'z')      // Lower case
        k = KeyCode((key - 'a') + KeyA);
    else {
        switch (key) {
        case '0': case ')':     k = Key0;               break;
        case '1': case '!':     k = Key1;               break;
        case '2': case '@':     k = Key2;               break;
        case '3': case '#':     k = Key3;               break;
        case '4': case '$':     k = Key4;               break;
        case '5': case '%':     k = Key5;               break;
        case '6': case '^':     k = Key6;               break;
        case '7': case '&':     k = Key7;               break;
        case '8': case '*':     k = Key8;               break;
        case '9': case '(':     k = Key9;               break;
        case '-': case '_':     k = KeyMinus;           break;
        case '=': case '+':     k = KeyPlus;            break;
        case '[': case '{':     k = KeyLeftBracket;     break;
        case ']': case '}':     k = KeyRightBracket;    break;
        case '\\': case '|':    k = KeyBackwardSlash;   break;
        case '/': case '?':     k = KeyForwardSlash;    break;
        case ',': case '<':     k = KeyLessThan;        break;
        case '.': case '>':     k = KeyGreaterThan;     break;
        case ';': case ':':     k = KeyColon;           break;
        case '\'': case '"':    k = KeyQuote;           break;
        case '`': case '~':     k = KeyTilde;           break;
        case 0x20:              k = KeySpace;           break;
        case 0x09:              k = KeyTab;             break;
        case 0x0D: case 0x0A:   k = KeyEnter;           break;
        case 0x08:              k = KeyBackspace;       break;
        case 0x7F:              k = KeyDelete;          break;
        case 0x1B:              k = KeyEscape;          break;
        default:
            logger << "translateNormalKey('" << key << "', 0x"
                   << std::hex << int(key) << std::dec
                   << "): Unrecognized keycode";
            logger.warning();
            k = InvalidKey;
        }
    }

//    cerr << "Convert " << (glutGetModifiers() & ShiftModifier ? 'S' : ' ')
//                       << (glutGetModifiers() & ControlModifier ? 'C' : ' ')
//                       << (glutGetModifiers() & AltModifier ? 'C' : ' ')
//                       << int(key) << " -> " << int(k) << endl;

    return k;
}

KeyCode GLUTWindow::translateSpecialKey(int key) {
    KeyCode k;
    switch (key) {
        case GLUT_KEY_F1:           k = KeyF1;         break;
        case GLUT_KEY_F2:           k = KeyF2;         break;
        case GLUT_KEY_F3:           k = KeyF3;         break;
        case GLUT_KEY_F4:           k = KeyF4;         break;
        case GLUT_KEY_F5:           k = KeyF5;         break;
        case GLUT_KEY_F6:           k = KeyF6;         break;
        case GLUT_KEY_F7:           k = KeyF7;         break;
        case GLUT_KEY_F8:           k = KeyF8;         break;
        case GLUT_KEY_F9:           k = KeyF9;         break;
        case GLUT_KEY_F10:          k = KeyF10;        break;
        case GLUT_KEY_F11:          k = KeyF11;        break;
        case GLUT_KEY_F12:          k = KeyF12;        break;
        case GLUT_KEY_LEFT:         k = KeyLeft;       break;
        case GLUT_KEY_RIGHT:        k = KeyRight;      break;
        case GLUT_KEY_UP:           k = KeyUp;         break;
        case GLUT_KEY_DOWN:         k = KeyDown;       break;
        case GLUT_KEY_HOME:         k = KeyHome;       break;
        case GLUT_KEY_END:          k = KeyEnd;        break;
        case GLUT_KEY_PAGE_UP:      k = KeyPageUp;     break;
        case GLUT_KEY_PAGE_DOWN:    k = KeyPageDown;   break;
        case GLUT_KEY_INSERT:       k = KeyInsert;     break;
        default:
            logger << "translateSpecialKey(" << key << ", 0x"
                   << std::hex << key << std::dec
                   << "): Unrecognized keycode";
            logger.warning();
            k = InvalidKey;
    }
//    cerr << "Convert " << int(key) << " -> " << int(k) << endl;

    return k;
}


/*---------------------------------------------------------------------------*
 | Window control functions
 *---------------------------------------------------------------------------*/
void GLUTWindow::setTitle(const string &title) {
    glutPushWindow();                   // Save the previous window
        glutSetWindow(windowID);            // Pick this window
        glutSetWindowTitle(title.c_str());  // Change the title
    glutPopWindow();                    // Restore the previous window
}

void GLUTWindow::setVisible(bool vis) {
    if (! vis && visible) {         // ...then we must make it so
        visible = false;                // Disappear!
        glutPushWindow();               // Store the previous window
            glutSetWindow(windowID);        // Pick this window
            glutHideWindow();               // Hide it
        glutPopWindow();                // Restore the previous window
    } else if (vis && ! visible) {  // ...OK...gotta put it back
        visible = true;
        glutPushWindow();               // Store the previous window
            glutSetWindow(windowID);        // Pick this window
            glutShowWindow();               // Show it
        glutPopWindow();                // Restore the previous window
    }
}

void GLUTWindow::setIconified(bool icon) {
    if (icon && ! iconified) {      // ...then we must make it so
        iconified = true;               // We're shrinking!
        restoreSize = size;             // Store the size for later
        glutPushWindow();               // Store the previous window
            glutSetWindow(windowID);        // Pick this window
            glutIconifyWindow();            // Iconify it
        glutPopWindow();                // Restore the previous window
    } else if (!icon && iconified) {// ...OK...gotta put it back
        restore();
    }
}

void GLUTWindow::setFullScreen(bool fs) {
#if (GLUT_API_VERSION >= 3) // This is only supported in later versions of GLUT
    if (fs && ! fullScreen) {       // ...then we must make it so
        fullScreen = true;              // We're going full-out
        restoreSize = size;             // Store the size for later
        cerr << "Full screen\n";
        glutPushWindow();               // Store the previous window
            glutSetWindow(windowID);        // Pick this window
            glutFullScreen();               // Full-screen-ify it
        glutPopWindow();                // Restore the previous window
    } else if (!fs && fullScreen) { // ...OK...gotta put it back
        cerr << "Un-full-screen\n";
        restore();
    }
#else
    cerr << "Full-screen mode is not supported by this version of GLUT\n";
#endif
}

void GLUTWindow::restore() {
    if (! visible) {
        setVisible(true);               // Make ourselves visible again
    } else if (fullScreen || iconified) {
        fullScreen = false;             // We're no longer fullscreen
        iconified = false;              // We're no longer iconified
        setPosition(position);          // Restore the original window position
        setSize(restoreSize);           // And restore its size
    }
}

void GLUTWindow::setPosition(Pixel p) {
    glutPushWindow();                   // Save the previous window
        glutSetWindow(windowID);            // Pick this window
        glutPositionWindow(p[0], p[1]);     // Move it
    glutPopWindow();                    // Restore the previous window
}

void GLUTWindow::setSize(Dimension d) {
    // Enforce any aspect ratio constraint
    if (aspectRatio > 0.0f)
        d[0] = static_cast<unsigned int>(d[1] * aspectRatio);

    // Clamp the width of the window
    if (d[0] > maxSize[0])      d[0] = maxSize[0];
    else if (d[0] < minSize[0]) d[0] = minSize[0];

    // Clamp the height of the window
    if (d[1] > maxSize[1])      d[1] = maxSize[1];
    else if (d[1] < minSize[1]) d[1] = minSize[1];

    // Store this as our new size
    size = d;

    glutPushWindow();                   // Save the previous window
        glutSetWindow(windowID);            // Select this window
        glutReshapeWindow(size[0], size[1]);// Resize it
    glutPopWindow();                    // Restore the previous window
}

void GLUTWindow::setMinimumSize(Dimension d) {
    // Make this dimension obey our aspect ratio constraint (if we have one)
    if (aspectRatio > 0.0f)
        d[0] = static_cast<unsigned int>(d[1] * aspectRatio);

    // Make sure the size is sane
    if (d[0] > 0 && d[0] <= maxSize[0] && d[1] > 0 && d[1] <= maxSize[1]) {
        minSize = d;

        // Reshape the window, if necessary
        if (minSize[0] > size[0] || minSize[1] > size[1])
            setSize(minSize);
    }
}

void GLUTWindow::setMaximumSize(Dimension d) {
    // Make this dimension obey our aspect ratio constraint (if we have one)
    if (aspectRatio > 0.0f)
        d[0] = static_cast<unsigned int>(d[1] * aspectRatio);

    // Make sure the size is sane
    if (d[0] >= minSize[0] && d[1] >= minSize[1]) {
        maxSize = d;

        // Reshape the window, if necessary
        if (maxSize[0] < size[0] || maxSize[1] < size[1])
            setSize(maxSize);
    }
}

void GLUTWindow::setAspectRatio(float ratio) {
    if (ratio < 0.0f)  // Negative ratios are invalid
        return; // Bail right now

    aspectRatio = ratio;    // Set our new ratio

    // If we're imposing a constraint (i.e., ratio > 0.0), enforce it
    if (aspectRatio > 0.0f) {
        // Set min/max, then make sure by trying to reset the current size
        minSize[0] = static_cast<unsigned int>(minSize[1] * aspectRatio);
        maxSize[0] = static_cast<unsigned int>(maxSize[1] * aspectRatio);
        setSize(size);              // Resize the window
    }
}

GLUTWindow::Dimension GLUTWindow::getScreenSize() const {
    Dimension d;
    d[0] = glutGet(GLUT_SCREEN_WIDTH);
    d[1] = glutGet(GLUT_SCREEN_HEIGHT);
    return d;
}

void GLUTWindow::requestRedisplay() const { glutPostRedisplay(); }

