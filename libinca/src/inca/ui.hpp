/*
 * File: ui.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file defines the classes comprising the generic Inca
 *      user-interface framework. Conceptually, an Inca user-interface is just
 *      a pretty, interactive "skin" over the real nuts and bolts: no core
 *      functionality is actually implemented in the UI code.
 *
 *      In order to simplify the writing of individual interfaces
 *      (using, for example, GLUT, GTK+, QT, or even plain, old console-mode),
 *      common functionality, such as event handling, command-line
 *      argument processing, etc. is implemented in these generic classes.
 *      Since these classes cannot be directly instantiated to make an
 *      application, there is no main() function defined here -- this will be
 *      in a toolkit-specific c++ file that ties everything together.
 *
 *      The Application interface is the base for all classes that
 *      act as the top-level class in an application. It is not itself
 *      visible, but may own and instantiate windows. It provides generic
 *      functionality for parsing command-line options and requires
 *      implementation of a constructInterface() function.
 *
 *      The View interface is the base for all classes that draw a 2D or 3D
 *      rectangular view of something, such as a portion of an image,
 *      or a camera view into an Inca Scene. A View has a height and width,
 *      which give the largest rectangular area that the View may draw to,
 *      though it need not utilize the entire area. This permits
 *      partially-transparent Views to be layered on top of one another. If
 *      the bottommost View is partially transparent, then the container should
 *      first paint the drawing area with its background color to prevent
 *      multiple frames of animation from overlapping.
 *
 *      The Control interface is the base class for generic user
 *      event-handling. Control provides no-op implementations of its
 *      event-handlers, which may be overridden in subclasses that want to
 *      receive these events. It also provides a toolkit-independent mechanism
 *      for determining the current state of keyboard modifier keys (Ctrl,
 *      Alt, Shift) and mouse buttons.
 *
 *      The Widget interface is a composite of a View and a Control, for the
 *      sake of convenience, as in most cases, a View will also want to do
 *      its own event handling.
 *
 *      The WidgetContainer interface describes the functions that must be
 *      implemented by something that wants to be able to support embedding
 *      Widgets in it. This will need to be implemented in a toolkit-specific
 *      manner.
 *
 *      The normal way of creating an Inca UI is to subclass Application
 *      and implement the required abstract functions. The main() function
 *      should look something like this:
 *          int main(int argc, char **argv) {
 *              GLUTMyApp app;
 *              app.initialize(argc, argv);
 *              app.run();
 *          }
 *
 *      and should defer any GUI object initialization until the
 *      constructInterface.
 *
 * TODO:
 *      WidgetContainer needs some thoughtful elaboration!
 */

#ifndef INCA_UI
#define INCA_UI

// Import core ui framework classes
#include "ui/UIComponent.hpp"
#include "ui/WidgetPart.hpp"
#include "ui/View.hpp"
#include "ui/Control.hpp"
#include "ui/Widget.hpp"
#include "ui/Window.hpp"
#include "ui/Application.hpp"

// Import basic widget subclasses
#include "ui/BasicView.hpp"
#include "ui/BasicControl.hpp"
#include "ui/BasicWidget.hpp"
#include "ui/PassThruView.hpp"
#include "ui/PassThruControl.hpp"
#include "ui/PassThruWidget.hpp"
#include "ui/CompositeWidget.hpp"

// Import specialized widget classes
#include "ui/CameraControl.hpp"
#include "ui/MultiplexorWidget.hpp"

// Import supported toolkit implementations
#include <inca/integration/glut/GLUTApplication.hpp>
#include <inca/integration/glut/GLUTWindow.hpp>

#endif
