/*
 * File: Framework.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file defines the classes comprising the generic Inca
 *      user-interface framework. Conceptually, an Inca user-interface is just
 *      a pretty, interactive "skin" over the real nuts and bolts: no core
 *      functionality is actually implemented in the UI code.
 *
 *      In order to simplify the writing of individual interfaces
 *      (using, for example, GLUT, GTK+, QT, or even plain, old console-mode),
 *      common functionality, such as event handling, command-line
 *      argument processing, etc. is implemented in these generic classes.
 *      Since these classes cannot be directly instantiated to make an
 *      application, there is no main() function defined here -- this will be
 *      in a toolkit-specific c++ file that ties everything together.
 *
 *      The Application interface is the base for all classes that
 *      act as the top-level class in an application. It is not itself
 *      visible, but may own and instantiate windows. It provides generic
 *      functionality for parsing command-line options and requires
 *      implementation of a constructInterface() function.
 *
 *      The View interface is the base for all classes that draw a 2D or 3D
 *      rectangular view of something, such as a portion of an image,
 *      or a camera view into an Inca Scene. A View has a height and width,
 *      which give the largest rectangular area that the View may draw to,
 *      though it need not utilize the entire area. This permits
 *      partially-transparent Views to be layered on top of one another. If
 *      the bottommost View is partially transparent, then the container should
 *      first paint the drawing area with its background color to prevent
 *      multiple frames of animation from overlapping.
 *
 *      The Control interface is the base class for generic user
 *      event-handling. Control provides no-op implementations of its
 *      event-handlers, which may be overridden in subclasses that want to
 *      receive these events. It also provides a toolkit-independent mechanism
 *      for determining the current state of keyboard modifier keys (Ctrl,
 *      Alt, Shift) and mouse buttons.
 *
 *      The Widget interface is a composite of a View and a Control, for the
 *      sake of convenience, as in most cases, a View will also want to do
 *      its own event handling.
 *
 *      The WidgetContainer interface describes the functions that must be
 *      implemented by something that wants to be able to support embedding
 *      Widgets in it. This will need to be implemented in a toolkit-specific
 *      manner.
 *
 *      The normal way of creating an Inca UI is to subclass Application
 *      and implement the required abstract functions. The main() function
 *      should look something like this:
 *          int main(int argc, char **argv) {
 *              GLUTMyApp app;
 *              app.initialize(argc, argv);
 *              app.run();
 *          }
 *
 *      and should defer any GUI object initialization until the
 *      constructInterface.
 *
 * TODO:
 *      WidgetContainer needs some thoughtful elaboration!
 */

#ifndef INCA_INTERFACE_GENERIC_FRAMEWORK
#define INCA_INTERFACE_GENERIC_FRAMEWORK

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Application;
        class Containable;
        class View;
        class Control;
        class Widget;
        class WidgetContainer;

        // GUI-event type definitions
        enum KeyCode {
            // Letter Keys
            KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
            KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q,
            KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

            // Number Keys
            KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

            // Symbol Keys
            KEY_MINUS, KEY_PLUS, KEY_TILDE, KEY_QUOTE, KEY_COLON,
            KEY_BACKWARD_SLASH, KEY_FORWARD_SLASH,
            KEY_LESS_THAN, KEY_GREATER_THAN, KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET,

            // Navigation Keys
            KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_HOME, KEY_END,
            KEY_PAGE_UP, KEY_PAGE_DOWN,

            // Editing Keys
            KEY_INSERT, KEY_DELETE, KEY_BACKSPACE, 
            KEY_TAB, KEY_SPACE, KEY_ENTER,

            // Control Keys
            KEY_ESCAPE, KEY_SHIFT, KEY_CONTROL, KEY_ALT,
            KEY_CAPS_LOCK, KEY_NUM_LOCK, KEY_SCROLL_LOCK,
            KEY_BREAK, KEY_PRINT_SCREEN, KEY_WIN, KEY_POPUP,

            // Function Keys
            KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
            KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
        };
        enum ModifierKey {          
            NO_MODIFIER   = 0x0000, // Bit-flag values for currently-depressed
            SHIFT_KEY     = 0x0001, // modifier keys. These are used to
            CTRL_KEY      = 0x0002, // interpret what is meant by the user's
            ALT_KEY       = 0x0004, // mouse clicks and drags
            ALL_MODIFIERS = 0x00FF,
        };
        enum MouseButton {
            NO_BUTTON     = 0x0000, // Bit-flag values for currently-depressed
            LEFT_BUTTON   = 0x0100, // mouse buttons. The combination of mouse
            MIDDLE_BUTTON = 0x0200, // buttons and modifier keys tells us what
            RIGHT_BUTTON  = 0x0400, // sort of action the user wants.
            WHEEL_UP      = 0x0800,
            WHEEL_DOWN    = 0x1000,
            ALL_BUTTONS   = 0xFF00,
        };
        typedef unsigned short ControlFlags; // Combination of the above
    };
};

// Import Timer definition
#include <inca/util/Timer>


/*****************************************************************************
 * The Application class represents a generic application
 *****************************************************************************/
class inca::ui::Application {
    // Set this class up to have properties
    PROPERTY_OWNING_OBJECT(Application);

public:
    // Typedef for the timer we'll use to drive animation
    typedef Timer<double, true> Timer;  // double resolution, generate events


/*---------------------------------------------------------------------------*
 | Constructor and properties
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    Application();


    // The application's main timer
    rw_property(Timer, timer, Timer());

    // Application behaves (almost) as a singleton. This returns THE instance.
    static Application & instance() { return *_instance; }

protected:
    // Singleton pointer to THE application
    static Application * _instance;


/*---------------------------------------------------------------------------*
 | Framework initialization (be sure to call this from your main()!!)
 *---------------------------------------------------------------------------*/
public:
    // Call all of the other initialization functions
    void initialize(int &argc, char **argv);


/*---------------------------------------------------------------------------*
 | Application setup functions
 *---------------------------------------------------------------------------*/
    // Do any command-line processing and app-specific initialization
    virtual void setup(int &argc, char **argv) { }

    // Construct any necessary user-interface thingies
    virtual void constructInterface() { }


/*---------------------------------------------------------------------------*
 | Toolkit-integration functions
 *---------------------------------------------------------------------------*/
public:
    // Initialize the UI-toolkit and process toolkit-specific arguments
    virtual void initializeToolkit(int &argc, char **argv) = 0;

    // Launch the application's event-handling mechanism (may not return)
    virtual int run() = 0;

    // Cause the application to terminate
    virtual void exit(int status, const string &msg);


/*---------------------------------------------------------------------------*
 | Utility functions
 *---------------------------------------------------------------------------*/
public:
    // Print out a usage message, giving available command-line options
    virtual void printUsage(ostream &o) { }
};


/*****************************************************************************
 * The Containable class handles the details of being contained
 *****************************************************************************/
class inca::ui::Containable {
protected:
    // Constructor
    Containable() : _container(NULL) { }
    
    // Destructor
    virtual ~Containable() { }

public:
    // Accessors
    WidgetContainer * container() const { return _container; }
    void setContainer(WidgetContainer * wc) { _container = wc; }
    
protected:
    WidgetContainer * _container;
};


/*****************************************************************************
 * The View class acts as a base class for a 2D or 3D view of something.
 *****************************************************************************/
class inca::ui::View : virtual public Containable {
/*---------------------------------------------------------------------------*
 | Rendering control
 *---------------------------------------------------------------------------*/
public:
    virtual void initializeView() = 0;
    virtual void resizeView(size_t w, size_t h) = 0;
    virtual void renderView() = 0;  // Subclasses must implement this

    void requestRedisplay() const;
};


/*****************************************************************************
 * The Control class acts as a base class for user-event-based
 * controls. It provides overridable toolkit-independent event-handler
 * functions, as well as facilities for handling modifier keys and mouse
 * buttons.
 *****************************************************************************/
class inca::ui::Control : virtual public Containable {
/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
protected:
    // Default initializer
    Control() : controlFlags(0x0000) { }


/*---------------------------------------------------------------------------*
 | Rendering control
 *---------------------------------------------------------------------------*/
public:
    void requestRedisplay() const;


/*---------------------------------------------------------------------------*
 | Event-handler functions: These allow event-handlers to be implemented in
 | a non-toolkit-specific manner. The application should receive user
 | events from the UI toolkit in the normal way, and pass thru the event to
 | one or more of these functions. The meaning of each event is as follows:
 |      keyPressed     -- the user pressed a key on the keyboard
 |      mouseTracked   -- the user moved the mouse without any buttons pressed
 |      mouseDragged   -- the user moved the mouse with one or more buttons
 |                        pressed
 |      buttonPressed  -- the user pressed a mouse button
 |      buttonReleased -- the user released a mouse button
 |      buttonClicked  -- the user pressed and released THE SAME button within
 |                        a short time
 | Each of these event handlers may also want to know which keyboard modifiers
 | are currently depressed. A toolkit-specific implementation should ensure
 | that the current state of the keyboard modifiers, as well as the state of
 | all buttons (including the one just pressed/released) is added to the
 | control flags before calling the event handler.
 *---------------------------------------------------------------------------*/
public:
    virtual void keyPressed(KeyCode keycode, index_t x, index_t y) { }
    virtual void mouseTracked(index_t x, index_t y) { }
    virtual void mouseDragged(index_t x, index_t y) { }
    virtual void buttonPressed(MouseButton b, index_t x, index_t y) { }
    virtual void buttonReleased(MouseButton b, index_t x, index_t y) { }
    virtual void buttonClicked(MouseButton b, index_t x, index_t y) { }


/*---------------------------------------------------------------------------*
 | Low-level event control: since a user control may want to behave
 | differently, depending on which modifer keys and/or mouse buttons are
 | currently depressed, we provide these functions for manipulating
 | 'controlFlags', which keeps track of this information.
 *---------------------------------------------------------------------------*/
public:
    // Unconditionally get/set the whole group of flags
    void setControlFlags(ControlFlags f) { controlFlags = f; }
    ControlFlags getControlFlags() const { return controlFlags; }

    // Query if flags set (returns true only if ALL specifed flags are set)
    bool flagsSet(ControlFlags f) const { return (f & !controlFlags) == 0; }

    // Get/set or incrementally modify button-related flags
    void setButtonFlags(ControlFlags f) { controlFlags = controlFlags & ~ALL_BUTTONS | f; }
    ControlFlags getButtonFlags() const { return controlFlags & ALL_BUTTONS; }
    void addButtonFlag(MouseButton b) { controlFlags |= b; }
    void removeButtonFlag(MouseButton b) { controlFlags &= ~b; }

    // Get/set or incrementally modify modifier-related flags
    void setModifierFlags(ControlFlags f) { controlFlags = controlFlags & ~ALL_MODIFIERS | f; }
    ControlFlags getModifierFlags() const { return controlFlags & ALL_MODIFIERS; }
    void addModifierFlag(ModifierKey k) { controlFlags |= k; }
    void removeModifierFlag(ModifierKey k) { controlFlags &= ~k; }

    // Get a string representation of the flags
    string printControlFlags() const;

protected:
    ControlFlags controlFlags;  // The fabled 'controlFlags'
};


/*****************************************************************************
 * The Widget class is a combination of a View and a Control. It inherits
 * both virtually, so that it may be used to combine a standalone View and
 * Control into a Widget.
 *****************************************************************************/
class inca::ui::Widget : virtual public View, virtual public Control {
public:    // XXX How should this work?
    void requestRedisplay() const {
        View::requestRedisplay();
    }
};


/*****************************************************************************
 * The WidgetContainer class specifies the functions that a toolkit-specific
 * container for generic Inca Widgets must implement.
 * FIXME: These are pretty much total hacks right now.
 *****************************************************************************/
class inca::ui::WidgetContainer {
public:
    // Constructor
    WidgetContainer() : view(NULL), control(NULL) { }

    virtual void setView(View *v) {
        view = v;
        view->setContainer(this);
    }

    virtual void setControl(Control *c) {
        control = c;
        control->setContainer(this);
    }

    virtual void setWidget(Widget *w) {
        view = w;
        control = w;
        view->setContainer(this);
        control->setContainer(this);
    }
    View * getView() const { return view; }
    Control * getControl() const { return control; }

    virtual void requestRedisplay(const View *v) = 0;
    virtual void setFullScreen(bool fs) = 0;
    virtual bool isFullScreen() const = 0;

    virtual void drawText(int x, int y, const string &text) = 0;

protected:
    View *view;
    Control *control;
};

#endif
