/*
 * File: Widget.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_WIDGET
#define INCA_UI_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Widget;

        // Pointer typedefs
        typedef shared_ptr<Widget>       WidgetPtr;
        typedef shared_ptr<Widget const> WidgetConstPtr;
    };
};

// Import superclass definitions
#include "View.hpp"
#include "Control.hpp"


class inca::ui::Widget : virtual public View,
                         virtual public Control {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(Widget);

protected:
    // Default constructor
    explicit Widget() { }

    // Non-public constructor with component name
    explicit Widget(const string &nm) { name = nm; }
};

#endif
