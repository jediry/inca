/*
 * File: Object.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Object class describes a (probably) visible object, in the world,
 *      including its geometry, its material properties, and its
 *      transformations. Objects are the only things in a world that are
 *      rendered (usually).
 */

#ifndef INCA_WORLD_OBJECT
#define INCA_WORLD_OBJECT

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Object;

        // Pointer typedefs
        typedef shared_ptr<Object>       ObjectPtr;
        typedef shared_ptr<Object const> ObjectConstPtr;
    };
};


// Import Inca::Math library
#include <inca/math.hpp>

// Import the superclass and related object definitions
#include "WorldObject.hpp"
#include "Cameras.hpp"
#include "Transform.hpp"
//#include "Material.hpp"


class inca::world::Object : public WorldObject {
private:
    // Convenience typedefs
    typedef Object ThisType;

public:
    // Constructor
    Object() : transform(this) { }
    //Object() : transform(this), materials(this) { }

    // The properties that make up an object
    rw_ptr_property(Transform, transform, new Transform());
//    rw_list_property(Material, materials);

    // General object update hook
    virtual void update(CameraPtr camera, double time) { }
};

#endif
