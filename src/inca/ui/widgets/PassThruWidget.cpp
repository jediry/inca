/*
 * File: PassThruWidget.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The PassThruWidget class is a concrete Widget that simply wraps
 *      another Widget, and passes events through to the wrapped Widget.
 *      This makes a convenient base class for Widgets that want to intercept
 *      certain events (e.g., the Ctrl-Q sequence for quitting the application)
 *      and pass through the rest of them (mouse clicks, other key presses).
 */

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "PassThruWidget.hpp"
using namespace inca;
using namespace inca::ui;


// Default constructor with an optional component name
PassThruWidget::PassThruWidget(const std::string & nm)
    : BasicWidget(nm), widget(this) { }

void PassThruWidget::ptr_property_set(Widget, widget) {
    if (_widget)
        disown(_widget);
    _widget = value;                        // Set the new Widget
    if (_widget)
        adopt(_widget);
}


// Current position
void PassThruWidget::setPosition(Pixel p) {
    if (widget)
        widget->setPosition(p);
    BasicWidget::setPosition(p);
}
void PassThruWidget::setPosition(IndexType x, IndexType y) {
    if (widget)
        widget->setPosition(x, y);
    BasicWidget::setPosition(x, y);
}

// Current size
void PassThruWidget::setSize(Dimension d) {
    if (widget)
        widget->setSize(d);
    BasicWidget::setSize(d);
}
void PassThruWidget::setSize(SizeType w, SizeType h) {
    if (widget)
        widget->setSize(w, h);
    BasicWidget::setSize(w, h);
}

// Minimum allowable size
void PassThruWidget::setMinimumSize(Dimension d) {
    if (widget)
        widget->setMinimumSize(d);
    BasicWidget::setMinimumSize(d);
}
void PassThruWidget::setMinimumSize(SizeType w, SizeType h) {
    if (widget)
        widget->setMinimumSize(w, h);
    BasicWidget::setMinimumSize(w, h);
}

// Maximum allowable size
void PassThruWidget::setMaximumSize(Dimension d) {
    if (widget)
        widget->setMaximumSize(d);
    BasicWidget::setMaximumSize(d);
}
void PassThruWidget::setMaximumSize(SizeType w, SizeType h) {
    if (widget)
        widget->setMaximumSize(w, h);
    BasicWidget::setMaximumSize(w, h);
}

// Visibility
bool PassThruWidget::visible() const {
    return BasicWidget::visible();
}
void PassThruWidget::setVisible(bool v) {
    if (widget)
        widget->setVisible(v);
    BasicWidget::setVisible(v);
}


// Render function
void PassThruWidget::render() const {
    if (widget)
        widget->render();
}


// ComponentEvent handlers
void PassThruWidget::componentMoved(const ComponentEvent & e) {
    if (widget)
        widget->componentMoved(e);
}
void PassThruWidget::componentResized(const ComponentEvent & e) {
    if (widget)
        widget->componentResized(e);
}
void PassThruWidget::componentHidden(const ComponentEvent & e) {
    if (widget)
        widget->componentHidden(e);
}
void PassThruWidget::componentRevealed(const ComponentEvent & e) {
    if (widget)
        widget->componentRevealed(e);
}


// KeyEvent handlers
void PassThruWidget::keyPressed(const KeyEvent & e) {
    if (widget)
        widget->keyPressed(e);
}
void PassThruWidget::keyReleased(const KeyEvent & e) {
    if (widget)
        widget->keyReleased(e);
}
void PassThruWidget::keyTyped(const KeyEvent & e) {
    if (widget)
        widget->keyTyped(e);
}

// PointerEvent handlers
void PassThruWidget::pointerDragged(const PointerEvent & e) {
    if (widget)
        widget->pointerDragged(e);
}
void PassThruWidget::pointerTracked(const PointerEvent & e) {
    if (widget)
        widget->pointerTracked(e);
}
void PassThruWidget::pointerEntered(const PointerEvent & e) {
    if (widget)
        widget->pointerEntered(e);
}
void PassThruWidget::pointerExited(const PointerEvent & e) {
    if (widget)
        widget->pointerExited(e);
}

// ButtonEvent handlers
void PassThruWidget::buttonPressed(const ButtonEvent & e) {
    if (widget)
        widget->buttonPressed(e);
}
void PassThruWidget::buttonReleased(const ButtonEvent & e) {
    if (widget)
        widget->buttonReleased(e);
}
void PassThruWidget::buttonClicked(const ButtonEvent & e) {
    if (widget)
        widget->buttonClicked(e);
}


/*---------------------------------------------------------------------------*
 | Interface with the embedding RenderableSurface
 *---------------------------------------------------------------------------*/
void PassThruWidget::setSurface(RenderableSurfacePtr s) {
    Widget::setSurface(s);      // Call superclass version
    if (widget)
        adopt(widget());        // Cascade to the wrapped widget
}
