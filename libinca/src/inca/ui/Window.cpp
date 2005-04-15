/**
 *
 * \file Window.cpp
 *
 * \author Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *
 * FIXME: you CANNOT add widgets inside the subclass constructor, since no
 * shared_ptr has been constructed yet, and the self() function is not valid.
 */

// Import class definition
#include "Window.hpp"

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructors & peer interface
 *---------------------------------------------------------------------------*/

// Constructor
Window::Window(const std::string & title) {
    XXX
}

// Destructor
Window::~Window() {
    if (_peer)
        delete _peer;
}

// Function to set the peer pImpl
Window::Peer & Window::peer() {
    return *_peer;
}
const Window::Peer & Window::peer() const {
    return *_peer;
}
void Window::setPeer(Window::Peer * p) {
    _peer = p;
}


/*---------------------------------------------------------------------------*
 | Window state functions
 *---------------------------------------------------------------------------*/
// Window title
std::string Window::title() const {
    return peer()->title();
}
void Window::setTitle(const std::string & title) {
    peer()->setTitle(title);
}

// Window visibility state
bool Window::visible() const {
    return peer()->visible();
}
void Window::setVisible(bool vs) {
    peer()->setVisible(vs);
}

// Window iconification state
bool Window::iconified() const {
    return peer()->iconified();
}
void Window::setIconified(bool icf) {
    peer()->setIconified(icf);
}

// Window maximization state
bool Window::maximized() const {
    return peer()->maximized();
}
void Window::setMaximized(bool max) {
    peer()->setMaximized(max);
}

// Window full-screen state
bool Window::fullScreen() const {
    return peer()->fullScreen();
}
void Window::setFullScreen(bool fs) {
    peer()->setFullScreen(fs);
}

// Restore a maximized, iconified or full-screened window to its previous state
void Window::restore() {
    if (fullScreen())
        setFullScreen(false);
    else if (maximized())
        setMaximized(false);
    else if (iconified())
        setIconified(false);
}


/*---------------------------------------------------------------------------*
 | Window size & position
 *---------------------------------------------------------------------------*/
// Window position
Pixel Window::position() const {
    return peer()->position();
}
void Window::setPosition(Pixel px) {
    peer()->setPosition(px);
}
void Window::setPosition(IndexType x, IndexType y) {
    peer()->setPosition(Pixel(x, y));
}
void Window::centerOnScreen() {
    Dimension scr = getScreenSize();
    Dimension sz = size();
    setPosition(Pixel((scr[0] - sz[0]) / 2, (scr[1] - sz[1]) / 2));
}

// Current size
Dimension Window::size() const {
    return peer()->size();
}
void Window::setSize(Dimension d) {
    peer()->setSize(d);
}
void Window::setSize(SizeType w, SizeType h) {
    peer()->setSize(Dimension(w, h));
}

// Minimum allowable size
Dimension Window::minimumSize() const {
    return peer()->minimumSize();
}
void Window::setMinimumSize(Dimension d) {
    peer()->setMinimumSize(d);
}
void Window::setMinimumSize(SizeType w, SizeType h) {
    peer()->setMinimumSize(Dimension(w, h));
}

// Maximum allowable size
Dimension Window::maximumSize() const {
    return peer()->maximumSize();
}
void Window::setMaximumSize(Dimension d) {
    peer()->setMaximumSize(d);
}
void Window::setMaximumSize(SizeType w, SizeType h) {
    peer()->setMaximumSize(Dimension(w, h));
}

// Force aspect ratio (0.0 to allow unrestricted A/R)
float Window::getAspectRatio() const {
    Dimension sz = size();
    return sz[0] / float(sz[1]);
}
void Window::setAspectRatio(float ratio) {
    // XXX
}



// Query screen size
Dimension Window::getScreenSize() const {
}

// Request redisplay of the entire Window
void Window::requestRedisplay() const {
}
