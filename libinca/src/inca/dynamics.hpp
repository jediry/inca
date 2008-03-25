/* -*- C++ -*-
 *
 * File: Dynamics.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including
 *      all of the dynamics stuff.
 */

#pragma once
#ifndef INCA_DYNAMICS
#define INCA_DYNAMICS

// Generic dynamic system & ODE solving tools
#include "DynamicSystem"
#include "ODESolvers"

// Rigid-body dynamics
#include "rigid-body/RigidBody"
#include "rigid-body/ConstantOperators"
#include "rigid-body/SpringOperators"
#include "rigid-body/GravityOperators"

#endif
