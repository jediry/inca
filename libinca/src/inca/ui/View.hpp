/*
 * File: View.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_VIEW
#define INCA_UI_VIEW

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class View;

        // Pointer typedef
        typedef shared_ptr<View>       ViewPtr;
        typedef shared_ptr<View const> ViewConstPtr;
    };
};

// Import superclass definition
#include "WidgetPart.hpp"


class inca::ui::View : virtual public WidgetPart {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(View);

protected:
    // Non-public default constructor
    explicit View() { }

    // Non-public constructor with component name
    explicit View(const std::string & nm) { name = nm; }

public:
    // View callbacks
    virtual void initializeView() = 0;
    virtual void resizeView(Dimension size) = 0;
    virtual void renderView() = 0;
};

#endif
