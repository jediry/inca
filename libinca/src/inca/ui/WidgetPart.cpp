/*
 * File: WidgetPart.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "WidgetPart.hpp"
using namespace inca::ui;

typedef WidgetPart::Renderer    Renderer;
typedef WidgetPart::RendererPtr RendererPtr;


// Event-handler functions for reacting to how the container is
// treating this WidgetPart.
void WidgetPart::acquired(WidgetPartContainerPtr wpc) {
    // Make sure we weren't acquired by NULL
    if (! wpc) {
        INCA_WARNING("WidgetPart[" << name() << "] acquired by NULL?? Ignoring")
        return;
    }

    // Warn the user if things aren't quite kosher
    if (! _parent.expired()) {
        WidgetPartContainerPtr pp = parent();
        INCA_WARNING("WidgetPart[" << name() << "] acquired by "
                     << wpc->name() << " without being released by "
                     << pp->name() << " first.")
    }

    // Set the new parent and assume "active"
    _parent = wpc;
    _active = true;
    INCA_INFO("WidgetPart[" << name() << "] acquired by " << wpc->name())
}
void WidgetPart::released() {
    // Make sure a WidgetPart with no parent is not "released"
    if (_parent.expired()) {
        INCA_WARNING("WidgetPart[" << name() << "] released, but has no "
                     "parent...ignoring.")
        return;
    }

    // Unset the parent and become inactive
    _parent.reset();
    _active = false;
}
void WidgetPart::activated() {
    // Make sure a WidgetPart with no parent is not "activated"
    if (_parent.expired()) {
        INCA_WARNING("WidgetPart[" << name() << "] activated, but has no "
                     "parent...ignoring.")
        return;
    }

    // If we're already active, then why bother?
    if (_active) {
        INCA_WARNING("WidgetPart[" << name() << "] activated, "
                     "but was already so.")
        return;
    }

    // OK...life is good, make the change
    INCA_INFO("WidgetPart[" << name() << "] activated")
    _active = true;
}
void WidgetPart::suspended() {
    // Make sure a WidgetPart with no parent is not "suspended"
    if (_parent.expired()) {
        INCA_WARNING("WidgetPart[" << name() << "] suspended, but has no "
                     "parent...ignoring.")
        return;
    }

    // If we're already suspended, then why bother?
    if (! _active) {
        INCA_WARNING("WidgetPart[" << name() << "] suspended, "
                     "but was already so.")
        return;
    }

    // OK...life is good, make the change
    INCA_INFO("WidgetPart[" << name() << "] suspended")
    _active = false;
}


// Default implementation of WidgetPart function to request a redisplay from
// any WidgetPartContainers who are currently containing it (in an interested
// fashion).
void WidgetPart::requestRedisplay() const {
    // If we're active (have a parent and not suspended), pass call up
    if (active()) {
        WidgetPartContainerPtr pp = parent();
        pp->redisplay(self());
    }
}

// Ask our parent who the ultimate containing Window is
WindowPtr WidgetPart::getContainingWindow() const {
    WindowPtr win;  // Start with NULL as our answer

    // If we have a parent, ask him who the Window is
    WidgetPartContainerPtr myDaddy = parent();
    if (myDaddy)    win = myDaddy->getContainingWindow();

    return win;
}

// Who is my daddy?
WidgetPartContainerPtr WidgetPart::parent() const { return _parent.lock(); }

// Are we active within our parent?
bool WidgetPart::active() const { return ! _parent.expired() && _active; }


// Renderer object accessors
      Renderer & WidgetPart::renderer()       { return *_renderer; }
const Renderer & WidgetPart::renderer() const { return *_renderer; }
void WidgetPart::setRenderer(RendererPtr r) { _renderer = r; }
