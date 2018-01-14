/*
 * File: globals.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file's sole purpose in life is to instantiate global and static
 *      variables, which require a compilation unit to live in.
 */

// Import system configuration
#include <inca/inca-common.h>

// The global Logger instance
#include <inca/util/Logger.hpp>
inca::Logger inca::logger;

#if 0
// Dynamics state-vector pools
RigidBodySystemState::StateVectorPool RigidBodySystemState::stateVectorPool;
RigidBodySystemCalculation::StateVectorPool RigidBodySystemCalculation::stateVectorPool;
#endif
