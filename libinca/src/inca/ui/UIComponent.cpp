/*
 * File: UIComponent.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition and related UI types
#include "UIComponent.hpp"
#include "Application.hpp"
using namespace inca::ui;


// Default constructor
UIComponent::UIComponent() : name(this) {
    // Tell the Application about me so I don't delete myself
    // as soon as my brand-new shared_ptr goes out of scope!
    Application::instance().registerComponent(UIComponentPtr(this));
}

