/* -*- C++ -*-
 *
 * File: RasterMetafunctionTest
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *
 * Implementation note:
 *      This file is designed to be included by IncaTestMain.cpp, and may not
 *      work correctly otherwise, as it depends on IncaTestMain.cpp already
 *      having included some other things.
 */

#ifndef TEST_RASTER_METAFUNCTION
#define TEST_RASTER_METAFUNCTION


using namespace inca::raster;


// Import metaprogramming tools
#include <inca/util/metaprogramming/macros.hpp>


class RasterMetafunctionTest : public CppUnit::TestFixture {
private:
    // Convenience typedefs
    typedef RasterMetafunctionTest ThisTest;

    // Rasters of different types
    typedef MultiArrayRaster<float, 1> R1;
    typedef MultiArrayRaster<float, 2> R2;
    typedef MultiArrayRaster<float, 3> R3;
    typedef MultiArrayRaster<float, 4> R4;


public:

    // Create CppUnit test suite
    CPPUNIT_TEST_SUITE(ThisTest);
        // Print a nice, friendly header for this suite
        CPPUNIT_TEST(beginSuite);

        // Dimensionality metafunction tests
        CPPUNIT_TEST(test_raster_dimensionality);
        CPPUNIT_TEST(test_minimum_dimensionality);
        CPPUNIT_TEST(test_maximum_dimensionality);

        // Print a nice, friendly footer for this suite
        CPPUNIT_TEST(endSuite);
    CPPUNIT_TEST_SUITE_END();


/*---------------------------------------------------------------------------*
 | Test suite setup
 *---------------------------------------------------------------------------*/
public:
    void beginSuite() {
        cerr << "Testing Raster Metafunctions: ";
    }

    void endSuite() {
        cerr << endl;
    }


/*---------------------------------------------------------------------------*
 | Comparison operator tests
 *---------------------------------------------------------------------------*/
public:
    void test_raster_dimensionality() {
        // Dimensionality of Nothing is arbitrary
        CPPUNIT_ASSERT( raster_dimensionality<Nothing>::value == -1 );
        CPPUNIT_ASSERT( raster_dimensionality<Nothing COMMA 9999>::value == 9999 );

        // Dimensionality of non-raster is always zero
        CPPUNIT_ASSERT( raster_dimensionality<float>::value == 0 );
        CPPUNIT_ASSERT( raster_dimensionality<float COMMA 9999>::value == 0 );

        // Dimensionality of raster is always the raster's dimensionality
        CPPUNIT_ASSERT( raster_dimensionality<R1>::value == 1 );
        CPPUNIT_ASSERT( raster_dimensionality<R2>::value == 2 );
        CPPUNIT_ASSERT( raster_dimensionality<R3>::value == 3 );
        CPPUNIT_ASSERT( raster_dimensionality<R4>::value == 4 );
        CPPUNIT_ASSERT( raster_dimensionality<R1 COMMA 9999>::value == 1 );
        CPPUNIT_ASSERT( raster_dimensionality<R2 COMMA 9999>::value == 2 );
        CPPUNIT_ASSERT( raster_dimensionality<R3 COMMA 9999>::value == 3 );
        CPPUNIT_ASSERT( raster_dimensionality<R4 COMMA 9999>::value == 4 );
    }

    void test_minimum_dimensionality() {
        // Test the usual functioning
        CPPUNIT_ASSERT( minimum_dimensionality<>::value == -1 );
        CPPUNIT_ASSERT( minimum_dimensionality<R4>::value == 4 );
        CPPUNIT_ASSERT( minimum_dimensionality<R4 COMMA R3>::value == 3 );
        CPPUNIT_ASSERT( minimum_dimensionality<R4 COMMA R3 COMMA R2>::value == 2 );
        CPPUNIT_ASSERT( minimum_dimensionality<R4 COMMA R3 COMMA R2 COMMA R1>::value == 1 );

        // Throw in some scalar types
        CPPUNIT_ASSERT( minimum_dimensionality<float>::value == 0 );
        CPPUNIT_ASSERT( minimum_dimensionality<R1 COMMA float>::value == 0 );
        CPPUNIT_ASSERT( minimum_dimensionality<float COMMA R3 COMMA R4>::value == 0 );

        // Try to confuse it with a bunch of nothings
        CPPUNIT_ASSERT( minimum_dimensionality<Nothing COMMA R1>::value == 1 );
        CPPUNIT_ASSERT( minimum_dimensionality<R2 COMMA Nothing COMMA R3>::value == 2 );
        CPPUNIT_ASSERT( minimum_dimensionality<Nothing COMMA R4 COMMA R2 COMMA R2>::value == 2 );
        CPPUNIT_ASSERT( minimum_dimensionality<R4 COMMA R2 COMMA R3 COMMA R1>::value == 1 );
    }

    void test_maximum_dimensionality() {
        // Test the usual functioning
        CPPUNIT_ASSERT( maximum_dimensionality<>::value == -1 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1>::value == 1 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1 COMMA R2>::value == 2 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1 COMMA R2 COMMA R3>::value == 3 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1 COMMA R2 COMMA R3 COMMA R4>::value == 4 );

        // Throw in some scalar types
        CPPUNIT_ASSERT( maximum_dimensionality<float>::value == 0 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1 COMMA float>::value == 1 );
        CPPUNIT_ASSERT( maximum_dimensionality<float COMMA R3 COMMA R4>::value == 4 );

        // Try to confuse it with a bunch of nothings
        CPPUNIT_ASSERT( maximum_dimensionality<Nothing COMMA R1>::value == 1 );
        CPPUNIT_ASSERT( maximum_dimensionality<R1 COMMA Nothing COMMA R2>::value == 2 );
        CPPUNIT_ASSERT( maximum_dimensionality<Nothing COMMA R1 COMMA R2 COMMA R3>::value == 3 );
        CPPUNIT_ASSERT( maximum_dimensionality<R4 COMMA R2 COMMA R3 COMMA R1>::value == 4 );
    }
};

// Clean up the preprocessor namespace
#define UNDEFINE_INCA_METAPROGRAMMING_MACROS
#include <inca/util/metaprogramming/macros.hpp>

#endif
