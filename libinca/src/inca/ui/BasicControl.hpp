/*
 * File: BasicControl.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_BASIC_CONTROL
#define INCA_UI_BASIC_CONTROL

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class BasicControl;

        // Pointer typedef
        typedef shared_ptr<BasicControl>       BasicControlPtr;
        typedef shared_ptr<BasicControl const> BasicControlConstPtr;
    };
};

// Import superclass definition
#include "Control.hpp"


class inca::ui::BasicControl : virtual public Control {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(BasicControl);

public:
    // Default constructor with optional component name
    explicit BasicControl(const std::string & nm = "")
        : size(0, 0) { name = nm; }


public:
    // Control callbacks
    void keyPressed(KeyCode keycode, Pixel p) { }
    void mouseTracked(Pixel p) { }
    void mouseDragged(Pixel p) { }
    void buttonPressed(MouseButton b, Pixel p) { }
    void buttonReleased(MouseButton b, Pixel p) { }
    void buttonClicked(MouseButton b, Pixel p) { }
    
protected:
    Dimension size;
};

#endif
