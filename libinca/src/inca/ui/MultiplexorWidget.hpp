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
        typedef shared_ptr<MultiplexorWidget> MultiplexorWidgetPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"


class inca::ui::MultiplexorWidget : public PassThruWidget {
public:
    // Container typedefs
    typedef vector<WidgetPtr> WidgetList;


    // Default constructor with optional component name
    MultiplexorWidget(const string &nm = "")
        : PassThruWidget(nm) { }

    // Constructor taking an initial Widget and optional component name
    MultiplexorWidget(WidgetPtr w, const string &nm = "")
        : PassThruWidget(nm) { addWidget(w); }

    // Constructor taking a group of Widgets, and an optional component name
    template <class InputIterator>
    MultiplexorWidget(InputIterator first, InputIterator last,
                      const string &nm = "") : PassThruWidget(nm) {
        while (first != last) { // Move across this iterator range, adding
            addWidget(*first);  // each Widget in sequence
            ++first;
        }
    }

    void renderView() {
        cerr << "Render MUX" << endl;
        PassThruWidget::renderView();
    }
 
    // Widget management functions
    void addWidget(WidgetPtr w);        // Stick a new Widget into the list
    bool removeWidget(WidgetPtr w);     
    bool removeWidget(index_t index);

    // Widget-switching functions
    void selectPreviousWidget();        // Step backward in the widget order
    void selectNextWidget();            // Step forward in the widget order
    void selectWidget(index_t index);   // Jump straight to a widget

protected:
    WidgetList widgets;                 // The Widgets we're managing
    index_t selectedIndex;              // Index of the active Widget
};

#endif
