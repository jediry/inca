/*
 * File: BasicView.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_BASIC_VIEW
#define INCA_UI_BASIC_VIEW

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class BasicView;

        // Pointer typedef
        typedef shared_ptr<BasicView>       BasicViewPtr;
        typedef shared_ptr<BasicView const> BasicViewConstPtr;
    };
};

// Import superclass definition
#include "View.hpp"


class inca::ui::BasicView : virtual public View {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(BasicView);

public:
    // Default constructor with optional component name
    explicit BasicView(const string &nm = "")
        : size(0, 0) { name = nm; }


public:
    // View callbacks
    void initializeView() { }
    void resizeView(Dimension sz) { size = sz; }
    void renderView() { }
    
protected:
    Dimension size;
};

#endif
