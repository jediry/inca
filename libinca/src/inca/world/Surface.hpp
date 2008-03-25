/*
 * File: Surface.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#pragma once
#ifndef INCA_WORLD_SURFACE
#define INCA_WORLD_SURFACE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Surface;

        // Pointer typedefs
        typedef shared_ptr<Surface>       SurfacePtr;
        typedef shared_ptr<Surface const> SurfaceConstPtr;
    };
};


// Import superclass definitions
#include "WorldObject.hpp"
//#include <channel/ChannelContainer.hpp>


class inca::world::Surface : public WorldObject {//,
//                             public ChannelContainer {
public:
#if 0
    // Constructor
    Surface() {
        ior = -1;
        registerChannels();
        initializeChannelDefaults();
    }

    // Input methods XXX
    void setType(const std::string &s);
    void setIOR(GeometricScalar::arg_type _ior) { ior = _ior; }
    scalar_t getIOR() const { return ior; }

    // Channel network inputs
    const PointAdapterSource intersectPoint;
    const VectorAdapterSource lookVector;
    const VectorAdapterSource normalVector;

    // Channel network outputs
    const ColorPassThru shadowedColor;
    const ColorPassThru illuminatedColor;
    const ColorPassThru hilightedColor;
    const ScalarPassThru hilightTightness;
    const ScalarPassThru transparency;
    const ScalarPassThru reflectivity;

    scalar_t ior;

protected:
    // Channel network methods
    void registerChannels() {
#if 0
        // Register inputs
        addChannel("intersect", &intersectPoint);
        addChannel("normal", &normalVector);
        addChannel("look", &lookVector);

        // Register outputs (which can also serve as inputs)
        addChannel("shadowedColor", &shadowedColor);
        addChannel("illuminatedColor", &illuminatedColor);
        addChannel("hilightedColor", &hilightedColor);
        addChannel("hilightTightness", &hilightTightness);
        addChannel("transparency", &transparency);
        addChannel("reflectivity", &reflectivity);
#endif
    }

    void initializeChannelDefaults() {
        //XXX Ack! Hack! Horrible, horrible hack!
        ColorAdapterSource * dkred = new ColorAdapterSource();
        ColorAdapterSource * red = new ColorAdapterSource();
        ColorAdapterSource * white = new ColorAdapterSource();
        (*red) = Color(1, 0, 0, 1);
        (*dkred) = Color(0.5, 0, 0, 1);
        (*white) = Color(1, 1, 1, 1);
        ScalarAdapterSource * one = new ScalarAdapterSource();
        ScalarAdapterSource * zero = new ScalarAdapterSource();
        (*one) = 1;
        (*zero) = 0;
        
        shadowedColor << *dkred;
        illuminatedColor << *red;
        hilightedColor << *white;
        hilightTightness << *one;
        reflectivity << *zero;
        transparency << *zero;

/*        std::string initialSetup =
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
*/
    }
#endif
};

#endif
