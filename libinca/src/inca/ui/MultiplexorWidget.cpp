/*
 * File: MultiplexorWidget.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The MultiplexorWidget class acts as a simple "meta-widget", allowing
 *      an arbitrary number of widgets to share the viewport, much as the
 *      UNIX console manager "screen" does with TTY sessions. The user can
 *      cycle through all of the available widgets in sequence, or can jump
 *      directly to a specific widget.
 */

// Import class definition
#include "MultiplexorWidget.hpp"
using namespace inca::ui;


// Import specialized wrapping iterator
#include <inca/util/wraparound_iterator>
using namespace inca;
typedef wraparound_iterator<MultiplexorWidget::WidgetList::iterator> wrap_iter;
typedef wraparound_iterator<MultiplexorWidget::WidgetList::reverse_iterator> reverse_wrap_iter;


// Add a new widget to the list
void MultiplexorWidget::addWidget(WidgetPtr w) {
    if (! w)    // Oh NO you don't!
        return;

    // First, let's go hunt for an empty slot in the array
    index_t slot;
    for (slot = 0; slot < widgets.size(); ++slot)
        if (widgets[slot] == NULL)
            break;

    if (slot < widgets.size())      // There was an empty slot
        widgets[slot] = w;
    else                            // Have to add it to the end
        widgets.push_back(w);

    if (widget == NULL)     // ...there is no selected widget
        selectWidget(slot);     // So select this one
}

// Remove a widget from the list (true means we were successful)
bool MultiplexorWidget::removeWidget(WidgetPtr w) {
    // Try to find the first occurance of this Widget
    for (index_t pos = 0; pos < widgets.size(); pos++)
        if (widgets[pos] == w)      // OK...we found it
            return removeWidget(pos);   // So remove it
    return false;   // If we got here, we never found it
}

// Remove a widget at the speficied location (true means success)
bool MultiplexorWidget::removeWidget(index_t index) {
    // Make sure there's a valid Widget at that spot
    if (index >= 0 && index < widgets.size() && widgets[index] != NULL) {
        widgets[index] = WidgetPtr();   // Set it to NULL
        return true;                    // Success!
    } else
        return false;                   // I'm sorry, master.
}

// Step backward in the Widget list
void MultiplexorWidget::selectPreviousWidget() {
    // Iterate backwards through every slot in the vector, starting with the current
    reverse_wrap_iter i(selectedIndex, widgets.rbegin(), widgets.rend());
    ++i;
    while (i != widgets.rend()) {
        if (*i != NULL) {   // If this is non-NULL, Bob's your uncle
            selectWidget(i.current().base() - widgets.begin());
            return;             // All done.
        }
        ++i;    // Otherwise, keep looking
    }
}

// Step forward in the Widget list
void MultiplexorWidget::selectNextWidget() {
    // Iterate through every slot in the vector, starting with the current
    wrap_iter i(selectedIndex, widgets.begin(), widgets.end());
    ++i;
    while (i != widgets.end()) {
        if (*i != NULL) {   // If this is non-NULL, Bob's your uncle
            selectWidget(i.current() - widgets.begin());
            return;             // All done.
        }
        ++i;    // Otherwise, keep looking
    }
}

// Jump straight to a widget
void MultiplexorWidget::selectWidget(index_t index) {
    // Make sure this is sane
    if (index >= 0 && index < widgets.size()) {
        WidgetPtr w = widgets[index];
        if (w != NULL) {        // ...then this slot has a valid Widget
            widget = w;             // The superclass's "widget" property will
            selectedIndex = index;  // take care of notifying the new widgets
            requestRedisplay();
            cerr << "MUX[" << selectedIndex << "]: " << w->name() << endl;
        }
    }
}

