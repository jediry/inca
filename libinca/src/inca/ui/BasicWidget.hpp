/*
 * File: BasicWidget.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_BASIC_WIDGET
#define INCA_UI_BASIC_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class BasicWidget;

        // Pointer typedef
        typedef shared_ptr<BasicWidget>       BasicWidgetPtr;
        typedef shared_ptr<BasicWidget const> BasicWidgetConstPtr;
    };
};

// Import superclass definition
#include "Widget.hpp"


class inca::ui::BasicWidget : public Widget {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(BasicWidget);

public:
    // Default constructor with optional component name
    explicit BasicWidget(const std::string & nm = "")
        : size(0, 0) { name = nm; }


public:
    // View callbacks
    void initializeView() { }
    void resizeView(Dimension sz) { size = sz; }
    void renderView() { }

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
