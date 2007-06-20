/** -*- C++ -*-
 *
 * \file    InputEvent.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The InputEvent class is the base class for all events indicating
 *      low-level user input (e.g., key presses, mouse movement, joystick
 *      movement, head-tracker movement, etc.), and contains the properties
 *      common to these types of events. It cannot be directly instantiated.
 *
 *      Accordingly, there is no listener interface corresponding to InputEvent.
 */

#pragma once
#ifndef INCA_UI_INPUT_EVENT
#define INCA_UI_INPUT_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class InputEvent;

        // Bitmasks for setting and retrieving modifier keys
        enum ModifierKey {          
            NoModifierKeys     = 0x0000,
            ShiftModifierKey   = 0x0001,
            ControlModifierKey = 0x0002,
            AltModifierKey     = 0x0004,
            MetaModifierKey    = 0x0008,
            WinModifierKey     = 0x0010,
            AllModifierKeys    = 0x001F,
        };

        // Bitmasks for setting and retrieving pointer device buttons
        enum ButtonCode {
            NoButtons     = 0x0000,
            LeftButton    = 0x0100,
            MiddleButton  = 0x0200,
            RightButton   = 0x0400,
            WheelUp       = 0x0800,
            WheelDown     = 0x1000,
            AllButtons    = 0x1F00,
        };

    };
};

// Import superclass definition
#include "Event.hpp"

// Import FlagSet definition
#include <inca/util/FlagSet>


class inca::ui::InputEvent : public inca::ui::Event {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    // Combination of active modifier keys and pointer buttons
    typedef unsigned short  ControlFlags;
    
    // Bit-vector type wrapping the various bit test operations
    typedef FlagSet<ControlFlags>   Flags;


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
protected:
    // Protected constructor
    explicit InputEvent(Timestamp ts, ControlFlags flags, Pixel coords)
        : Event(ts), _flags(flags), _coordinates(coords) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    // Get all or part of the control flags
    ControlFlags controlFlags() const;
    ControlFlags modifierKeys() const;
    ControlFlags buttons() const;
    
    // Test that a specific flag is active
    bool modifierKeyActive(ModifierKey k) const;
    bool buttonActive(ButtonCode b) const;
    
    // Test that *exactly* the specified flags are active
    bool theseControlFlagsActive(ControlFlags f) const;
    bool theseModifierKeysActive(ControlFlags f) const;
    bool theseButtonsActive(ControlFlags f) const;

    // Test that *at least* the specified flags are active
    bool allControlFlagsActive(ControlFlags f) const;
    bool allModifierKeysActive(ControlFlags f) const;
    bool allButtonsActive(ControlFlags f) const;
    
    // Test that *any* of the specified flags are active
    bool anyControlFlagsActive(ControlFlags f) const;
    bool anyModifierKeysActive(ControlFlags f) const;
    bool anyButtonsActive(ControlFlags f) const;
    
    // Get a string representing the currently active flags
    std::string controlFlagString() const;
    std::string modifierKeyString() const;
    std::string buttonString() const;

    // Get the pointer coordinates at which the event occurred
    Pixel coordinates() const { return _coordinates; }
    
protected:
    // The set of active input control flags
    Flags _flags;
    Pixel _coordinates;
};

#endif
