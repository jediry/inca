/*
 * File: GLUTFramework.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The GLUTWidgetContainer class acts as a container for the generic
 *      Inca Widgets. It holds a single Widget, to which it gives the
 *      entire area.
 */

#ifndef INCA_INTERFACE_GLUT_FRAMEWORK
#define INCA_INTERFACE_GLUT_FRAMEWORK

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class GLUTApplication;
        class GLUTWidgetContainer;
    };
};

// Import superclass definitions
#include <inca/ui/Framework.hpp>
#include "GLUTWindow.hpp"

// Import Timer definition
#include <inca/util/Timer>


/*****************************************************************************
 * GLUT-based Application
 *****************************************************************************/
class inca::ui::GLUTApplication : virtual public Application {
public:
    virtual void initializeToolkit(int &argc, char **argv) {
        //Initialize GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    }

    virtual int run() {
        glutMainLoop();
        return 0;       // Never actually returns
    }

    virtual void exit(int status, const string &msg) {
        cerr << msg << endl;
        ::exit(status);
    }
};


/*****************************************************************************
 * GLUT-based WidgetContainer
 *****************************************************************************/
class inca::ui::GLUTWidgetContainer : public GLUTWindow,
                                     public WidgetContainer {
public:
    // Typedef for the timers we'll use for buttons
    typedef Timer<float, false> Timer;  // float resolution, no events

    // How many mouse buttons do we support
    static const size_t SUPPORTED_BUTTONS = 10;

    // How long can a click be (in seconds)
    const Timer::scalar_t CLICK_DURATION;


/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Default initializer
    GLUTWidgetContainer(const string &title)
        : GLUTWindow(title), initialized(false), CLICK_DURATION(0.5f) { }


/*---------------------------------------------------------------------------*
 | GLUT rendering functions
 *---------------------------------------------------------------------------*/
public:
    void reshape(int width, int height) {
        if (view != NULL) {
            if (! initialized) {
                view->initializeView();
                initialized = true;
            }
            view->resizeView(width, height);
        }
    }

    void display() {
        if (view != NULL) {
            view->renderView();
            glutSwapBuffers();
        }
    }

protected:
    bool initialized;


/*---------------------------------------------------------------------------*
 | GLUT event-handlers
 *---------------------------------------------------------------------------*/
protected:
    // Pass through a keypress
    void key(unsigned char key, int x, int y) {
        if (control != NULL) {
            KeyCode k;

            if (key >= 'A' && key <= 'Z')           // Upper case
                k = KeyCode((key - 'A') + KEY_A);
            else if (key >= 'a' && key <= 'z')      // Lower case
                k = KeyCode((key - 'a') + KEY_A);
            else {
                switch (key) {
                case '0': case ')':     k = KEY_0;              break;
                case '1': case '!':     k = KEY_1;              break;
                case '2': case '@':     k = KEY_2;              break;
                case '3': case '#':     k = KEY_3;              break;
                case '4': case '$':     k = KEY_4;              break;
                case '5': case '%':     k = KEY_5;              break;
                case '6': case '^':     k = KEY_6;              break;
                case '7': case '&':     k = KEY_7;              break;
                case '8': case '*':     k = KEY_8;              break;
                case '9': case '(':     k = KEY_9;              break;
                case '-': case '_':     k = KEY_MINUS;          break;
                case '=': case '+':     k = KEY_PLUS;           break;
                case '[': case '{':     k = KEY_LEFT_BRACKET;   break;
                case ']': case '}':     k = KEY_RIGHT_BRACKET;  break;
                case '\\': case '|':    k = KEY_BACKWARD_SLASH; break;
                case '/': case '?':     k = KEY_FORWARD_SLASH;  break;
                case ',': case '<':     k = KEY_LESS_THAN;      break;
                case '.': case '>':     k = KEY_GREATER_THAN;   break;
                case ';': case ':':     k = KEY_COLON;          break;
                case '\'': case '"':    k = KEY_QUOTE;          break;
                case '`': case '~':     k = KEY_TILDE;          break;
                case 0x20:              k = KEY_SPACE;          break;
                case 0x09:              k = KEY_TAB;            break;
                case 0x0D: case 0x0A:   k = KEY_ENTER;          break;
                case 0x08:              k = KEY_BACKSPACE;      break;
                case 0x7F:              k = KEY_DELETE;         break;
                case 0x1B:              k = KEY_ESCAPE;         break;
                default:
                    cerr << "GLUTKeyboardFunc(): Unrecognized keycode "
                         << std::hex << int(key) << std::dec << '\n';
                    return;             /*** WARNING: Early return: ***/
                                        // DON'T call the Control's handler
                }
            }

            // OK! We got thru, so call the handler
            control->setModifierFlags(glutGetModifiers());
            control->keyPressed(k, x, y);
        }
    }
    
    // Pass through a special (control) key
    void special(int key, int x, int y) {
        if (control != NULL) {
            KeyCode k;
            switch (key) {
                case GLUT_KEY_F1:           k = KEY_F1;         break;
                case GLUT_KEY_F2:           k = KEY_F2;         break;
                case GLUT_KEY_F3:           k = KEY_F3;         break;
                case GLUT_KEY_F4:           k = KEY_F4;         break;
                case GLUT_KEY_F5:           k = KEY_F5;         break;
                case GLUT_KEY_F6:           k = KEY_F6;         break;
                case GLUT_KEY_F7:           k = KEY_F7;         break;
                case GLUT_KEY_F8:           k = KEY_F8;         break;
                case GLUT_KEY_F9:           k = KEY_F9;         break;
                case GLUT_KEY_F10:          k = KEY_F10;        break;
                case GLUT_KEY_F11:          k = KEY_F11;        break;
                case GLUT_KEY_F12:          k = KEY_F12;        break;
                case GLUT_KEY_LEFT:         k = KEY_LEFT;       break;
                case GLUT_KEY_RIGHT:        k = KEY_RIGHT;      break;
                case GLUT_KEY_UP:           k = KEY_UP;         break;
                case GLUT_KEY_DOWN:         k = KEY_DOWN;       break;
                case GLUT_KEY_HOME:         k = KEY_HOME;       break;
                case GLUT_KEY_END:          k = KEY_END;        break;
                case GLUT_KEY_PAGE_UP:      k = KEY_PAGE_UP;    break;
                case GLUT_KEY_PAGE_DOWN:    k = KEY_PAGE_DOWN;  break;
                case GLUT_KEY_INSERT:       k = KEY_INSERT;     break;
                default:
                    cerr << "GLUTSpecialFunc(): Unrecognized keycode "
                         << std::hex << key << std::dec << '\n';
                    return;             /*** WARNING: Early return: ***/
                                        // DON'T call the Control's handler
            }

            // OK! We got thru, so call the handler
            control->setModifierFlags(glutGetModifiers());
            control->keyPressed(k, x, y);
        }
    }

    // Pass through a mouse button press
    void mouseButton(int b, int state, int x, int y) {
        if (control != NULL) {
            control->setModifierFlags(glutGetModifiers());
            MouseButton button = translateMouseButton(b);
            if (state == GLUT_DOWN) {
                control->addButtonFlag(button);
                control->buttonPressed(button, x, y);
                buttonTimer[b].reset();
                buttonTimer[b].start();
            } else {
                control->removeButtonFlag(button);
                control->buttonReleased(button, x, y);
                buttonTimer[b].stop();
                if (buttonTimer[b].time() < CLICK_DURATION)
                    control->buttonClicked(button, x, y);
            }
        }
    }

    // Pass through a mouse movement
    void mouseMotion(int x, int y) {
        if (control != NULL)
            control->mouseDragged(x, y);
    }

    void passiveMotion(int x, int y) {
        if (control != NULL)
            control->mouseTracked(x, y);
    }

    // Translate a GLUT mouse button into an Inca MouseButton
    MouseButton translateMouseButton(int button) {
        if (button == GLUT_LEFT_BUTTON)    return LEFT_BUTTON;
        if (button == GLUT_MIDDLE_BUTTON)  return MIDDLE_BUTTON;
        if (button == GLUT_RIGHT_BUTTON)   return RIGHT_BUTTON;
        if (button == 3)                   return WHEEL_UP;
        if (button == 4)                   return WHEEL_DOWN;
        return NO_BUTTON;
    }

    // Click timers, to determine whether to send the 'buttonClicked' event
    Timer buttonTimer[SUPPORTED_BUTTONS];

    void idle() {
        Application::instance().timer().updateTimer();
    }


/*---------------------------------------------------------------------------*
 | WidgetContainer-specified functions
 *---------------------------------------------------------------------------*/
public:
    void requestRedisplay(const View *v) { postRedisplay(); }
    void setFullScreen(bool fs) { GLUTWindow::setFullScreen(fs); }
    bool isFullScreen() const { return GLUTWindow::isFullScreen(); }

    void drawText(int x, int y, const string &text) { drawString(x, y, text); }
};

#endif
