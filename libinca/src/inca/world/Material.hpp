/*
 * File: Material.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Material class defines the material properties for an object.
 *      Material properties include an object's pigment, finish, texture,
 *      and interior properties. These properties are used to calculate
 *      the object's response to light, and thus, its appearance.
 */

#ifndef INCA_WORLD_MATERIAL
#define INCA_WORLD_MATERIAL

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Material;

        // Pointer typedefs
        typedef shared_ptr<Material>        MaterialPtr;
        typedef shared_ptr<Material const>  MaterialConstPtr;
    };
};


// Import superclass definition
#include "WorldObject.hpp"

// Import Inca::Math and Inca::Imaging libraries
#include <inca/math.hpp>
#include <inca/imaging.hpp>

// Import sub-component definitions
//#include "Transform.hpp"
//#include "Surface.hpp"
//#include "Interior.hpp"


class inca::world::Material : public WorldObject {
private:
    // Convenience typedefs
    typedef Material ThisType;

public:
    // Import typedefs for the scalar model we're using for colors
    typedef float                   scalar_t;
    typedef float                   scalar_arg_t;
    typedef imaging::Color<float, imaging::sRGB, true> Color;

    // Import typedefs for imaging data
    //INCA_IMAGING_TYPEDEFS(Scalar, Inca::Imaging::sRGB, , );


//    // The properties that make up a material
//    PTR_PROPERTY(transform, Transform, , setTransform);
//    PTR_PROPERTY(interior, Interior, , setInterior);
//    PTR_PROPERTY(surface, Surface, , setSurface);

    // Constructor
    Material() : illuminated(this), ambient(this), diffuse(this),
                 specular(this), emissivity(this), glossiness(this),
                 opacity(this) { }

    // Should I be rendered lit?
    rw_property(bool, illuminated, true);

    rw_property(Color, ambient, Color(0.2f, 0.2f, 0.2f));
    rw_property(Color, diffuse, Color(0.5f, 0.5f, 0.5f));
    rw_property(Color, specular, Color(0.0f, 0.0f, 0.0f));
    rw_property(Color, emissivity, Color(0.0f, 0.0f, 0.0f));
    rw_property(scalar_t, glossiness, 3.0f);
    rw_property(scalar_t, opacity, 1.0f);

    // Hack me! Hack me!
//    rw_ptr_property(Image, image, NULL);
    void setImage(const string &filename);
};

#endif
