/*
 * File: PassThruView.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PassThruView class is a concrete View that simply wraps
 *      another View, and passes events through to the wrapped View.
 *      This makes a convenient base class for Views that delegate most
 *      of their rendering to another View, but overlay additional content
 *      on top of the other View.
 */

#ifndef INCA_UI_PASS_THRU_VIEW
#define INCA_UI_PASS_THRU_VIEW

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class PassThruView;

        // Pointer typedefs
        typedef shared_ptr<PassThruView>       PassThruViewPtr;
        typedef shared_ptr<PassThruView const> PassThruViewConstPtr;
    };
};

// Import superclass definition
#include "View.hpp"


class inca::ui::PassThruView : virtual public View,
                               virtual public WidgetPartContainer {
private:
    // Set this class up to contain properties
    PROPERTY_OWNING_OBJECT(PassThruView);


/*---------------------------------------------------------------------------*
 | Constructors & Properties
 *---------------------------------------------------------------------------*/
public:
    // Default constructor
    PassThruView() : view(this) { }

    // Constructor with initialization of View
    PassThruView(ViewPtr v)
        : view(this, v) { }

    // Constructor with initialization of component name
    PassThruView(const string &nm)
        : view(this) { name = nm; }

    // Constructor with View and component name
    PassThruView(ViewPtr v, const string &nm)
        : view(this, v) { name = nm; }

    // The View that we're wrapping
    rw_ptr_property_custom_set(View, view, NULL);

    // Custom setter for the "view" property
    void ptr_property_set(View, view) {
        releaseWidgetPart(_view); // Let the old View go
        _view = value;            // Set the new View
        acquireWidgetPart(_view); // Tell him we got 'im
    }


/*---------------------------------------------------------------------------*
 | WidgetPartContainer function to propagate redisplay requests
 *---------------------------------------------------------------------------*/
public:
    // Pass redisplay requests up to my parent
    void redisplay(WidgetPartPtr w) { requestRedisplay(); }


/*---------------------------------------------------------------------------*
 | View-related events (passed-thru to the wrapped View, if there is one)
 *---------------------------------------------------------------------------*/
public:
    void initializeView() {
        if (view) view->initializeView();
    }
    void resizeView(Dimension size) {
        if (view) view->resizeView(size);
    }
    void renderView() {
        if (view) view->renderView();
    }
};

#endif
