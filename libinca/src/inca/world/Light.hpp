/*
 * File: Lights.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Light class represents a scene light, which may be
 */

#ifndef INCA_WORLD_LIGHTS
#define INCA_WORLD_LIGHTS

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Light;
        class AmbientLight;
        class PointLight;
        class SpotLight;

        // Pointer typedefs
        typedef shared_ptr<Light>               LightPtr;
        typedef shared_ptr<Light const>         LightConstPtr;
        typedef shared_ptr<AmbientLight>        AmbientLightPtr;
        typedef shared_ptr<AmbientLight const>  AmbientLightConstPtr;
        typedef shared_ptr<PointLight>          PointLightPtr;
        typedef shared_ptr<PointLight const>    PointLightConstPtr;
        typedef shared_ptr<SpotLight>           SpotLightPtr;
        typedef shared_ptr<SpotLight const>     SpotLightConstPtr;
    };
};


// Import superclass definition
#include "WorldObject.hpp"

// Include the Transform definition
#include "Transform.hpp"

// Include the inca::imaging library
#include <inca/imaging.hpp>


class inca::world::Light : public WorldObject {
private:
    // Convenience typedefs
    typedef Light ThisType;

public:
    // Import typedefs for the scalar model we're using for colors
    typedef float       scalar_t;
    typedef float       scalar_arg_t;
    typedef imaging::Color<float, imaging::sRGB, false> Color;

    // Import typedefs for imaging data
    //INCA_IMAGING_TYPEDEFS(Scalar, Inca::Imaging::sRGB, /* */, /* */);

    // Constructor
    Light() : transform(this), color(this), intensity(this) { }

    // Shared light properties
    rw_ptr_property(Transform, transform, new Transform());
    rw_property(Color, color, Color(1, 1, 1));
    rw_property(scalar_t, intensity, 1.0);
};


class inca::world::AmbientLight : public Light {
private:
    // Convenience typedefs
    typedef AmbientLight ThisType;

    /* This class has no additional properties */
};


class inca::world::PointLight : public Light {
private:
    // Convenience typedefs
    typedef PointLight ThisType;

public:
    // Constructor
    PointLight() { }
};


class inca::world::SpotLight : public Light {
private:
    // Convenience typedefs
    typedef SpotLight ThisType;

public:
    // Constructor
    SpotLight() { }
};

#endif

