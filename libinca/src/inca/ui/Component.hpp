/** -*- C++ -*-
 *
 * \file    Component.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Component class is the root of the UI class hierarchy. It defines
 *      the basic properties that all UI components share.
 *
 *      Architecturally, the Inca UI framework has a lot in common with the
 *      Java AWT (the user-interface model introduced in Java 1.0) and the
 *      Swing toolkit (which has largely supplanted the AWT in practice). The
 *      goal of the Inca UI framework is to provide a GUI toolkit-neutral set
 *      of "heavyweight" objects, which are implemented "behind the scenes" in
 *      terms of a particular UI toolkit, and an extensible set of "lightweight"
 *      objects, which are implemented using the Inca rendering architecture.
 *
 *      Unlike in the Java UI frameworks, widget extensibility is NOT a goal of
 *      the Inca UI framework, nor is uniform rendering and event-handling. If
 *      consistent cross-platform look & feel is important to you, you can
 *      either select an underlying toolkit that is available on all platforms
 *      you wish to support (e.g, FLTK, QT), or you can implement your own
 *      widgets as lightweight components.
 *
 *      Objects in the Inca UI framework follow a two-phase initialization
 *      mechanism, for several different reasons (explained below, for the
 *      curious). There is a virtual function called 'construct' that is
 *      guaranteed to be called before the component is first displayed. All
 *      GUI initialization should be deferred to this function, since when this
 *      is called:
 *          * all classes are initialized, down to the most-derived class,
 *            meaning that the 'this' pointer is completely valid and all
 *            virtual function calls will call the most-derived version
 *          * in the case of "heavyweight" components, the "peer" object is
 *            guaranteed to be valid, and may be used to configure the native
 *            object
 *          * in the case of "lightweight" components, the renderer object is
 *            guaranteed to be valid, and may be queried for system capabilities
 *            and used to construct display lists and such
 *      The construct() function may be called MULTIPLE TIMES over the lifetime
 *      of an object, so subclasses should be written with this in mind.
 *
 * Warning:
 *      There is currently no mechanism in place to ensure that the 'construct'
 *      function is called for each intermediate class, since a virtual function
 *      call always calls the most derived version. Thus, until such a
 *      mechanism is in place, any implementation of construct() should call
 *      the superclass version at the beginning, in order to ensure complete
 *      construction of the object.
 *
 *      This may be changed in a future version.
 *
 * Implementation:
 *      The reason for initializing objects in two passes is several-fold.
 *
 *      First, and most importantly, there is a chicken-and-egg problem
 *      involving the use of the 'this' pointer in the constructor. Since the
 *      object is not fully constructed, 'this' cannot be reliably used, and
 *      may corrupt the object in some cases (GCC silently allows this, and
 *      VC++ will complain only if 'this' is used in the constructor's
 *      initializer list). Furthermore, since the object may not yet be of its
 *      correct sub-type (in the case of inheritance), any virtual function
 *      calls may not select the correct, most-derived version.
 *
 *      Second, since the component has not yet been added into the object
 *      hierarchy, the 'parent' object pointer is not yet valid.
 *
 *      Third, since parts of the object hierarchy may be constructed before
 *      a rendering context is available, trying to configure the rendering
 *      subsystem in the constructor will likely introduce subtle bugs.
 *
 *      Fourth, some rendering architectures (e.g., Direct3D) will occasionally
 *      invalidate rendering objects (for example, if the application is
 *      minimized). The construct() function will be called as often as needed
 *      to re-create invalidated objects.
 *
 * TODO: ensure all construct() functions are called
 * TODO: do we need a destruct() function too?
 * TODO: do event filtering as far up chain as possible
 *          e.g., deregister the GLUT handler if unneeded
 * TODO: Finish Array math ops and remove dependency on math.hpp
 */

#ifndef INCA_UI_COMPONENT
#define INCA_UI_COMPONENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Component;

        // Pointer typedefs
        typedef shared_ptr<Component>       ComponentPtr;
        typedef shared_ptr<Component const> ComponentConstPtr;
        typedef weak_ptr<Component>         ComponentWeakPtr;
    };
};

// Import low-level event definitions & dispatching utility
#include "events/ComponentEvent.hpp"
#include "events/KeyEvent.hpp"
#include "events/PointerEvent.hpp"
#include "events/ButtonEvent.hpp"
#include "events/EventDispatcher"


// Import support for getting a shared_ptr from a 'this' ptr
#include <boost/enable_shared_from_this.hpp>
namespace boost {
    template <typename T> class shared_ptr;
}


class inca::ui::Component
    : public boost::enable_shared_from_this<inca::ui::Component> {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    typedef Event::Pixel        Pixel;
    typedef Event::Dimension    Dimension;

    // Event listener lists
    typedef EventDispatcher<ComponentEvent, ComponentListener> ComponentListenerList;
    typedef EventDispatcher<KeyEvent,     KeyListener>     KeyListenerList;
    typedef EventDispatcher<PointerEvent, PointerListener> PointerListenerList;
    typedef EventDispatcher<ButtonEvent,  ButtonListener>  ButtonListenerList;


/*---------------------------------------------------------------------------*
 | Constructors, initialization & destructor
 *---------------------------------------------------------------------------*/
protected:
    // Default constructor, taking optional component name
    explicit Component(const std::string & nm = std::string());
    
    // Virtual destructor to ensure proper destruction of subclasses
    virtual ~Component();

public:
    // 2nd-phase initialization function
    virtual void construct();


/*---------------------------------------------------------------------------*
 | Basic Component properties
 *---------------------------------------------------------------------------*/
public:
    // What's your name?
    const std::string & name() const;
    void setName(const std::string & nm);

    // Who's your daddy?
    ComponentPtr      parent();
    ComponentConstPtr parent() const;
    virtual void setParent(ComponentPtr p);

    
/*---------------------------------------------------------------------------*
 | Abstract interface functions for Component position, size & visibility
 *---------------------------------------------------------------------------*/
public:
    // Window position
    virtual Pixel position() const = 0;
    virtual void setPosition(Pixel p) = 0;
    virtual void setPosition(IndexType x, IndexType y) = 0;

    // Current size
    virtual Dimension size() const = 0;
    virtual void setSize(Dimension d) = 0;
    virtual void setSize(SizeType w, SizeType h) = 0;

    // Minimum allowable size
    virtual Dimension minimumSize() const = 0;
    virtual void setMinimumSize(Dimension d) = 0;
    virtual void setMinimumSize(SizeType w, SizeType h) = 0;

    // Maximum allowable size
    virtual Dimension maximumSize() const = 0;
    virtual void setMaximumSize(Dimension d) = 0;
    virtual void setMaximumSize(SizeType w, SizeType h) = 0;

    // Component visibility
    virtual bool visible() const = 0;
    virtual void setVisible(bool v) = 0;
    
protected:
    std::string         _name;      // The component's name
    ComponentWeakPtr    _parent;    // Weak pointer to the parent component


/*---------------------------------------------------------------------------*
 | shared_ptr to self mechanism:
 | This is necessary for getting into the shared_ptr "network" -- it's
 | *definitely* not enough just to do "shared_ptr<T>(this)", as the new
 | shared_ptr would delete the object upon going out of scope, even if there
 | were other shared_ptrs to the object elsewhere.
 *---------------------------------------------------------------------------*/
//protected:
//    friend class shared_ptr;    // WFT??
//    friend template <typename T> class boost::shared_ptr;

    // Non-const version, returning a shared_ptr-to-object of the requested
    // object type
    template <typename AsType>
    shared_ptr<AsType> self() {
        return dynamic_pointer_cast<AsType>(shared_from_this());
    }
    
    // Const version, returning a shared_ptr-to-const-object of the requested
    // object type
    template <typename AsType>
    shared_ptr<AsType const> self() const {
        return dynamic_pointer_cast<AsType const>(shared_from_this());
    }


/*---------------------------------------------------------------------------*
 | Event handling mechanism
 *---------------------------------------------------------------------------*/
public:
    // ComponentEvent listeners
    void addComponentListener(ComponentListenerPtr lp);
    void removeComponentListener(ComponentListenerPtr lp);

    // KeyEvent listeners
    void addKeyListener(KeyListenerPtr lp);
    void removeKeyListener(KeyListenerPtr lp);

    // PointerEvent listeners
    void addPointerListener(PointerListenerPtr lp);
    void removePointerListener(PointerListenerPtr lp);

    // ButtonEvent listeners
    void addButtonListener(ButtonListenerPtr lp);
    void removeButtonListener(ButtonListenerPtr lp);
  
protected:
    // These make the following function signatures shorter
    typedef InputEvent::ControlFlags ControlFlags;
    typedef KeyEvent::Character      Character;

    // ComponentEvent-firing functions
    void fireComponentMoved() const;
    void fireComponentResized() const;
    void fireComponentHidden() const;
    void fireComponentRevealed() const;

    // KeyEvent-firing functions
    void fireKeyPressed(ControlFlags flags, Pixel p, KeyCode k, Character ch) const;
    void fireKeyReleased(ControlFlags flags, Pixel p, KeyCode k, Character ch) const;
    void fireKeyTyped(ControlFlags flags, Pixel p, KeyCode k, Character ch) const;

    // PointerEvent-firing functions    
    void firePointerDragged(ControlFlags flags, Pixel p) const;
    void firePointerTracked(ControlFlags flags, Pixel p) const;
    void firePointerEntered(ControlFlags flags, Pixel p) const;
    void firePointerExited(ControlFlags flags, Pixel p) const;
    
    // ButtonEvent-firing functions
    void fireButtonPressed(ControlFlags flags, Pixel p, ButtonCode b) const;
    void fireButtonReleased(ControlFlags flags, Pixel p, ButtonCode b) const;
    void fireButtonClicked(ControlFlags flags, Pixel p, ButtonCode b) const;
    
    // Event listener lists
    ComponentListenerList   _componentListeners;
    KeyListenerList         _keyListeners;
    PointerListenerList     _pointerListeners;
    ButtonListenerList      _buttonListeners;
};

#endif
