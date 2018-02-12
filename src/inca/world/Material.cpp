/*
 * File: Material.cpp
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

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "Material.hpp"
using namespace inca::world;

#if 0
// Import Inca::IO subsystem
#include <io/IOUtilities.hpp>


void Material::setImage(const std::string & filename) {
    if (filename == "") {
        image = ImagePtr(); // Un-set
    } else {
        ImagePtr img = IO::loadImage(filename);
        if (img != NULL)
            image = img;
    }
}
#endif
