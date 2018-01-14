/** -*- C++ -*-
 *
 * \file    WindowControlWidget.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the Window control declared in
 *      WindowControlWidget.hpp.
 */

// Import class definition
#include "WindowControlWidget.hpp"

// Import Application and Window classes
#include <inca/ui/Application.hpp>
#include <inca/ui/Window.hpp>

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
// Constructor with optional component name
WindowControlWidget::WindowControlWidget(const std::string & nm)
        : PassThruWidget(nm) { }


/*---------------------------------------------------------------------------*
 | Event-handling functions
 *---------------------------------------------------------------------------*/
void WindowControlWidget::keyPressed(const KeyEvent & e) {
    switch (e.keyCode()) {
    case KeyEscape:
        inca::ui::Application::instance().exit(0, "User pressed ESC");
        break;
    case KeyF11:
        {
            WindowPtr w = static_pointer_cast<Window>(surface()->parent());
            w->toggleFullScreen();
        }
        break;
    default:
        PassThruWidget::keyPressed(e);
        break;
    }
}
