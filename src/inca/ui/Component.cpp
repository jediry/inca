/*
 * File: Component.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "Component.hpp"
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructor & destructor
 *---------------------------------------------------------------------------*/
// Default constructor
Component::Component(const std::string & nm) : _name(nm) { }

// Second-pass initialization
void Component::construct() { }

// Destructor
Component::~Component() { }


/*---------------------------------------------------------------------------*
 | Basic Component properties
 *---------------------------------------------------------------------------*/
// What's your name?
const std::string & Component::name() const     { return _name; }
void Component::setName(const std::string & nm) { _name = nm; }

// Who's your daddy?
ComponentPtr      Component::parent()       { return _parent.lock(); }
ComponentConstPtr Component::parent() const { return _parent.lock(); }
void Component::setParent(ComponentPtr p) { _parent = p; }


/*---------------------------------------------------------------------------*
 | Event handling mechanism
 *---------------------------------------------------------------------------*/
// ComponentEvent listeners
void Component::addComponentListener(ComponentListenerPtr lp) {
    _componentListeners.add(lp);
}
void Component::removeComponentListener(ComponentListenerPtr lp) {
    _componentListeners.remove(lp);
}

// KeyEvent listeners
void Component::addKeyListener(KeyListenerPtr lp) {
    _keyListeners.add(lp);
}
void Component::removeKeyListener(KeyListenerPtr lp) {
    _keyListeners.remove(lp);
}

// PointerEvent listeners
void Component::addPointerListener(PointerListenerPtr lp) {
    _pointerListeners.add(lp);
}
void Component::removePointerListener(PointerListenerPtr lp) {
    _pointerListeners.remove(lp);
}

// ButtonEvent listeners
void Component::addButtonListener(ButtonListenerPtr lp) {
    _buttonListeners.add(lp);
}
void Component::removeButtonListener(ButtonListenerPtr lp) {
    _buttonListeners.remove(lp);
}


/*---------------------------------------------------------------------------*
 | Event firing functions
 *---------------------------------------------------------------------------*/
void Component::fireComponentMoved() const {
    ComponentEvent e(getSystemClocks(), ComponentEvent::ComponentMoved,
                     ComponentPtr(), position(), size(), visible());
    _componentListeners.fireEvent(e, &ComponentListener::componentMoved);
}

void Component::fireComponentResized() const {
    ComponentEvent e(getSystemClocks(), ComponentEvent::ComponentResized,
                     ComponentPtr(), position(), size(), visible());
    _componentListeners.fireEvent(e, &ComponentListener::componentResized);
}

void Component::fireComponentHidden() const {
    ComponentEvent e(getSystemClocks(), ComponentEvent::ComponentHidden,
                     ComponentPtr(), position(), size(), visible());
    _componentListeners.fireEvent(e, &ComponentListener::componentHidden);
}

void Component::fireComponentRevealed() const {
    ComponentEvent e(getSystemClocks(), ComponentEvent::ComponentRevealed,
                     ComponentPtr(), position(), size(), visible());
    _componentListeners.fireEvent(e, &ComponentListener::componentRevealed);
}



// KeyEvent-firing functions
void Component::fireKeyPressed(ControlFlags flags, Pixel p, 
                               KeyCode k, Character ch) const {
    KeyEvent e(getSystemClocks(), flags, p, KeyEvent::KeyPressed, k, ch);
    _keyListeners.fireEvent(e, &KeyListener::keyPressed);
}

void Component::fireKeyReleased(ControlFlags flags, Pixel p, 
                                KeyCode k, Character ch) const {
    KeyEvent e(getSystemClocks(), flags, p, KeyEvent::KeyReleased, k, ch);
    _keyListeners.fireEvent(e, &KeyListener::keyReleased);
}

void Component::fireKeyTyped(ControlFlags flags, Pixel p, 
                             KeyCode k, Character ch) const {
    KeyEvent e(getSystemClocks(), flags, p, KeyEvent::KeyTyped, k, ch);
    _keyListeners.fireEvent(e, &KeyListener::keyTyped);
}


// PointerEvent-firing functions
void Component::firePointerDragged(ControlFlags flags, Pixel p) const {
    PointerEvent e(getSystemClocks(), flags, p, PointerEvent::PointerDragged);
    _pointerListeners.fireEvent(e, &PointerListener::pointerDragged);
}

void Component::firePointerTracked(ControlFlags flags, Pixel p) const {
    PointerEvent e(getSystemClocks(), flags, p, PointerEvent::PointerTracked);
    _pointerListeners.fireEvent(e, &PointerListener::pointerTracked);
}

void Component::firePointerEntered(ControlFlags flags, Pixel p) const {
    PointerEvent e(getSystemClocks(), flags, p, PointerEvent::PointerEntered);
    _pointerListeners.fireEvent(e, &PointerListener::pointerEntered);
}

void Component::firePointerExited(ControlFlags flags, Pixel p) const {
    PointerEvent e(getSystemClocks(), flags, p, PointerEvent::PointerExited);
    _pointerListeners.fireEvent(e, &PointerListener::pointerExited);
}


// ButtonEvent-firing functions
void Component::fireButtonPressed(ControlFlags flags, Pixel p, ButtonCode b) const {
    ButtonEvent e(getSystemClocks(), flags, p, ButtonEvent::ButtonPressed, b);
    _buttonListeners.fireEvent(e, &ButtonListener::buttonPressed);
}

void Component::fireButtonReleased(ControlFlags flags, Pixel p, ButtonCode b) const {
    ButtonEvent e(getSystemClocks(), flags, p, ButtonEvent::ButtonReleased, b);
    _buttonListeners.fireEvent(e, &ButtonListener::buttonReleased);
}

void Component::fireButtonClicked(ControlFlags flags, Pixel p, ButtonCode b) const {
    ButtonEvent e(getSystemClocks(), flags, p, ButtonEvent::ButtonClicked, b);
    _buttonListeners.fireEvent(e, &ButtonListener::buttonClicked);
}
