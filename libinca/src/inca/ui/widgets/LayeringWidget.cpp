/*
 * File: LayeringWidget.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "LayeringWidget.hpp"
using namespace inca::ui;


// Import specialized wrapping iterator
#include <inca/util/wraparound_iterator>
using namespace inca;
typedef wraparound_iterator<LayeringWidget::WidgetList
                            ::const_iterator> wrap_iter;
typedef wraparound_iterator<LayeringWidget::WidgetList
                            ::const_reverse_iterator> reverse_wrap_iter;


/*---------------------------------------------------------------------------*
 | Search functions
 *---------------------------------------------------------------------------*/
// Return the index in the list of the first occurance of Widget 'w'
IndexType LayeringWidget::indexOf(WidgetConstPtr w) const {
    // Try to find the first occurance of this Widget
    for (IndexType i = 0; i < IndexType(widgets.size()); ++i)
        if (widgets[i] == w)        // OK...we found it
            return i;
    return NOT_FOUND;   // Crap.
}

// Return the index in the list of the first occurance of a Widget named 'nm'
IndexType LayeringWidget::indexOf(const std::string & nm) const {
    // Try to find the first occurance of this Widget
    for (IndexType i = 0; i < IndexType(widgets.size()); ++i)
        if (widgets[i] != NULL && widgets[i]->name() == nm)
            return i;   // OK...we found it
    return NOT_FOUND;   // Crap.
}

// Return the index of the previous non-NULL Widget (wrapping at the end)
IndexType LayeringWidget::indexBefore(IndexType from) const {
    // Iterate through every slot in the vector, starting with the current
    reverse_wrap_iter i(from, widgets.rbegin(), widgets.rend());
    ++i;
    while (i != widgets.rend()) {
        if (*i != NULL)     // If this is non-NULL, Bob's your uncle
            return (i.current().base() - widgets.begin()); // All done.
        ++i;    // Otherwise, keep looking
    }
    return NOT_FOUND;   // Crap.
}

// Return the index of the next non-NULL Widget (wrapping at the end)
IndexType LayeringWidget::indexAfter(IndexType from) const {
    // Iterate through every slot in the vector, starting with the current
    wrap_iter i(from, widgets.begin(), widgets.end());
    ++i;
    while (i != widgets.end()) {
        if (*i != NULL)     // If this is non-NULL, Bob's your uncle
            return (i.current() - widgets.begin()); // All done.
        ++i;    // Otherwise, keep looking
    }
    return NOT_FOUND;   // Crap.
}


/*---------------------------------------------------------------------------*
 | Add/remove functions
 *---------------------------------------------------------------------------*/
// Add a new widget to the list
void LayeringWidget::addWidget(WidgetPtr w) {
    if (! w)    // Oh NO you don't!
        return;

    // First, let's go hunt for an empty slot in the array
    IndexType slot;
    for (slot = 0; slot < IndexType(widgets.size()); ++slot)
        if (widgets[slot] == NULL)
            break;

    if (slot < IndexType(widgets.size())) { // There was an empty slot
        widgets[slot] = w;
        layerVisibility[slot] = true;
    } else {                                // Have to add it to the end
        widgets.push_back(w);
        layerVisibility.push_back(true);
    }

    adopt(w);
}

// Remove widget 'w' from the list (true means success)
bool LayeringWidget::removeWidget(WidgetPtr w) {
    return removeWidget(indexOf(w));
}

// Remove the first Widget named 'nm' (true means success)
bool LayeringWidget::removeWidget(const std::string & nm) {
    return removeWidget(indexOf(nm));
}

// Remove a widget at the speficied location (true means success)
bool LayeringWidget::removeWidget(IndexType index) {
    // Make sure there's a valid Widget at that spot
    if (index >= 0 && index < IndexType(widgets.size())
                 && widgets[index] != NULL) {
        widgets[index] = WidgetPtr();   // Set it to NULL
        return true;                    // Success!
    } else
        return false;                   // I'm sorry, master.
}

void LayeringWidget::setLayerVisible(int index, bool visible) {
    layerVisibility[index] = visible;
}
bool LayeringWidget::isLayerVisible(int index) {
    return layerVisibility[index];
}
bool LayeringWidget::toggleLayerVisible(int index) {
    setLayerVisible(index, ! isLayerVisible(index));
    return isLayerVisible(index);
}

void LayeringWidget::render() const {
    WidgetList::const_iterator it;
    std::vector<bool>::const_iterator vit;
    for (it = widgets.begin(), vit = layerVisibility.begin();
         it != widgets.end(); ++it, ++vit)
    {
        if (*vit)
            (*it)->render();
    }
}


/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface
 *---------------------------------------------------------------------------*/
void LayeringWidget::setSurface(RenderableSurfacePtr s) {
    Widget::setSurface(s);      // Call superclass version
    WidgetList::const_iterator it;
    for (it = widgets.begin(); it != widgets.end(); ++it)
        adopt(*it);
}
