/*
 * File: Application.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "Application.hpp"

// Import exception definitions
#include <inca/util/IllegalStateException.hpp>

// Import toolkit-specialized peer definitions
#include <inca/integration/glut/GLUT-peers.hpp>

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Singleton instance access
 *---------------------------------------------------------------------------*/
// The singleton Application instance
Application * Application::_instance;

// Query singleton existence
bool Application::instanceExists() { return _instance != NULL; }

// Return the singleton instance
Application & Application::instance() { return *_instance; }

// Destroy the singleton instance (may have "consequences")
void Application::destroy() {
    if (instanceExists()) {
        delete _instance;
        _instance = NULL;
    }
}


/*---------------------------------------------------------------------------*
 | Constructors, destructor & main function
 *---------------------------------------------------------------------------*/
Application::Application() : HeavyweightComponent<ApplicationPeer>("Application") {
    if (! instanceExists()) {
        _instance = this;   // Claim singleton-ness
    } else {
        IllegalStateException e;
        e << __FUNCTION__ "(): An application instance already exists";
        throw e;
    }
}

Application::~Application() { }


// Application main function
int Application::main(int & argc, char **& argv) {
    // First, initialize the UI toolkit
    setPeer(new GLUTApplicationPeer(this));
    peer()->initialize(argc, argv);

    // Next, we get our own application-specific command-line arguments
    // and do any necessary initialization
    setup(argc, argv);

    // Now, assemble the user interface
    construct();
    
    // Finally, run the application loop
    return peer()->run();
}


// Application exit function
void Application::exit(int code, const std::string & msg) {
    if (code != 0)      INCA_FATAL(msg)
    else                INCA_INFO(msg)
    peer()->exit(code);
}


/*---------------------------------------------------------------------------*
 | Input device state functions
 *---------------------------------------------------------------------------*/
// NumLock key state
bool Application::numLockActive() const {
    return peer()->lockKeyState(NumLockKey);
}
void Application::setNumLockActive(bool a) {
    peer()->setLockKeyState(NumLockKey, a);
}
void Application::toggleNumLock() {
    peer()->setLockKeyState(NumLockKey, ! numLockActive());
}

// ScrollLock key state
bool Application::scrollLockActive() const {
    return peer()->lockKeyState(ScrollLockKey);
}
void Application::setScrollLockActive(bool a) {
    peer()->setLockKeyState(ScrollLockKey, a);
}
void Application::toggleScrollLock() {
    peer()->setLockKeyState(ScrollLockKey, ! scrollLockActive());
}

// CapsLock key state
bool Application::capsLockActive() const {
    return peer()->lockKeyState(CapsLockKey);
}
void Application::setCapsLockActive(bool a) {
    peer()->setLockKeyState(CapsLockKey, a);
}
void Application::toggleCapsLock() {
    peer()->setLockKeyState(CapsLockKey, ! capsLockActive());
}


/*---------------------------------------------------------------------------*
 | Utility functions
 *---------------------------------------------------------------------------*/
// Pull off the first command-line argument and remove it from the list
std::string Application::shift(int & argc, char **& argv) {
    std::string result;
    if (argc > 1) {
        result = argv[1];   // Save the first argument into a string
        for (IndexType i = 1; i < argc; ++i)  // Shuffle pointers for other args
            argv[i] = argv[i+1];
        argc--;             // Decrement the count
    }
    return result;
}


/*---------------------------------------------------------------------------*
 | Window management functions
 *---------------------------------------------------------------------------*/
// Add a Window to the Application
WindowPtr Application::add(Window * w) {
    WindowPtr wp(w);    // Wrap it in a WindowPtr and call the other version
    return add(wp);
}
WindowPtr Application::add(WindowPtr w) {
    w->setPeer(new GLUTWindowPeer(w.get()));    // Give it an appropriate peer
//    w->setParent(self<Component>());
    w->construct();                             // Run 2nd phase init
    _windows.push_back(w);                      // Include it in the list
    return w;                                   // Pass it back
}

// Remove a Window from the Application
void Application::remove(WindowPtr w) {
    WindowList::iterator it;
    for (it = _windows.begin(); it != _windows.end(); ++it)
        if (w == *it) {
            _windows.erase(it);
            break;
        }
}
