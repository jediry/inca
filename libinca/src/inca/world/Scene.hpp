/*
 * File: Scene.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      A Scene represents a concrete, renderable virtual world, containing
 *      one or more Cameras, Lights, and Objects.
 */

#ifndef INCA_WORLD_SCENE
#define INCA_WORLD_SCENE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Scene;

        // Pointer typedefs
        typedef shared_ptr<Scene>       ScenePtr;
        typedef shared_ptr<Scene const> SceneConstPtr;
    };
};


// Import superclass definition
#include "WorldObject.hpp"

// Import other parts of the world object model
#include "Cameras.hpp"
#include "Light.hpp"
#include "Object.hpp"


class inca::world::Scene : public WorldObject {
private:
    // Internal typedefs
    typedef Scene ThisType;

public:
    // Constructor
    Scene() : cameras(this), objects(this), lights(this) { }

    // Things that we keep track of
    rw_list_property(CameraPtr, cameras);
    rw_list_property(ObjectPtr, objects);
    rw_list_property(LightPtr, lights);
};

#endif
