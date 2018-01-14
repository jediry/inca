/** -*- C++ -*-
 *
 * \file    WindowControlWidget.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The WindowControlWidget class is a PassThruWidget subclass that
 *      provides key-press handlers for manipulating the Window it is embedded
 *      within. Any other events are passed on to the wrapped Widget.
 */

#pragma once
#ifndef INCA_UI_APPLICATION_CONTROL_WIDGET
#define INCA_UI_APPLICATION_CONTROL_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca user-interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class WindowControlWidget;

        // Pointer typedefs
        typedef shared_ptr<WindowControlWidget>       WindowControlWidgetPtr;
        typedef shared_ptr<WindowControlWidget const> WindowControlWidgetConstPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"


class inca::ui::WindowControlWidget : public inca::ui::PassThruWidget {
/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Constructor with optional component name
    explicit WindowControlWidget(const std::string & nm = std::string());


/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
public:
    // Key events
    void keyPressed(const KeyEvent & e);
};

#endif
