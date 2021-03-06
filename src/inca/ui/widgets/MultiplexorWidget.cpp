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

// TODO: deleting a widget can result in an invalid selection
// TODO: this depends on too much knowledge of PassThruWidget

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "MultiplexorWidget.hpp"
using namespace inca::ui;


// Import specialized wrapping iterator
#include <inca/util/wraparound_iterator>
using namespace inca;
typedef wraparound_iterator<MultiplexorWidget::WidgetList
                            ::const_iterator> wrap_iter;
typedef wraparound_iterator<MultiplexorWidget::WidgetList
                            ::const_reverse_iterator> reverse_wrap_iter;


/*---------------------------------------------------------------------------*
 | Search functions
 *---------------------------------------------------------------------------*/
// Return the index in the list of the first occurance of Widget 'w'
IndexType MultiplexorWidget::indexOf(WidgetConstPtr w) const {
    // Try to find the first occurance of this Widget
    for (IndexType i = 0; i < IndexType(widgets.size()); ++i)
        if (widgets[i] == w)        // OK...we found it
            return i;
    return NOT_FOUND;   // Crap.
}

// Return the index in the list of the first occurance of a Widget named 'nm'
IndexType MultiplexorWidget::indexOf(const std::string & nm) const {
    // Try to find the first occurance of this Widget
    for (IndexType i = 0; i < IndexType(widgets.size()); ++i)
        if (widgets[i] != NULL && widgets[i]->name() == nm)
            return i;   // OK...we found it
    return NOT_FOUND;   // Crap.
}

// Return the index of the previous non-NULL Widget (wrapping at the end)
IndexType MultiplexorWidget::indexBefore(IndexType from) const {
    // Iterate through every slot in the vector, starting with the current
    reverse_wrap_iter i(selectedIndex, widgets.rbegin(), widgets.rend());
    ++i;
    while (i != widgets.rend()) {
        if (*i != NULL)     // If this is non-NULL, Bob's your uncle
            return (i.current().base() - widgets.begin()); // All done.
        ++i;    // Otherwise, keep looking
    }
    return NOT_FOUND;   // Crap.
}

// Return the index of the next non-NULL Widget (wrapping at the end)
IndexType MultiplexorWidget::indexAfter(IndexType from) const {
    // Iterate through every slot in the vector, starting with the current
    wrap_iter i(selectedIndex, widgets.begin(), widgets.end());
    ++i;
    while (i != widgets.end()) {
        if (*i != NULL)     // If this is non-NULL, Bob's your uncle
            return (i.current() - widgets.begin()); // All done.
        ++i;    // Otherwise, keep looking
    }
    return NOT_FOUND;   // Crap.
}


/*---------------------------------------------------------------------------*
 | Current selection query functions
 *---------------------------------------------------------------------------*/
WidgetPtr MultiplexorWidget::selectedWidget() {
    return widgets[selectedWidgetIndex()];
}
WidgetConstPtr MultiplexorWidget::selectedWidget() const {
    return widgets[selectedWidgetIndex()];
}
IndexType MultiplexorWidget::selectedWidgetIndex() const {
    return selectedIndex;
}
const std::string & MultiplexorWidget::selectedWidgetName() const {
    return selectedWidget()->name();
}


/*---------------------------------------------------------------------------*
 | Add/remove functions
 *---------------------------------------------------------------------------*/
// Add a new widget to the list
void MultiplexorWidget::addWidget(WidgetPtr w) {
    if (! w)    // Oh NO you don't!
        return;

    // First, let's go hunt for an empty slot in the array
    IndexType slot;
    for (slot = 0; slot < IndexType(widgets.size()); ++slot)
        if (widgets[slot] == NULL)
            break;

    if (slot < IndexType(widgets.size())) // There was an empty slot
        widgets[slot] = w;
    else                                // Have to add it to the end
        widgets.push_back(w);

    if (widget == NULL)     // ...there is no selected widget
        selectWidget(slot);     // So select this one
}

// Remove widget 'w' from the list (true means success)
bool MultiplexorWidget::removeWidget(WidgetPtr w) {
    return removeWidget(indexOf(w));
}

// Remove the first Widget named 'nm' (true means success)
bool MultiplexorWidget::removeWidget(const std::string & nm) {
    return removeWidget(indexOf(nm));
}

// Remove a widget at the speficied location (true means success)
bool MultiplexorWidget::removeWidget(IndexType index) {
    // Make sure there's a valid Widget at that spot
    if (index >= 0 && index < IndexType(widgets.size())
                 && widgets[index] != NULL) {
        widgets[index] = WidgetPtr();   // Set it to NULL
        return true;                    // Success!
    } else
        return false;                   // I'm sorry, master.
}


/*---------------------------------------------------------------------------*
 | Widget selection functions
 *---------------------------------------------------------------------------*/
// Step backward in the Widget list
bool MultiplexorWidget::selectPreviousWidget() {
    return selectWidget(indexBefore(selectedIndex));
}

// Step forward in the Widget list
bool MultiplexorWidget::selectNextWidget() {
    return selectWidget(indexAfter(selectedIndex));
}

// Select the first occurance of Widget 'w'
bool MultiplexorWidget::selectWidget(WidgetConstPtr w) {
    return selectWidget(indexOf(w));
}

// Select a widget by name (if there are duplicates, select the first)
bool MultiplexorWidget::selectWidget(const std::string & nm) {
    return selectWidget(indexOf(nm));
}

// Jump straight to a widget
bool MultiplexorWidget::selectWidget(IndexType index) {
    // Make sure this is sane
    if (index >= 0 && index < IndexType(widgets.size())) {
        WidgetPtr w = widgets[index];
        if (w != NULL) {        // ...then this slot has a valid Widget
            widget = w;             // The superclass's "widget" property will
            selectedIndex = index;  // take care of notifying the new widgets
            requestRedisplay();
            cerr << "MUX[" << selectedIndex << "]: " << w->name() << endl;
            return true;        // We successfully changed widgets!
        }
    }
    return false;   // Awww...something went wrong
}

