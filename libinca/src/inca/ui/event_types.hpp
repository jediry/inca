/*
 * File: event_types.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file declares enumerated types for representing UI events,
 *      including key and mouse button presses.
 */

#ifndef INCA_UI_EVENTS
#define INCA_UI_EVENTS

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
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
            KeyEscape, KeyShift, KeyControl, KeyAlt,
            KeyCapsLock, KeyNumLock, KeyScrollLock,
            KeyBreak, KeyPrintScreen, KeyWin, KeyPopup,

            // Function Keys
            KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6,
            KeyF7, KeyF8, KeyF9, KeyF10, KeyF11, KeyF12,
        };

        // 
        enum ModifierKey {          
            NoModifiers     = 0x0000, // Bit-flag values for
            ShiftModifier   = 0x0001, // currently-depressed modifier keys.
            ControlModifier = 0x0002, // These are used to interpret what is
            AltModifier     = 0x0004, // meant by the user's mouse clicks and
            AllModifiers    = 0x0007, // drags
        };
        enum MouseButton {
            NoButtons     = 0x0000, // Bit-flag values for currently-depressed
            LeftButton    = 0x0100, // mouse buttons. The combination of mouse
            MiddleButton  = 0x0200, // buttons and modifier keys tells us what
            RightButton   = 0x0400, // sort of action the user wants.
            WheelUp       = 0x0800,
            WheelDown     = 0x1000,
            AllButtons    = 0x1F00,
        };

        // This type is the combination of the above ModiferKey and MouseButton
        // types, and represents the bitwise-or'd combination of currently
        // active modifiers and buttons.
        typedef unsigned short ControlFlags;

        inline string buttonFlagsString(ControlFlags cf) {
            string s("    ");   // Default to string of spaces

            // Fill in slots corresponding to active buttons
            if (cf & LeftButton)        s[0] = 'L';
            if (cf & MiddleButton)      s[1] = 'M';
            if (cf & RightButton)       s[2] = 'R';
            if (cf & WheelUp)           s[3] = '^';
            if (cf & WheelDown)         s[3] = 'v';

            return s;           // Report back to HQ
        }

        inline string modifierFlagsString(ControlFlags cf) {
            string s("   ");    // Default to string of spaces

            // Fill in slots corresponding to active modifier keys
            if (cf & ShiftModifier)     s[0] = 'S';
            if (cf & ControlModifier)   s[1] = 'C';
            if (cf & AltModifier)       s[2] = 'A';

            return s;           // Report back to HQ
        }
    };
};

#endif
