/**
 *
 * \file Window.cpp
 *
 * \author Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include "Window.hpp"

// Import Application definition
#include "Application.hpp"

// TODO Need better way to get peers
#include <inca/integration/glut/GLUT-peers.hpp>

using namespace inca;
using namespace inca::ui;


/*---------------------------------------------------------------------------*
 | Constructors & destructor
 *---------------------------------------------------------------------------*/
// Constructor
Window::Window(const std::string & nm)
    : HeavyweightComponent<WindowPeer>(nm) { }

// Destructor
Window::~Window() { }

// Second-phase initialization
void Window::construct() {
    // Register to receive our own events
//    addComponentListener(self<ComponentListener>());
//    addWindowListener(self<WindowListener>());

    // Create the native object
    peer()->create();
    peer()->setTitle(this->name());
}


/*---------------------------------------------------------------------------*
 | XXX
 *---------------------------------------------------------------------------*/
RenderableSurfacePtr Window::surface() {
    if (! _surface)
        setSurface(new RenderableSurface("Main Surface"));
    return _surface;
}
RenderableSurfaceConstPtr Window::surface() const {
    if (! _surface)
        const_cast<Window *>(this)->setSurface(new RenderableSurface("Main Surface"));
    return _surface;
}
void Window::setSurface(RenderableSurface * s) {
    RenderableSurfacePtr sp(s);
    setSurface(sp);
}
void Window::setSurface(RenderableSurfacePtr s) {
    _surface = s;
    _surface->setPeer(new GLUTRenderableSurfacePeer(s.get()));
    _surface->setParent(self<Component>());
    _surface->construct();
}


/*---------------------------------------------------------------------------*
 | Window state control
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
void Window::toggleVisible() {
    setVisible(! visible());
}

// Window iconification state
bool Window::iconified() const {
    return peer()->iconified();
}
void Window::setIconified(bool icf) {
    peer()->setIconified(icf);
}
void Window::toggleIconified() {
    setIconified(! iconified());
}

// Window maximization state
bool Window::maximized() const {
    return peer()->maximized();
}
void Window::setMaximized(bool max) {
    peer()->setMaximized(max);
}
void Window::toggleMaximized() {
    setMaximized(! maximized());
}

// Window full-screen state
bool Window::fullScreen() const {
    return peer()->fullScreen();
}
void Window::setFullScreen(bool fs) {
    peer()->setFullScreen(fs);
}
void Window::toggleFullScreen() {
    setFullScreen(! fullScreen());
}

// Window resizable state
bool Window::resizable() const {
    return peer()->resizable();
}
void Window::setResizable(bool fs) {
    peer()->setResizable(fs);
}
void Window::toggleResizable() {
    setResizable(! resizable());
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
void Window::centerOnScreen() {
//    Dimension scr = getScreenSize();
    Dimension scr(1024, 768);
    Dimension sz = size();
    setPosition(Pixel((scr[0] - sz[0]) / 2, (scr[1] - sz[1]) / 2));
}


/*---------------------------------------------------------------------------*
 | Event-firing functions
 *---------------------------------------------------------------------------*/
void Window::fireWindowOpened() const {

}
void Window::fireWindowClosed() const {

}
