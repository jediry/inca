/*
 * File: UtilPropertyTest
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2002, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The UtilPropertyTest class tests the Inca utility library's
 *      implementation of C#-like properties.
 *
 * Implementation note:
 *      This file is designed to be included by IncaTestMain.cpp, and may not
 *      work correctly otherwise, as it depends on IncaTestMain.cpp already
 *      having included some other things.
 */

#ifndef TEST_UTIL_PROPERTY
#define TEST_UTIL_PROPERTY

class UtilPropertyTest : public CppUnit::TestFixture {
private:
    // Convenience typedefs
    typedef UtilPropertyTest ThisTest;


/*---------------------------------------------------------------------------*
 | Test configuration
 *---------------------------------------------------------------------------*/
public:
    void setUp() {
//        intPropRO.init(this);
//        strPropRW.init(this);
    }

    void tearDown() {

    }

    // Constructor -- needed for property initialization
    UtilPropertyTest() : intPropRO(this), strPropRW(this, "stop"),
        str0PtrPropRO(this), str1PtrPropRW(this),
        str2PtrPropRO(this), doublePtrPropRW(this) { }
    typedef ThisTest ThisType;

    // By-value properties
    ro_property(int, intPropRO, 1);
    rw_property(string, strPropRW, "yo");

    // Pointer properties
    ro_ptr_property(string, str0PtrPropRO, new string("Who's"));
    rw_ptr_property(string, str1PtrPropRW, new string("your"));
    ro_ptr_property(string, str2PtrPropRO,
                            shared_ptr<string>(new string("daddy?")));
    rw_ptr_property(double, doublePtrPropRW, shared_ptr<double>(new double(10.5)));

    // List properties
//    ro_list_property(int, intListPropRO);
//    rw_list_property(double, doubleListPropRW);

    // Create the CppUnit test suite
    CPPUNIT_TEST_SUITE(ThisTest);
        CPPUNIT_TEST(testPropRO);
        CPPUNIT_TEST(testPropRW);
        CPPUNIT_TEST(testPtrPropRO);
        CPPUNIT_TEST(testPtrPropRW);
//        CPPUNIT_TEST(testListPropRO);
//        CPPUNIT_TEST(testListPropRW);
    CPPUNIT_TEST_SUITE_END();


/*---------------------------------------------------------------------------*
 | Individual tests
 *---------------------------------------------------------------------------*/
    void testPropRO() {
        // Test accessor functions, make sure default was set correctly
        CPPUNIT_ASSERT(intPropRO == 1);         // Implicit, type-cast accessor
        CPPUNIT_ASSERT(intPropRO() == 1);       // Explicit, functor accessor
        CPPUNIT_ASSERT(_intPropRO == 1);        // Underlying (protected) variable

        // Change underlying variable, make sure it's reflected externally
        _intPropRO = 5;
        CPPUNIT_ASSERT(intPropRO == 5);         // Implicit, type-cast accessor
        CPPUNIT_ASSERT(intPropRO() == 5);       // Explicit, functor accessor
        CPPUNIT_ASSERT(_intPropRO == 5);        // Underlying (protected) variable
    }

    void testPropRW() {
        // Test accessor functions
//        CPPUNIT_ASSERT(strPropRW == string("stop"));    // Implicit, type-cast accessor
        CPPUNIT_ASSERT(strPropRW() == string("stop"));  // Explicit, functor accessor
        CPPUNIT_ASSERT(_strPropRW == string("stop"));   // Underlying (protected) variable

        // Change underlying variable, make sure it's reflected externally
        _strPropRW = string("slow");
//        CPPUNIT_ASSERT(strPropRW == string("slow"));    // Implicit, type-cast accessor
        CPPUNIT_ASSERT(strPropRW() == string("slow"));  // Explicit, functor accessor
        CPPUNIT_ASSERT(_strPropRW == string("slow"));   // Underlying (protected) variable

        // Change via assignment to property, make sure it's reflected externally
        strPropRW = string("go");
//        CPPUNIT_ASSERT(strPropRW == string("go"));      // Implicit, type-cast accessor
        CPPUNIT_ASSERT(strPropRW() == string("go"));    // Explicit, functor accessor
        CPPUNIT_ASSERT(_strPropRW == string("go"));     // Underlying (protected) variable
    }

    void testPtrPropRO() {
        str0PtrPropRO();
        str1PtrPropRW();
        str2PtrPropRO();

        // Test constructors/comparison functions
        CPPUNIT_ASSERT(*str0PtrPropRO == "Who's");      // Pointer dereference * accessor
        CPPUNIT_ASSERT(*str1PtrPropRW() == "your");     // Explicit, functor accessor
        CPPUNIT_ASSERT(*_str2PtrPropRO == "daddy?");    // Underlying (protected) variable
        CPPUNIT_ASSERT(str1PtrPropRW->c_str() == string("your"));// Pointer dereference -> accessor

        // Change underlying variable, make sure it's reflected externally
        _str2PtrPropRO = shared_ptr<string>(new string("mommy?"));
        CPPUNIT_ASSERT(*str0PtrPropRO == "Who's");      // Implicit, type-cast accessor
        CPPUNIT_ASSERT(*_str1PtrPropRW == "your");      // Underlying (protected) variable
        CPPUNIT_ASSERT(*str2PtrPropRO() == "mommy?");   // Explicit, functor accessor
    }

    void testPtrPropRW() {
        // Test constructors/comparison functions
//        CPPUNIT_ASSERT(doublePtrPropRW == _doublePtrPropRW);    // Implicit, type-cast accessor
        CPPUNIT_ASSERT(*doublePtrPropRW == 10.5);       // Pointer dereference accessor
        CPPUNIT_ASSERT(*(doublePtrPropRW()) == 10.5);   // Explicit, functor accessor
        CPPUNIT_ASSERT(*_doublePtrPropRW == 10.5);      // Underlying (protected) variable

        // Change via assignment to property, make sure it's reflected externally
        str1PtrPropRW = new string("my");
        CPPUNIT_ASSERT(*str1PtrPropRW() == "my");       // Explicit, functor accessor
        doublePtrPropRW = shared_ptr<double>(new double(-0.111));
        CPPUNIT_ASSERT(*doublePtrPropRW == -0.111);     // Pointer dereference accessor
    }

#if 0
    void testListPropRO() {
        // Test constructors/comparison functions
        CPPUNIT_ASSERT();
    }

    void testListPropRW() {
        // Test constructors/comparison functions
        CPPUNIT_ASSERT(doublePtrPropRW == 10.5);
    }
#endif
};

#endif
