/** -*- C++ -*-
 *
 * \file Nothing.hpp
 *
 * \author Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

#ifndef INCA_UTIL_METAPROGRAMMING_NOTHING
#define INCA_UTIL_METAPROGRAMMING_NOTHING

// Import metaprogramming macros
#include "macros.hpp"

// This is part of the Inca utilities collection
namespace inca {
    // The "nothing" placeholder class
    class Nothing {
        // Dummy assignment operator
        Nothing & operator=(Nothing) { return *this; }

        // String conversion for use with std::ostream << operator
        operator const char *() { return "Nothing"; }
    };

    // Metafunction to test whether a class is "nothing"
    BOOLEAN_METAFUNCTION( is_nothing, IS_SAME( T, Nothing ) )
};

// Clean up the preprocessor namespace
#define UNDEFINE_INCA_METAPROGRAMMING_MACROS
#include "macros.hpp"

#endif

