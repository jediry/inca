#pragma once
#ifndef INCA_UTIL_IS_COLLECTION
#define INCA_UTIL_IS_COLLECTION


#include "macros.hpp"

namespace inca {
    template <class T, typename Enable = void>
    struct is_collection {
        typedef ::boost::mpl::false_ type;
        static const bool value = type::value;
    };
    template <class T>
    struct is_collection<T, ENABLE_IF( IS_SAME( typename T::const_iterator,
                                                typename T::const_iterator ) ) > {
        typedef ::boost::mpl::true_ type;
        static const bool value = type::value;
    };
};

// Clean up the preprocessor's namespace
#define UNDEFINE_INCA_METAPROGRAMMING_MACROS
#include "macros.hpp"

#endif
