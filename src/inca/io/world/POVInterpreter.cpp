/*
 * File: POVInterpreter.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The POVInterpreter class handles parsing data scene data stored using
 *      the POV-Ray Scene Description Language. The contents of the .pov file
 *      are converted to/from an XML DOM tree.
 *
 *      The XML DOM document is used as an intermediary representation between
 *      the actual scene objects and the scene file.
 */

// Include precompiled header
#include <inca/precomp.h>

// Include class definitions
#include "SceneInterpreters.hpp"
using Inca::IO::POVInterpreter;
