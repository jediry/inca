/*
 * File: inca-common.h
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains common includes and compile-time option
 *      declarations. Other modules should utilize these definitions and
 *      minimize the number of direct #includes of external libraries.
 *
 *      On UNIX-based platforms, this includes the autoconf-generated
 *      config-ac.h to determine the values of these options.
 *
 *      On Windoze, this includes config-win32.h, which should be generated by
 *      hand. A curse on Redmond for not having such a wonderful tool as
 *      autoconf.
 */

#ifndef INCA_COMMON_H
#define INCA_COMMON_H

// Choose platform-specific configuration
#if   defined(__GNUC__)
#	include "config-ac.h"
#elif defined(_MSC_VER)
#	include "config-win32.h"
#endif


// Include C++ stdlib's and Boost's smart(er) pointers
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
namespace inca {
    using std::auto_ptr;
    using boost::scoped_ptr;
    using boost::scoped_array;
    using boost::shared_ptr;
    using boost::shared_array;
    using boost::weak_ptr;
    using boost::dynamic_pointer_cast;
    using boost::static_pointer_cast;
};


// Include C++ standard library stuff
#include <string>
namespace inca {
    using std::string;
};

#include <iostream>
#include <iomanip>
namespace inca {
    using std::cin;     using std::endl;
    using std::cout;    using std::dec;
    using std::cerr;    using std::hex;
    using std::ostream;
};

#include <fstream>
namespace inca {
    using std::ifstream;
    using std::ofstream;
};

#include <sstream>
namespace inca {
    using std::stringstream;
    using std::ostringstream;
};


// Include C++ STL and Boost containers
#include <vector>
#include <list>
#include <set>
#include <boost/multi_array.hpp>
#include <inca/util/hash_container>     // Hash containers are non-standard
namespace inca {
    using std::pair;
    using std::vector;
    using std::list;
    using std::set;
    using boost::multi_array;

    // Import hash containers from whatever namespace they live in
    using stl_ext::hash_map;
    using stl_ext::hash_set;
};


// Declare special primitive types
namespace inca {
    typedef int index_t;                // Index into a list
    typedef unsigned short  uchar;      // Unicode character
};

// Everything in the system should have access to the Logger instance
#include <inca/util/Logger.hpp>

// We want to make accessible our C#-style properties
#include <inca/util/property>

#endif  /* End of multi-include protection */
