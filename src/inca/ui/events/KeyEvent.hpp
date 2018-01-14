/** -*- C++ -*-
 *
 * \file    KeyEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The KeyEvent class represents a key press or release event arising
 *      from the operating system. The types of KeyEvents that can be generated
 *      are:
 *          KeyPressed      the user pushed down the key
 *          KeyReleased     the user let go of the key
 *          KeyTyped        the user pressed then released the key
 *
 *      The KeyListener class is the corresponding listener interface, and
 *      should be implemented by any class wishing to receive KeyEvents.
 *
 *      The KeyAdapter class provides default (no-op) implementations of
 *      the KeyListener interface functions.
 */

#pragma once
#ifndef INCA_UI_KEY_EVENT
#define INCA_UI_KEY_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class KeyEvent;
        class KeyListener;
        class KeyAdapter;

        // Pointer typedefs
        typedef shared_ptr<KeyListener>       KeyListenerPtr;
        typedef shared_ptr<KeyListener const> KeyListenerConstPtr;

        // Keypress event definitions
        enum KeyCode {
            // When no other key will do...
            InvalidKey = 0,

            // Letter Keys
            KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH,
            KeyI, KeyJ, KeyK, KeyL, KeyM, KeyN, KeyO, KeyP, KeyQ,
            KeyR, KeyS, KeyT, KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,

            // Number Keys
            Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,

            // Symbol Keys
            KeyMinus, KeyPlus, KeyTilde, KeyQuote, KeyColon,
            KeyBackwardSlash, KeyForwardSlash,
            KeyLessThan, KeyGreaterThan, KeyLeftBracket, KeyRightBracket,

            // Navigation Keys
            KeyUp, KeyDown, KeyLeft, KeyRight, KeyHome, KeyEnd,
            KeyPageUp, KeyPageDown,

            // Editing Keys
            KeyInsert, KeyDelete, KeyBackspace, 
            KeyTab, KeySpace, KeyEnter,

            // Control Keys
            KeyEscape, KeyShift, KeyControl, KeyAlt, KeyMeta,
            KeyCapsLock, KeyNumLock, KeyScrollLock,
            KeyBreak, KeyPrintScreen, KeyWin, KeyPopup,

            // Function Keys
            KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6,
            KeyF7, KeyF8, KeyF9, KeyF10, KeyF11, KeyF12,
        };

        enum LockKey {
            NoLockKeys    = 0x0000, // Bit-flag values for currently active
            NumLockKey    = 0x0020, // *Lock keys.
            ScrollLockKey = 0x0040,
            CapsLockKey   = 0x0080,
            AllLockKeys   = 0x00E0,
        };

    };
};

// Import superclass definition
#include "InputEvent.hpp"


class inca::ui::KeyEvent : public inca::ui::InputEvent {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Event types that can be indicated by this object
    enum EventType {
        KeyPressed,     // The user pressed a key down
        KeyReleased,    // The user released a key
        KeyTyped,       // The user pressed and released a key
    };
    
    // The type representing a character
    typedef unsigned char Character;


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Constructor
    explicit KeyEvent(Timestamp timestamp, ControlFlags flags, Pixel coords,
                      EventType type, KeyCode code, Character ch)
        : InputEvent(timestamp, flags, coords),
          _type(type), _keyCode(code), _character(ch) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    EventType type()      const { return _type; }
    KeyCode   keyCode()   const { return _keyCode; }
    Character character() const { return _character; }
    
protected:
    EventType   _type;
    KeyCode     _keyCode;
    Character   _character;
};


class inca::ui::KeyListener {
public:
    virtual void keyPressed(const KeyEvent & e)  = 0;
    virtual void keyReleased(const KeyEvent & e) = 0;
    virtual void keyTyped(const KeyEvent & e)    = 0;
};


class inca::ui::KeyAdapter : public inca::ui::KeyListener {
public:
    void keyPressed(const KeyEvent & e) { }
    void keyReleased(const KeyEvent & e) { }
    void keyTyped(const KeyEvent & e) { }
};

#endif
