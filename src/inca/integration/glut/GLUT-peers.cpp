/** -*- C++ -*-
 *
 * \file    GLUT-peers.cpp
 *
 * \author  Ryan L. Saunders <rls@tamu.edu>
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains the implementations of the Peer classes for
 *      interfacing the Inca UI framework with the GL Utilities Toolkit (GLUT).
 */
 
// TODO: how do we handle pointer icons (e.g., NONE) and locked-to-window pointer
// tracking in a TK-neutral manner?


// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "GLUT-peers.hpp"

// Import OpenGL & GLUT
#define GL_HPP_IMPORT_GLUT
#include <inca/integration/opengl/GL.hpp>

// Import exception classes
#include <inca/util/UnsupportedOperationException.hpp>
#include <inca/util/IllegalStateException.hpp>

using namespace inca;
using namespace inca::ui;
using namespace GL;

            
/*---------------------------------------------------------------------------*
 | Interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
//void GLUTApplicationPeer::timerFunc() { }
//void GLUTApplicationPeer::idleFunc() { }


/*---------------------------------------------------------------------------*
 | GLUTApplicationPeer constructor & destructor
 *---------------------------------------------------------------------------*/
// Constructor
GLUTApplicationPeer::GLUTApplicationPeer(Application * a)
    : ApplicationPeer(a) { }

// Destructor
GLUTApplicationPeer::~GLUTApplicationPeer() { }


/*---------------------------------------------------------------------------*
 | ApplicationPeer interface functions
 *---------------------------------------------------------------------------*/
void GLUTApplicationPeer::initialize(int & argc, char **& argv) {
    // Process any command-line arguments
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}

int GLUTApplicationPeer::run() {
    glutMainLoop(); // This runs forever...
    return 0;       // ...so this never actually returns...
}

void GLUTApplicationPeer::exit(int code) {
    ::exit(code);   // Just call the standard library's exit
}

bool GLUTApplicationPeer::lockKeyState(LockKey k) const {
    return false;//_inputStateFlags.allSet(k);
}
void GLUTApplicationPeer::setLockKeyState(LockKey k, bool a) {
//    _inputStateFlags
}


/*---------------------------------------------------------------------------*
 | WindowPeer interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
void GLUTWindowPeer::reshapeFunc(int width, int height) {
    GLUTWindowPeer * wPeer = _peers[glutGetWindow()];
    GLUTRenderableSurfacePeer * rsPeer = wPeer->renderableSurfacePeer();
    wPeer->fireComponentResized();
    if (rsPeer)
        rsPeer->fireComponentResized();
}
void GLUTWindowPeer::visibilityFunc(int visible) {
    GLUTWindowPeer * wPeer = _peers[glutGetWindow()];
    GLUTRenderableSurfacePeer * rsPeer = wPeer->renderableSurfacePeer();
    if (visible)    wPeer->fireComponentRevealed();
    else            wPeer->fireComponentHidden();
    if (rsPeer) {
        if (visible)    rsPeer->fireComponentRevealed();
        else            rsPeer->fireComponentHidden();
    }
}


/*---------------------------------------------------------------------------*
 | GLUTWindowPeer peer management functions
 *---------------------------------------------------------------------------*/
// Register a window to receive events
void GLUTWindowPeer::registerPeer(GLUTWindowPeer * peer) {
    // Make sure the peer is pointing to something
    if (! peer->valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "(...): the peer does not manage a valid window";
        throw e;
    }

    // Add it to the list
    _peers.resize(peer->id() + 1);
    _peers[peer->id()] = peer;
    
    // Register event handlers for this window
    glutPushWindow();
        glutSetWindow(peer->id());
        
        glutReshapeFunc(reshapeFunc);
        glutVisibilityFunc(visibilityFunc);
    glutPopWindow();
}

// Deregister a window so that it can no longer receive events
void GLUTWindowPeer::deregisterPeer(GLUTWindowPeer * peer) {
    // Make sure the peer is pointing to something
    if (! peer->valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "(...): the peer does not manage a valid window";
        throw e;
    }

    // Disable all event callbacks for this window
    glutPushWindow();        
        glutSetWindow(peer->id());
        
        glutReshapeFunc(NULL);
        glutVisibilityFunc(NULL);
    glutPopWindow();

    // Remove it from the window list
    _peers[peer->id()] = NULL;
}

// Static peer object registry, indexed by GLUT window ID
GLUTWindowPeer::PeerList GLUTWindowPeer::_peers;


/*---------------------------------------------------------------------------*
 | GLUTWindowPeer constructor & destructor
 *---------------------------------------------------------------------------*/
// Constructor
GLUTWindowPeer::GLUTWindowPeer(Window * w)
        : WindowPeer(w), _id(NO_WINDOW),
          _visible(true), _iconified(false), _fullScreen(false) {

    // Do static initialization the first time
    static bool initialized = false;
    if (! initialized) {
        initialized = true;

        // Initialize peer list (slot 0 is always empty, since GLUT doesn't
        // issue this as a window ID)
        _peers.resize(1, NULL);
    }
}

// Destructor
GLUTWindowPeer::~GLUTWindowPeer() {
    if (valid())    // Make sure we clean up the window
        destroy();
}

// Return a pointer to the GLUTRenderableSurfacePeer owned by this W
GLUTRenderableSurfacePeer * GLUTWindowPeer::renderableSurfacePeer() {
    if (id() == NO_WINDOW) {
        INCA_WARNING(__FUNCTION__ << "() called with no associated GLUT window ID")
        return NULL;
    }
    return GLUTRenderableSurfacePeer::_peers[id()];
}
GLUTRenderableSurfacePeer const * GLUTWindowPeer::renderableSurfacePeer() const {
    if (id() == NO_WINDOW) {
        INCA_WARNING(__FUNCTION__ << "() called with no associated GLUT window ID")
        return NULL;
    }
    return GLUTRenderableSurfacePeer::_peers[id()];
}

// Return the ID of the currently managed native object
IDType GLUTWindowPeer::id() const {
    return _id;
}

// ID sentinel value indicating no window currently managed
const IDType GLUTWindowPeer::NO_WINDOW = 0;


/*---------------------------------------------------------------------------*
 | GLUTWindowPeer ComponentPeer interface functions
 *---------------------------------------------------------------------------*/
// Get/set window position
GLUTWindowPeer::Pixel GLUTWindowPeer::position() const {
    Pixel px;
    glutPushWindow();                   // Save the previous window
        glutSetWindow(id());                // Pick this window
        px[0] = glutGet(GLUT_WINDOW_X);     // Get its coordinates
        px[1] = glutGet(GLUT_WINDOW_Y);
    glutPopWindow();                    // Restore the previous window
    return px;
}
void GLUTWindowPeer::setPosition(Pixel px) {
    glutPushWindow();                   // Save the previous window
        glutSetWindow(id());                // Pick this window
        glutPositionWindow(px[0], px[1]);   // Move it
    glutPopWindow();                    // Restore the previous window
}

// Get/set window size
GLUTWindowPeer::Dimension GLUTWindowPeer::size() const {
    Dimension sz;
    glutPushWindow();                   // Save the previous window
        glutSetWindow(id());                // Pick this window
        sz[0] = glutGet(GLUT_WINDOW_WIDTH); // Get its dimensions
        sz[1] = glutGet(GLUT_WINDOW_HEIGHT);
    glutPopWindow();                    // Restore the previous window
    return sz;
}
void GLUTWindowPeer::setSize(Dimension sz) {
    glutPushWindow();                   // Save the previous window
        glutSetWindow(id());                // Select this window
        glutReshapeWindow(sz[0], sz[1]);    // Resize it
    glutPopWindow();                    // Restore the previous window
}

// Get/set minimum window size
GLUTWindowPeer::Dimension GLUTWindowPeer::minimumSize() const {
    return Dimension(0, 0);
}
void GLUTWindowPeer::setMinimumSize(Dimension d) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(...): Window size limits not supported by GLUT";
    throw e;
}

// Get/set maximum window size
GLUTWindowPeer::Dimension GLUTWindowPeer::maximumSize() const {
    return Dimension(0, 0);
}
void GLUTWindowPeer::setMaximumSize(Dimension d) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(...): Window size limits not supported by GLUT";
    throw e;
}

// Get/set the window visibility state
bool GLUTWindowPeer::visible() const {
    return _visible;
}
void GLUTWindowPeer::setVisible(bool vis) {
    if (! vis && _visible) {        // ...then we must make it so
        _visible = false;               // Disappear!
        glutPushWindow();               // Store the previous window
            glutSetWindow(id());            // Pick this window
            glutHideWindow();               // Hide it
        glutPopWindow();                // Restore the previous window
    } else if (vis && ! _visible) { // ...OK...gotta put it back
        _visible = true;                // Come back!
        glutPushWindow();               // Store the previous window
            glutSetWindow(id());            // Pick this window
            glutShowWindow();               // Show it
        glutPopWindow();                // Restore the previous window
    }
}


/*---------------------------------------------------------------------------*
 | GLUTWindowPeer WindowPeer interface functions
 *---------------------------------------------------------------------------*/
// Create a window to manage
void GLUTWindowPeer::create() {
    // Make sure we're not shooting ourselves in the foot
    if (valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "() called, but this peer already "
             "manages a window";
        throw e;
    }
    
    // Create the window & register callback functions
    _id = glutCreateWindow("Inca GLUT Window");
    registerPeer(this);
}

// Destroy the managed window
void GLUTWindowPeer::destroy() {
    // Make sure we have something to destroy
    if (! valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "() called, but this peer is not currently "
             "managing a window";
        throw e;
    }
    
    // Destroy the window and mark this peer as invalid
    deregisterPeer(this);
    glutDestroyWindow(_id);
    _id = NO_WINDOW;

    // Reset the window state
    _title      = "";
    _visible    = true;
    _iconified  = false;
    _fullScreen = false;
}

// Query whether a native object is being managed
bool GLUTWindowPeer::valid() const {
    return _id != NO_WINDOW;
}

// Get/set the window's title
std::string GLUTWindowPeer::title() const {
    return _title;
}    
void GLUTWindowPeer::setTitle(const std::string & title) {
    glutPushWindow();                   // Save the previous window
        glutSetWindow(id());            // Pick this window
        glutSetWindowTitle(title.c_str());  // Change the title
    glutPopWindow();                    // Restore the previous window
    _title = title;
}

// Get/set the window iconification state
bool GLUTWindowPeer::iconified() const {
    return _iconified;
}
void GLUTWindowPeer::setIconified(bool icon) {
    if (icon && ! _iconified) {     // ...then we must make it so
        _iconified = true;              // We're shrinking!
        glutPushWindow();               // Store the previous window
            glutSetWindow(id());            // Pick this window
            _restoreToPosition = position();// Save its current position...
            _restoreToSize     = size();    // ...and size
            glutIconifyWindow();            // Iconify it
        glutPopWindow();                // Restore the previous window
    } else if (!icon && _iconified) {// ...OK...gotta put it back
        setPosition(_restoreToPosition);
        setSize(_restoreToSize);
    }
}

// Get/set the window maximization state
bool GLUTWindowPeer::maximized() const {
    return false;
}
void GLUTWindowPeer::setMaximized(bool m) {
    if (! m) {
        UnsupportedOperationException e;
        e << __FUNCTION__ << "(...): Maximization of windows not supported by GLUT";
        throw e;
    }
}

// Get/set the window full-screen state
bool GLUTWindowPeer::fullScreen() const {
    return _fullScreen;
}
void GLUTWindowPeer::setFullScreen(bool fs) {
#if (GLUT_API_VERSION >= 3) // This is only supported in later versions of GLUT
    if (fs && ! _fullScreen) {      // ...then we must make it so
        _fullScreen = true;             // We're going full-out
        glutPushWindow();               // Store the previous window
            glutSetWindow(id());            // Pick this window
            _restoreToPosition = position();// Save its current position...
            _restoreToSize     = size();    // ...and size
            glutFullScreen();               // Full-screen-ify it
        glutPopWindow();                // Restore the previous window
    } else if (!fs && _fullScreen) {// ...OK...gotta put it back
        setPosition(_restoreToPosition);
        setSize(_restoreToSize);
        _fullScreen = false;
    }
#else
    UnsupportedOperationException e;
    e << "Full-screen mode is not supported by this version of GLUT";
    throw e;
#endif
}

bool GLUTWindowPeer::resizable() const {
    return true;
}
void GLUTWindowPeer::setResizable(bool r) {
    if (! r) {
        UnsupportedOperationException e;
        e << __FUNCTION__ << "(...): non-resizable windows not supported by GLUT";
        throw e;
    }
}


/*---------------------------------------------------------------------------*
 | GLUTRenderableSurface interface to the GLUT event handling mechanism
 *---------------------------------------------------------------------------*/
void GLUTRenderableSurfacePeer::displayFunc() {
    _peers[glutGetWindow()]->component()->render();
    glutSwapBuffers();
}
void GLUTRenderableSurfacePeer::overlayDisplayFunc() {
//    _peers[glutGetWindow()]->component()->renderOverlay();
}
void GLUTRenderableSurfacePeer::keyboardFunc(unsigned char key, int x, int y) {
    _inputStateFlags.set(glutGetModifiers(), AllModifierKeys);
    _peers[glutGetWindow()]->
        fireKeyPressed(_inputStateFlags.get(), Pixel(x, y),
                       translateNormalKey(key), key);
}
void GLUTRenderableSurfacePeer::mouseFunc(int button, int state, int x, int y) {
    GLUTRenderableSurfacePeer * peer = _peers[glutGetWindow()];
    _inputStateFlags.set(glutGetModifiers(), AllModifierKeys);
    ButtonCode b = translateButton(button);
    if (state == GLUT_DOWN) {
        _inputStateFlags.include(b);
        peer->fireButtonPressed(_inputStateFlags.get(), Pixel(x, y), b);
        _buttonTimers[button].start(true);  // Restart & start the timer
    } else {
        _inputStateFlags.exclude(b);
        peer->fireButtonReleased(_inputStateFlags.get(), Pixel(x, y), b);
        _buttonTimers[button].stop();       // Stop the timer
        if (_buttonTimers[button].time() < CLICK_DURATION)
            peer->fireButtonClicked(_inputStateFlags.get(), Pixel(x, y), b);
    }
}
void GLUTRenderableSurfacePeer::motionFunc(int x, int y) {
    _peers[glutGetWindow()]->
        firePointerDragged(_inputStateFlags.get(), Pixel(x, y));
}
void GLUTRenderableSurfacePeer::passiveMotionFunc(int x, int y) {
    _peers[glutGetWindow()]->
        firePointerTracked(_inputStateFlags.get(), Pixel(x, y));
}
void GLUTRenderableSurfacePeer::entryFunc(int state) {
    if (state == GLUT_ENTERED)
        _peers[glutGetWindow()]->
            firePointerEntered(_inputStateFlags.get(), Pixel(0,0));
    else                            
        _peers[glutGetWindow()]->
            firePointerExited(_inputStateFlags.get(), Pixel(0,0));
}
void GLUTRenderableSurfacePeer::specialFunc(int key, int x, int y) {
    _inputStateFlags.set(glutGetModifiers(), AllModifierKeys);
    _peers[glutGetWindow()]->
        fireKeyPressed(_inputStateFlags.get(), Pixel(x, y),
                       translateSpecialKey(key), 0);
}
//void GLUTRenderableSurfacePeer::spaceballMotionFunc() { }
//void GLUTRenderableSurfacePeer::spaceballRotateFunc() { }
//void GLUTRenderableSurfacePeer::spaceballButtonFunc() { }
//void GLUTRenderableSurfacePeer::buttonBoxFunc() { }
//void GLUTRenderableSurfacePeer::dialsFunc() { }
//void GLUTRenderableSurfacePeer::tabletMotionFunc() { }
//void GLUTRenderableSurfacePeer::tabletButtonFunc() { }


// Translate a value recognized by glutMouseFunc to an Inca MouseButton
ButtonCode GLUTRenderableSurfacePeer::translateButton(int button) {
    if (button == GLUT_LEFT_BUTTON)    return LeftButton;
    if (button == GLUT_MIDDLE_BUTTON)  return MiddleButton;
    if (button == GLUT_RIGHT_BUTTON)   return RightButton;
    if (button == 3)                   return WheelUp;
    if (button == 4)                   return WheelDown;
    return NoButtons;
}

// Translate a value recognized by glutKeyboardFunc to an Inca KeyCode
KeyCode GLUTRenderableSurfacePeer::translateNormalKey(unsigned char key) {
    KeyCode k;

    // Since we're handling the Control key separately, we need to remap the
    // range '^a'..'^z' (0x01..0x1A) to 'a'..'z', a difference of 64 (0x70)
    if ((glutGetModifiers() & GLUT_ACTIVE_CTRL) && key >= 0x01 && key <= 0x1A)
        key += 'a' - 1;

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
            INCA_WARNING("translateNormalKey('" << key << "', 0x"
                         << std::hex << int(key) << std::dec
                         << "): Unrecognized keycode")
            k = InvalidKey;
        }
    }

//    cerr << "Convert " << (glutGetModifiers() & ShiftModifier ? 'S' : ' ')
//                       << (glutGetModifiers() & ControlModifier ? 'C' : ' ')
//                       << (glutGetModifiers() & AltModifier ? 'C' : ' ')
//                       << int(key) << " -> " << int(k) << endl;

    return k;
}

// Translate a value recognized by glutSpecialFunc to an Inca KeyCode
KeyCode GLUTRenderableSurfacePeer::translateSpecialKey(int key) {
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
            INCA_WARNING("translateSpecialKey(" << key << ", 0x"
                         << std::hex << key << std::dec
                         << "): Unrecognized keycode")
            k = InvalidKey;
    }

    return k;
}

// Register a peer to receive events
void GLUTRenderableSurfacePeer::registerPeer(GLUTRenderableSurfacePeer * peer) {
    // Make sure the peer is pointing to something
    if (! peer->valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "(...): the peer does not manage a valid window";
        throw e;
    }
    
    // Add it to the list
    _peers.resize(peer->id() + 1);
    _peers[peer->id()] = peer;
    
    // Register event handlers for this window
    glutPushWindow();
        glutSetWindow(peer->id());
        
        glutDisplayFunc(displayFunc);
//        glutOverlayDisplayFunc(overlayDisplayFunc);
        glutKeyboardFunc(keyboardFunc);
        glutMouseFunc(mouseFunc);
        glutMotionFunc(motionFunc);
        glutPassiveMotionFunc(passiveMotionFunc);
        glutEntryFunc(entryFunc);
        glutSpecialFunc(specialFunc);
//        glutSpaceballMotionFunc(spaceballMotionFunc);
//        glutSpaceballRotateFunc(spaceballRotateFunc);
//        glutSpaceballButtonFunc(spaceballButtonFunc);
//        glutButtonBoxFunc(buttonBoxFunc);
//        glutDialsFunc(dialsFunc);
//        glutTabletMotionFunc(tabletMotionFunc);
//        glutTabletButtonFunc(tabletButtonFunc);
    glutPopWindow();
}

// Deregister a peer so that it can no longer receive events
void GLUTRenderableSurfacePeer::deregisterPeer(GLUTRenderableSurfacePeer * peer) {
    // Make sure the peer is pointing to something
    if (! peer->valid()) {
        IllegalStateException e;
        e << __FUNCTION__ << "(...): the peer does not manage a valid window";
        throw e;
    }

    // Disable all event callbacks for this window
    glutPushWindow();        
        glutSetWindow(peer->id());
        
//        glutDisplayFunc(NULL);        // Not legal in GLUT v3
        glutOverlayDisplayFunc(NULL);
        glutKeyboardFunc(NULL);
        glutMouseFunc(NULL);
        glutMotionFunc(NULL);
        glutPassiveMotionFunc(NULL);
        glutEntryFunc(NULL);
        glutSpecialFunc(NULL);
        glutSpaceballMotionFunc(NULL);
        glutSpaceballRotateFunc(NULL);
        glutSpaceballButtonFunc(NULL);
        glutButtonBoxFunc(NULL);
        glutDialsFunc(NULL);
        glutTabletMotionFunc(NULL);
        glutTabletButtonFunc(NULL);
    glutPopWindow();

    // Remove it from the window list
    _peers[peer->id()] = NULL;
}

// Timers for detecting "clicks"
const float GLUTRenderableSurfacePeer::CLICK_DURATION = 0.5f;
GLUTRenderableSurfacePeer::TimerList GLUTRenderableSurfacePeer::_buttonTimers;

// Static peer object registry, indexed by GLUT window ID
GLUTRenderableSurfacePeer::PeerList GLUTRenderableSurfacePeer::_peers;

// Last known set of active modifier keys, stateful keys, and buttons
FlagSet<unsigned int> GLUTRenderableSurfacePeer::_inputStateFlags;


/*---------------------------------------------------------------------------*
 | GLUTRenderableSurfacePeer constructor & destructor
 *---------------------------------------------------------------------------*/
// Constructor
GLUTRenderableSurfacePeer::GLUTRenderableSurfacePeer(RenderableSurface * s)
        : RenderableSurfacePeer(s), _renderer(new Renderer()) {

    // Do static initialization the first time
    static bool initialized = false;
    if (! initialized) {
        initialized = true;

        // Determine device capabilities & create button timers. GLUT seems to
        // mis-report button capabilities sometimes, so we'll fudge things a bit.
        int buttonCount = glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS) + 1;
        _buttonTimers.resize(buttonCount);

        // Initialize peer list (slot 0 is always empty, since GLUT doesn't
        // issue this as a window ID)
        _peers.resize(1, NULL);
    }
}

// Destructor
GLUTRenderableSurfacePeer::~GLUTRenderableSurfacePeer() { }


// Return a pointer to the GLUTWindowPeer owning this RS
GLUTWindowPeer * GLUTRenderableSurfacePeer::windowPeer() {
    if (id() == NO_WINDOW) {
        INCA_WARNING(__FUNCTION__ << "() called with no associated GLUT window ID")
        return NULL;
    }
    return GLUTWindowPeer::_peers[id()];
}
GLUTWindowPeer const * GLUTRenderableSurfacePeer::windowPeer() const {
    if (id() == NO_WINDOW) {
        INCA_WARNING(__FUNCTION__ << "() called with no associated GLUT window ID")
        return NULL;
    }
    return GLUTWindowPeer::_peers[id()];
}

// Return the ID of the currently managed native object
IDType GLUTRenderableSurfacePeer::id() const {
    return _id;
}

// ID sentinel value indicating no window currently managed
const IDType GLUTRenderableSurfacePeer::NO_WINDOW = 0;


/*---------------------------------------------------------------------------*
 | GLUTRenderableSurfacePeer ComponentPeer interface functions
 *---------------------------------------------------------------------------*/
// The RenderableSurface is always located at (0,0)
GLUTRenderableSurfacePeer::Pixel GLUTRenderableSurfacePeer::position() const {
    return Pixel(0);
}
void GLUTRenderableSurfacePeer::setPosition(Pixel px) { }

// The RenderableSurface is the same size as the window that holds it
// (well, not really...) and cannot be resized.
GLUTRenderableSurfacePeer::Dimension GLUTRenderableSurfacePeer::size() const {
    GLUTWindowPeer const * wPeer = windowPeer();
    if (wPeer)  return wPeer->size();
    else        return Dimension(0);
}
void GLUTRenderableSurfacePeer::setSize(Dimension d) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(d): RenderableSurface is not resizable in GLUT--"
         "it must fill the whole window";
    throw e;
}

// Get/set minimum window size
GLUTRenderableSurfacePeer::Dimension GLUTRenderableSurfacePeer::minimumSize() const {
    return Dimension(0, 0);
}
void GLUTRenderableSurfacePeer::setMinimumSize(Dimension d) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(d): Window size limits not supported by GLUT";
    throw e;
}

// Get/set maximum window size
GLUTRenderableSurfacePeer::Dimension GLUTRenderableSurfacePeer::maximumSize() const {
    return Dimension(0, 0);
}
void GLUTRenderableSurfacePeer::setMaximumSize(Dimension d) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(d): Window size limits not supported by GLUT";
    throw e;
}

// The RenderableSurface is visible if the window containing it is, and cannot
// be changed independently
bool GLUTRenderableSurfacePeer::visible() const {
    GLUTWindowPeer const * wPeer = windowPeer();
    if (wPeer)  return wPeer->visible();
    else        return false;
}
void GLUTRenderableSurfacePeer::setVisible(bool vis) {
    UnsupportedOperationException e;
    e << __FUNCTION__ << "(d): RenderableSurface cannot be shown/hidden "
         "independently in GLUT--use Window::setVisible(v) instead";
    throw e;
}



/*---------------------------------------------------------------------------*
 | GLUTRenderableSurfacePeer RenderableSurfacePeer interface functions
 *---------------------------------------------------------------------------*/
void GLUTRenderableSurfacePeer::create() {
    // Get the owning WindowPeer
    WindowPtr w = static_pointer_cast<Window>(component()->parent());

    // We have the same ID as he does (since GLUT doesn't distinguish between
    // the window and the rendering context)
    _id = static_cast<GLUTWindowPeer *>(w->peer())->id();

    // Register with the application, to receive events
    registerPeer(this);
}

// Create the window & register callback functions
void GLUTRenderableSurfacePeer::destroy() { }

// Query whether a native object is being managed
bool GLUTRenderableSurfacePeer::valid() const {
    return _id != NO_WINDOW;
}


// Render  
RenderableSurfacePeer::Renderer & GLUTRenderableSurfacePeer::renderer() const {
    return *_renderer;
}

// Request that the window be re-rendered
void GLUTRenderableSurfacePeer::requestRedisplay() const {
    glutPushWindow();
        glutSetWindow(id());
        glutPostRedisplay();
    glutPopWindow();
}
