/*
 * File: unit_test_main.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This is the entry point for the CppUnit test harness. This should
 *      instantiate each test suite that should be run. If you want to
 *      exclude some tests, because, for example, you know they won't compile
 *      (this of course NEVER happens in MY code :-), simply comment out
 *      the appropriate #define
 */

// Import system configuration
#include <inca/inca-common.h>
using namespace inca;

// Import CppUnit classes and macros
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/HelperMacros.h>


// Comment out these symbol definitions to exclude specific test sets
#define TEST_INCA_UTIL_PROPERTY         0
#define TEST_INCA_UTIL_CONTAINERS       0
#define TEST_INCA_MATH                  0


// Inca implementation of C#-like properties
#if TEST_INCA_UTIL_PROPERTY
#   include <inca/util/property>
#   include "UtilPropertyTest.hpp"
#endif

// Inca util containers
#if TEST_INCA_UTIL_CONTAINERS
#   include "UtilMultiArrayTest"
#endif

// inca::math library
#if TEST_INCA_MATH
#   include <inca/math.hpp>
#   include "MathScalarTest"
#   include "MathVectorTypesTest"
#   include "MathVectorOpsTest"
#endif


int main(int argc, char **argv) {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::Outputter * outputter
        = new CppUnit::TextOutputter(&runner.result(), cout);
    runner.setOutputter(outputter);


/*---------------------------------------------------------------------------*
 | Inca utility library tests
 *---------------------------------------------------------------------------*/
#if TEST_INCA_UTIL_PROPERTY
    runner.addTest(UtilPropertyTest::suite());
#endif
#if TEST_INCA_UTIL_CONTAINERS
    runner.addTest(UtilMultiArrayTest<int, 2>::suite());
    runner.addTest(UtilMultiArrayTest<char, 4>::suite());
    runner.addTest(UtilMultiArrayTest<double, 6>::suite());
    runner.addTest(UtilMultiArrayTest<char, 9>::suite());
#endif

/*---------------------------------------------------------------------------*
 | inca::math library tests
 *---------------------------------------------------------------------------*/
#if TEST_INCA_MATH
    // Test the various math models
//    runner.addTest(MathScalarTest<int>::suite());
//    runner.addTest(MathScalarTest<char>::suite());
//    runner.addTest(MathScalarTest<unsigned char>::suite());
//    runner.addTest(MathScalarTest<signed char>::suite());
//    runner.addTest(MathScalarTest<short>::suite());
//    runner.addTest(MathScalarTest<unsigned short>::suite());
//    runner.addTest(MathScalarTest<int>::suite());
//    runner.addTest(MathScalarTest<unsigned int>::suite());
//    runner.addTest(MathScalarTest<long>::suite());
//    runner.addTest(MathScalarTest<unsigned long>::suite());
//    runner.addTest(MathScalarTest<long long>::suite());
//    runner.addTest(MathScalarTest<unsigned long long>::suite());
    runner.addTest(MathScalarTest<float>::suite());
    runner.addTest(MathScalarTest<double>::suite());
    runner.addTest(MathScalarTest<long double>::suite());

    // Test the vector type templates with various math models and dimensions
    runner.addTest(MathVectorTypesTest<double, 2>::suite());
    runner.addTest(MathVectorTypesTest<float, 3>::suite());
//    runner.addTest(MathVectorTypesTest<int, 4>::suite());
//    runner.addTest(MathVectorTypesTest<unsigned int, 5>::suite());
    runner.addTest(MathVectorTypesTest<long double, 6>::suite());
//    runner.addTest(MathVectorTypesTest<long long, 7>::suite());

    // Test the vector operations with various math models and dimensions
//    runner.addTest(MathVectorOpsTest<char, 2>::suite());
//    runner.addTest(MathVectorOpsTest<long, 3>::suite());
    runner.addTest(MathVectorOpsTest<float, 4>::suite());
    runner.addTest(MathVectorOpsTest<double, 5>::suite());
    runner.addTest(MathVectorOpsTest<long double, 6>::suite());
//    runner.addTest(MathVectorOpsTest<long long, 7>::suite());
#endif


    // Finally, let's go do it
    bool result = runner.run("");
    return ! result;
}
