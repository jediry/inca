/* -*- C++ -*-
 *
 * File: RandomGeneratorBase.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The RandomGeneratorBase class is an object interface to the a
 *      (psuedo) random number generator. It provides the basic facilities
 *      used by more specialized random number generators, such as the ability
 *      to get a random integer and to reseed the generator.
 */

#ifndef INCA_MATH_GENERATOR_RANDOM_GENERATOR_BASE
#define INCA_MATH_GENERATOR_RANDOM_GENERATOR_BASE

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca math library
namespace inca {
    namespace math {
        // Forward declaration
        class RandomGeneratorBase;
    };
};

#include <stdlib.h>
#include <time.h>

class inca::math::RandomGeneratorBase {
public:
    // Constructor using a random (timer-based) seed
    RandomGeneratorBase() { /*reseed();*/ }

    // Constructor accepting a user-specified seed
    RandomGeneratorBase(unsigned int s) { reseed(s); }

    // Set a new seed and reinitialize the generator
    void reseed(unsigned int s = static_cast<unsigned int>(time(NULL))) {
        _seed = s;
        reset();
    }
    
    // Reinitialize the generator using the stored seed value
    void reset() { srand(seed()); }

    // Retrieve the currently stored seed value
    unsigned int seed() const { return _seed; }

    // Generate a random integer
    unsigned int randomInteger() const { return rand(); }
    
    // Return the upper bound on the generated integer
    unsigned int maximumInteger() const { return RAND_MAX; }

protected:
    unsigned int _seed;
};

#endif
