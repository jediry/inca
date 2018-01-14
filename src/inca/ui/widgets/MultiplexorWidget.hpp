/*
 * File: MultiplexorWidget.hpp
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

#pragma once
#ifndef INCA_UI_MULTIPLEXOR_WIDGET
#define INCA_UI_MULTIPLEXOR_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class MultiplexorWidget;

        // Pointer typedefs
        typedef shared_ptr<MultiplexorWidget>       MultiplexorWidgetPtr;
        typedef shared_ptr<MultiplexorWidget const> MultiplexorWidgetConstPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"

// Import container definitions
#include <vector>

class inca::ui::MultiplexorWidget : public PassThruWidget {
public:
    // Container typedefs
    typedef std::vector<WidgetPtr> WidgetList;


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional component name
    explicit MultiplexorWidget(const std::string & nm = "")
        : PassThruWidget(nm) { }

    // Constructor taking an initial Widget and optional component name
    explicit MultiplexorWidget(WidgetPtr w, const std::string & nm = "")
        : PassThruWidget(nm) { addWidget(w); }

    // Constructor taking a group of Widgets, and an optional component name
    template <class InputIterator>
    MultiplexorWidget(InputIterator first, InputIterator last,
                      const std::string & nm = "") : PassThruWidget(nm) {
        while (first != last) { // Move across this iterator range, adding
            addWidget(*first);  // each Widget in sequence
            ++first;
        }
    }

    // Search functions
    IndexType indexOf(WidgetConstPtr w) const; // First index of 'w'
    IndexType indexOf(const std::string & nm) const; // First widget named 'nm'
    IndexType indexBefore(IndexType from) const; // Previous widget before 'from'
    IndexType indexAfter(IndexType from)  const; // Next widget after 'from'

    static const IndexType NOT_FOUND = -1;// The "I couldn't find it" index

    // Current selection query functions
    WidgetPtr      selectedWidget();
    WidgetConstPtr selectedWidget() const;
    IndexType           selectedWidgetIndex() const;
    const std::string & selectedWidgetName() const;

    // Add/remove functions
    void addWidget(WidgetPtr w);        // Stick a new Widget into the list
    bool removeWidget(WidgetPtr w);     // Remove the first occurance of 'w'
    bool removeWidget(const std::string & nm);// Remove the widget with name 'nm'
    bool removeWidget(IndexType index);   // Remove the widget at 'index'

    // Widget selection functions
    bool selectPreviousWidget();        // Step backward in the widget order
    bool selectNextWidget();            // Step forward in the widget order
    bool selectWidget(WidgetConstPtr w);// Select the first occurance of 'w'
    bool selectWidget(const std::string & nm);// Select the wigdet with name 'nm'
    bool selectWidget(IndexType index);   // Select the widget at 'index'

protected:
    WidgetList widgets;                 // The Widgets we're managing
    IndexType selectedIndex;              // Index of the active Widget
};

#endif
