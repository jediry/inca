/** -*- C++ -*-
 *
 * \file    BasicWidget.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The BasicWidget class provides default, no-op implementations of the
 *      abstract functions required by the Widget interface, simplifying the
 *      creation of widgets by allowing them to implement only the event
 *      handlers they're interested in.
 *
 *      For you design pattern buffs out there, this is basically the
 *      Adapter design pattern.
 */

#pragma once
#ifndef INCA_UI_BASIC_WIDGET
#define INCA_UI_BASIC_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declaration
        class BasicWidget;

        // Pointer typedef
        typedef shared_ptr<BasicWidget>       BasicWidgetPtr;
        typedef shared_ptr<BasicWidget const> BasicWidgetConstPtr;
    };
};

// Import superclass definition
#include "Widget.hpp"


class inca::ui::BasicWidget : public inca::ui::Widget {
/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Default constructor with optional component name
    explicit BasicWidget(const std::string & nm = std::string());


/*---------------------------------------------------------------------------*
 | Component position, size & visibility
 *---------------------------------------------------------------------------*/
public:
    // Current position
    Pixel position() const;
    void setPosition(Pixel p);
    void setPosition(IndexType x, IndexType y);

    // Current size
    Dimension size() const;
    void setSize(Dimension d);
    void setSize(SizeType w, SizeType h);

    // Minimum allowable size
    Dimension minimumSize() const;
    void setMinimumSize(Dimension d);
    void setMinimumSize(SizeType w, SizeType h);

    // Maximum allowable size
    Dimension maximumSize() const;
    void setMaximumSize(Dimension d);
    void setMaximumSize(SizeType w, SizeType h);

    // Visibility
    bool visible() const;
    void setVisible(bool v);

protected:
    Pixel       _position;
    Dimension   _size,
                _minimumSize,
                _maximumSize;
    bool        _visible;
};

#endif
