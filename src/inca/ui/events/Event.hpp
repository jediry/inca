/** -*- C++ -*-
 *
 * \file    Event.hpp
 * 
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Event class is the root of the Inca event hierarchy, and contains
 *      the properties common to all event types (e.g., the time at which the
 *      event occurred. It cannot be directly instantiated.
 *
 *      Accordingly, there is no listener interface corresponding to Event.
 */

#pragma once
#ifndef INCA_UI_EVENT
#define INCA_UI_EVENT

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class Event;
    };
};

// Import time function definitions
#include <inca/util/Timer>

// HACK Import math types -- this should be using just Array
#include <inca/math.hpp>


class inca::ui::Event {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
public:
    typedef clock_t         Timestamp;
    typedef math::Point<IndexType, 2> Pixel;     // X,Y pair of integer coords
    typedef math::Vector<SizeType, 2> Dimension; // W,H pair of unsigned dims


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
protected:
    // Protected constructor
    explicit Event(Timestamp ts) : _timestamp(ts) { }


/*---------------------------------------------------------------------------*
 | Event property accessors
 *---------------------------------------------------------------------------*/
public:
    Timestamp timestamp() const { return _timestamp; }
    
protected:
    Timestamp _timestamp;
};

#endif
