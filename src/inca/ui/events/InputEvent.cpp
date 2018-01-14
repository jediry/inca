/** -*- C++ -*-
 *
 * \file    InputEvent.cpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the functions belonging to the InputEvent class.
 */

// Import class definition
#include "InputEvent.hpp"
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
// Get all or part of the control flags
InputEvent::ControlFlags InputEvent::controlFlags() const {
    return _flags.get();
}
InputEvent::ControlFlags InputEvent::modifierKeys() const {
    return _flags.get(AllModifierKeys);
}
InputEvent::ControlFlags InputEvent::buttons() const {
    return _flags.get(AllButtons);
}

// Test that a specific flag is active
bool InputEvent::modifierKeyActive(ModifierKey k) const {
    return _flags.allSet(k);
}
bool InputEvent::buttonActive(ButtonCode b) const {
    return _flags.allSet(b);
}

// Test that *exactly* the specified flags are active
bool InputEvent::theseControlFlagsActive(InputEvent::ControlFlags f) const {
    return _flags.get() == f;
}
bool InputEvent::theseModifierKeysActive(InputEvent::ControlFlags f) const {
    return _flags.get(AllModifierKeys) == f;
}
bool InputEvent::theseButtonsActive(InputEvent::ControlFlags f) const {
    return _flags.get(AllButtons) == f;
}

// Test that *at least* the specified flags are active
bool InputEvent::allControlFlagsActive(InputEvent::ControlFlags f) const {
    return _flags.allSet(f);
}
bool InputEvent::allModifierKeysActive(InputEvent::ControlFlags f) const {
    return _flags.allSet(f & AllModifierKeys);
}
bool InputEvent::allButtonsActive(InputEvent::ControlFlags f) const {
    return _flags.allSet(f & AllButtons);
}

// Test that *any* of the specified flags are active
bool InputEvent::anyControlFlagsActive(InputEvent::ControlFlags f) const {
    return _flags.anySet(f);
}
bool InputEvent::anyModifierKeysActive(InputEvent::ControlFlags f) const {
    return _flags.anySet(f & AllModifierKeys);
}
bool InputEvent::anyButtonsActive(InputEvent::ControlFlags f) const {
    return _flags.anySet(f & AllButtons);
}

// Get a string representing the currently active flags
std::string InputEvent::controlFlagString() const {
    return '[' + buttonString() + '-' + modifierKeyString() + ']';
}
std::string InputEvent::modifierKeyString() const {
    std::string s("     ");  // Default to string of spaces

    // Fill in slots corresponding to active modifier keys
    if (_flags.allSet(ShiftModifierKey))    s[0] = 'S';
    if (_flags.allSet(ControlModifierKey))  s[1] = 'C';
    if (_flags.allSet(AltModifierKey))      s[2] = 'A';
    if (_flags.allSet(MetaModifierKey))     s[3] = 'M';
    if (_flags.allSet(WinModifierKey))      s[4] = 'W';

    return s;           // Report back to HQ
}
std::string InputEvent::buttonString() const {
    std::string s("    ");   // Default to string of spaces

    // Fill in slots corresponding to active buttons
    if (_flags.allSet(LeftButton))      s[0] = 'L';
    if (_flags.allSet(MiddleButton))    s[1] = 'M';
    if (_flags.allSet(RightButton))     s[2] = 'R';
    if (_flags.allSet(WheelUp))         s[3] = '^';
    if (_flags.allSet(WheelDown))       s[3] = 'v';

    return s;           // Report back to HQ
}
