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


// Event-handler functions for reacting to how the containers are
// treating this WidgetPart.
void WidgetPart::acquiredBy(WidgetPartContainerPtr w) {
    if (w) {
        containers.insert(w);
        isActiveIn[w.get()] = true;
        logger << "WidgetPart[" << name() << "] acquired";
        logger.info();
    }
}
void WidgetPart::releasedBy(WidgetPartContainerPtr w) {
    if (w) {
        containers.erase(w);
        isActiveIn.erase(w.get());
        logger << "WidgetPart[" << name() << "] released";
        logger.info();
    }
}
void WidgetPart::suspendedBy(WidgetPartContainerPtr w) {
    if (w) {
        isActiveIn[w.get()] = false;
        logger << "WidgetPart[" << name() << "] suspended";
        logger.info();
    }
}
void WidgetPart::resumedBy(WidgetPartContainerPtr w) {
    if (w) {
        isActiveIn[w.get()] = true;
        logger << "WidgetPart[" << name() << "] resumed";
        logger.info();
    }
}


// Default implementation of WidgetPart function to request a redisplay from
// any WidgetPartContainers who are currently containing it (in an interested
// fashion).
void WidgetPart::requestRedisplay() {
    WidgetPartContainerSet::iterator i;
    for (i = containers.begin(); i != containers.end(); ++i) {
        WidgetPartContainerPtr w = *i;

        // If we are active in this container, we need to request a redraw
        if (isActiveIn[w.get()])
            w->redisplay(self());
    }
}

