/** -*- C++ -*-
 *
 * \file    BasicWidget.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the functions belonging to the BasicWidget class.
 */

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "BasicWidget.hpp"

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
BasicWidget::BasicWidget(const std::string & nm)
    : Widget(nm), _position(0, 0), _size(0, 0), _visible(true),
      _minimumSize(0, 0), _maximumSize(10000, 10000) { }
      

/*---------------------------------------------------------------------------*
 | Component position, size & visibility
 *---------------------------------------------------------------------------*/
BasicWidget::Pixel BasicWidget::position() const {
    return _position;
}
void BasicWidget::setPosition(Pixel p) {
    _position = p;
    fireComponentMoved();
}
void BasicWidget::setPosition(IndexType x, IndexType y) {
    _position = Pixel(x, y);
    fireComponentMoved();
}

// Current size
BasicWidget::Dimension BasicWidget::size() const {
    return _size;
}
void BasicWidget::setSize(Dimension d) {
    _size = d;
    fireComponentResized();
}
void BasicWidget::setSize(SizeType w, SizeType h) {
    _size = Dimension(w, h);
    fireComponentResized();
}

// Minimum allowable size
BasicWidget::Dimension BasicWidget::minimumSize() const {
    return _minimumSize;
}
void BasicWidget::setMinimumSize(Dimension d) {
    _minimumSize = d;
}
void BasicWidget::setMinimumSize(SizeType w, SizeType h) {
    _minimumSize = Dimension(w, h);
}

// Maximum allowable size
BasicWidget::Dimension BasicWidget::maximumSize() const {
    return _maximumSize;
}
void BasicWidget::setMaximumSize(Dimension d) {
    _maximumSize = d;
}
void BasicWidget::setMaximumSize(SizeType w, SizeType h) {
    _maximumSize = Dimension(w, h);
}

// Visibility
bool BasicWidget::visible() const {
    return _visible;
}
void BasicWidget::setVisible(bool v) {
    if (v != visible()) {
        _visible = v;
        if (v)  fireComponentRevealed();
        else    fireComponentHidden();
    }
}
