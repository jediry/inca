/*
 * File: Control.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_CONTROL
#define INCA_UI_CONTROL

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class Control;

        // Pointer typedefs
        typedef shared_ptr<Control>       ControlPtr;
        typedef shared_ptr<Control const> ControlConstPtr;
    };
};

// Import superclass definition
#include "WidgetPart.hpp"

// Import event enumerations
#include "event_types.hpp"


/*****************************************************************************
 * The Control class acts as a base class for user-event-based
 * controls. It provides overridable toolkit-independent event-handler
 * functions, as well as facilities for handling modifier keys and mouse
 * buttons.
 *****************************************************************************/
class inca::ui::Control : virtual public WidgetPart {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(Control);

/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
protected:
    // Non-public default constructor
    explicit Control() : controlFlags(0x0000) { }

    // Non-public constructor with component name
    explicit Control(const std::string & nm) : controlFlags(0x0000) { name = nm; }


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
    virtual void keyPressed(KeyCode keycode, Pixel p) = 0;
    virtual void mouseTracked(Pixel p) = 0;
    virtual void mouseDragged(Pixel p) = 0;
    virtual void buttonPressed(MouseButton b, Pixel p) = 0;
    virtual void buttonReleased(MouseButton b, Pixel p) = 0;
    virtual void buttonClicked(MouseButton b, Pixel p) = 0;


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

    // Get/set or individually modify button-related flags
    void setButtonFlags(ControlFlags f) { controlFlags = controlFlags & ~AllButtons | f; }
    ControlFlags getButtonFlags() const { return controlFlags & AllButtons; }
    void addButtonFlag(MouseButton b)    { controlFlags |= b; }
    void removeButtonFlag(MouseButton b) { controlFlags &= ~b; }

    // Get/set or individually modify modifier-related flags
    void setModifierFlags(ControlFlags f) { controlFlags = controlFlags & ~AllModifiers | f; }
    ControlFlags getModifierFlags() const { return controlFlags & AllModifiers; }
    void addModifierFlag(ModifierKey k) { controlFlags |= k; }
    void removeModifierFlag(ModifierKey k) { controlFlags &= ~k; }

    // Flag cluster query functions
    bool theseFlagsActive(ControlFlags f) const { return f == controlFlags; }
    bool allFlagsActive(ControlFlags f) const { return (f & ~controlFlags) == 0; }
    bool anyFlagsActive(ControlFlags f) const { return (f & controlFlags) != 0; }
    bool theseButtonsActive(ControlFlags f) const { return (f & AllButtons) == (controlFlags & AllButtons); }
    bool allButtonsActive(ControlFlags f) const { return ((f & ~controlFlags) & AllButtons) == 0; }
    bool anyButtonsActive(ControlFlags f) const { return ((f & controlFlags) & AllButtons) != 0; }
    bool theseModifiersActive(ControlFlags f) const { return (f & AllModifiers) == (controlFlags & AllModifiers); }
    bool allModifiersActive(ControlFlags f) const { return ((f & ~controlFlags) & AllModifiers) == 0; }
    bool anyModifiersActive(ControlFlags f) const { return ((f & controlFlags) & AllModifiers) != 0; }

    // Get a string representations of the active flags
    std::string printActiveFlags() const { return '[' + buttonFlagsString(controlFlags) + '-' + modifierFlagsString(controlFlags) + ']'; }
    std::string printActiveButtons() const { return '[' + buttonFlagsString(controlFlags) + ']'; }
    std::string printActiveModifiers() const { return '[' + modifierFlagsString(controlFlags) + ']'; }

protected:
    ControlFlags controlFlags;  // The fabled 'controlFlags'
};

#endif
