/*
 * File: Interior.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#pragma once
#ifndef INCA_WORLD_INTERIOR
#define INCA_WORLD_INTERIOR

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Interior;

        // Pointer typedefs
        typedef shared_ptr<Interior>        InteriorPtr;
        typedef shared_ptr<Interior const>  InteriorConstPtr;
    };
};


// Import superclass definitions
#include "WorldObject.hpp"
//#include <channel/ChannelContainer.hpp>


class inca::world::Interior : public WorldObject {//,
//                              public ChannelContainer {
public:
#if 0
    // Constructors
    Interior() {
        registerChannels();
        initializeChannelDefaults();
    }

    // Channel network inputs
    const PointAdapterSource intersectPoint;
    const VectorAdapterSource lookVector;

    // Channel network outputs
    const VectorPassThru normal;
    const ColorPassThru shadowedColor;
    const ColorPassThru illuminatedColor;
    const ColorPassThru hilightedColor;
    const ScalarPassThru hilightTightness;
    const ScalarPassThru transparency;
    const ScalarPassThru reflectivity;

protected:
    // Channel network methods
    void registerChannels() {
        // Register inputs
        addChannel("intersect", &intersectPoint);
        addChannel("look", &lookVector);

        // Register outputs (which can also serve as inputs)
        addChannel("normal", &normal);
        addChannel("shadowedColor", &shadowedColor);
        addChannel("illuminatedColor", &illuminatedColor);
        addChannel("hilightedColor", &hilightedColor);
        addChannel("hilightTightness", &hilightTightness);
        addChannel("transparency", &transparency);
        addChannel("reflectivity", &reflectivity);
    }

    void initializeChannelDefaults() {
#if 0
        std::string initialSetup =
            // Our material is medium gray
            "illuminatedColor = [0.5, 0.5, 0.5, 1.0];\n"
            // Shadow area is the same hue
            "shadowedColor = illuminatedColor * 0.2;\n"
            // Hilights are white and tight
            "hilightColor = [1.0, 1.0, 1.0];\n"
            "hilightTightness = 20;\n"
            // We're non-transparent, non-reflective
            "transparency = 0;\n"
            "reflectivity = 0;\n";
        parseChannelExpression(initialSetup);
#endif
    }
#endif
};

#endif
