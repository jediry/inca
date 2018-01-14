/*
 * File: raster.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including and
 *      configuring the raster processing library.
 */

#pragma once
#ifndef INCA_RASTER
#define INCA_RASTER

// Include raster concepts, algorithms, and metafunctions
#include "raster/concepts.hpp"
//#include "raster/algorithms.hpp"

// Import essential raster implementations
#include "raster/MultiArrayRaster"

// Algorithms working on Raster objects

// Raster operator lambda-expression construction
//#include "raster/operators/arithmetic"
//#include "raster/operators/selection"
//#include "raster/operators/transformation"

#endif
