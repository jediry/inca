/*
 * File: concepts
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The semantics of the Raster concept require that assignment be a
 *      lightweight, constant-time operation. For many implementations this
 *      is trivial because they contain very little data (e.g., most raster
 *      operator classes). For implementations that occupy a large amount of
 *      memory, assignment should be reference-counted and only actually copied
 *      when one of the references is written to.
 */

#ifndef INCA_RASTER_CONCEPTS
#define INCA_RASTER_CONCEPTS

// Import system configuration
#include <inca/inca-common.h>


// Import Raster concept definitions and associated tags
#include "concepts/RasterConcept"

// Allowable values for the semantic tag "AccessTag"
#include "concepts/ReadableRasterConcept"
#include "concepts/WritableRasterConcept"
#include "concepts/ReadWriteRasterConcept"

// Allowable values for the semantic tag "SizeTag"
#include "concepts/ArbitrarySizeRasterConcept"
#include "concepts/FixedSizeRasterConcept"
#include "concepts/MutableSizeRasterConcept"

// Allowable values for the semantic tag "BoundsTag"
#include "concepts/StationaryRasterConcept"
#include "concepts/MovableRasterConcept"


// Import metaprogramming tools
#include <inca/util/metaprogramming/Nothing.hpp>
#include <inca/util/metaprogramming/macros.hpp>

// This is part of the Inca raster processing library
namespace inca {
    namespace raster {

        // Metafunction returning the dimensionality of a raster.
        // If the argument is not a raster, it returns negative one.
        // If the argument is Nothing, the NothingValue is returned.
        template <typename T, inca::SizeType NothingValue = -1, typename Enabled = void>
        struct raster_dimensionality {
            // Specialization for Nothing
            typedef ::boost::mpl::integral_c_tag tag;
            typedef raster_dimensionality type;
            typedef SizeType value_type;
            static const SizeType value = NothingValue;
        };
#if 0
        template <typename T>
        struct raster_dimensionality<T, -1, ENABLE_IF( is_raster<T> ) > {
            // Specialization for raster (default NothingValue)
            typedef ::boost::mpl::integral_c_tag tag;
            typedef raster_dimensionality type;
            typedef SizeType value_type;
            static const SizeType value = T::dimensionality;
        };
#endif
        template <typename T, inca::SizeType NothingValue>
        struct raster_dimensionality<T, NothingValue, ENABLE_IF( is_raster<T> ) > {
            // Specialization for raster (specifying NothingValue)
            typedef ::boost::mpl::integral_c_tag tag;
            typedef raster_dimensionality type;
            typedef SizeType value_type;
            static const SizeType value = T::dimensionality;
        };
        template <typename T, inca::SizeType NothingValue>
        struct raster_dimensionality<T, NothingValue,
                ENABLE_IF( AND2( NOT( is_raster<T> ), NOT( is_nothing<T> ) ) ) > {
            // Specialization for non-raster (specifying NothingValue)
            typedef ::boost::mpl::integral_c_tag tag;
            typedef raster_dimensionality type;
            typedef SizeType value_type;
            static const SizeType value = 0;
        };


        // Metafunction returning the element type of a raster.
        // If the argument is not a raster, the type of the argument is returned.
        template <typename T, typename Enabled = void>
        struct raster_element_type {
            // Specialization for non-raster
            typedef T type;
        };
        template <typename T>
        struct raster_element_type<T, ENABLE_IF( is_raster<T> ) > {
            // Specialization for raster
            typedef typename T::ElementType type;
        };


        // Metafunction to choose the minimum dimensionality among all arguments
        template <typename T0 = Nothing, typename T1 = Nothing,
                  typename T2 = Nothing, typename T3 = Nothing>
        struct minimum_dimensionality {
            typedef ::boost::mpl::integral_c_tag tag;
            typedef SizeType value_type;
            static const SizeType huge = 9999;
            static const SizeType min = MIN4( raster_dimensionality<T0 COMMA huge>,
                                              raster_dimensionality<T1 COMMA huge>,
                                              raster_dimensionality<T2 COMMA huge>,
                                              raster_dimensionality<T3 COMMA huge> )::value;
            static const SizeType value = IF( EQUAL( INT(min), INT(huge) ),
                                              INT(-1),
                                              INT(min) )::value;
        };


        // Metafunction to choose the maximum dimensionality among all arguments
        template <typename T0 = Nothing, typename T1 = Nothing,
                  typename T2 = Nothing, typename T3 = Nothing>
        struct maximum_dimensionality {
            typedef ::boost::mpl::integral_c_tag tag;
            typedef SizeType value_type;
            static const SizeType value = MAX4( raster_dimensionality<T0>,
                                                raster_dimensionality<T1>,
                                                raster_dimensionality<T2>,
                                                raster_dimensionality<T3> )::value;
        };

    }
}

// Clean up the preprocessor's namespace
#define UNDEFINE_INCA_METAPROGRAMMING_MACROS
#include <inca/util/metaprogramming/macros.hpp>

#endif
