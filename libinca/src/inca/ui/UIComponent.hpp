/*
 * File: UIComponent.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UI_COMPONENT
#define INCA_UI_COMPONENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class UIComponent;

        // Pointer typedefs
        typedef shared_ptr<UIComponent>         UIComponentPtr;
        typedef shared_ptr<UIComponent const>   UIComponentConstPtr;
    };
};

// Import math types
#include <inca/math.hpp>

// HACK import renderer definition
#include <inca/rendering.hpp>


// Import support for getting a shared_ptr from a 'this' ptr
#include <boost/enable_shared_from_this.hpp>

// This macro creates const and non-const versions of a member function
// called 'self()' that returns a shared_ptr to 'this'. This is necessary for
// getting into the shared_ptr "network" -- it's *definitely* not enough just
// to do "shared_ptr<T>(this)", as the new shared_ptr would delete the object
// immediately upon going out of scope.
#define SHARED_PTR_TO_SELF(TYPE)                                            \
    shared_ptr<TYPE> self() {                                               \
        return dynamic_pointer_cast<TYPE>(shared_from_this());              \
    }                                                                       \
    shared_ptr<TYPE const> self() const {                                   \
        return dynamic_pointer_cast<TYPE const>(shared_from_this());        \
    }


class inca::ui::UIComponent
    : public boost::enable_shared_from_this<UIComponent> {
private:
    // self() function to get a shared_ptr to myself of the appropriate type
    SHARED_PTR_TO_SELF(UIComponent);

    // Set up this class to own properties
    PROPERTY_OWNING_OBJECT(UIComponent);

public:
    // XXX HORRIBLY HACKED!!! Help me, Obi-Wan Kenobi!!!
    typedef rendering::ImmediateModeRenderer<rendering::OpenGLTraits,
                                             rendering::ExclusiveAccessCachePolicy> Renderer;
    typedef shared_ptr<Renderer> RendererPtr;
    typedef math::Point<IndexType, 2> Pixel;     // X,Y pair of integer coords
    typedef math::Vector<SizeType, 2> Dimension; // W,H pair of unsigned dims

protected:
    // Default constructor
    explicit UIComponent();

public:
    // Virtual destructor to ensure proper destruction of subclasses
    virtual ~UIComponent() { }

    // This component's name
    rw_property(std::string, name, "");

    bool selfPointerValid() const;
};

#endif
