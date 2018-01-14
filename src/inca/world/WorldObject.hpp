/*
 * File: WorldObject.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      WorldObject is the base class for all of the world-level classes. Each
 *      object in the world has a unique ID.
 */

#pragma once
#ifndef INCA_WORLD_OBJECT_BASE
#define INCA_WORLD_OBJECT_BASE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca::World object model
namespace inca {
    namespace world {
        // Forward declarations
        class WorldObject;
    };
};


class inca::world::WorldObject {
private:
    // Convenience typedefs
    typedef WorldObject ThisType;

public:
    // Constructors
//    WorldObject() : id(this) { }
//    WorldObject(const std::string & i) : id(this, i) { }

    // All objects in the world have a (hopefully) unique ID
//    rw_property(std::string, id, "hello");

protected:
    // This exists only to ensure that the compiler generates RTTI info
    virtual void ensure_RTTI_InfoIsGeneratedForThisClass() { }
};

#endif

