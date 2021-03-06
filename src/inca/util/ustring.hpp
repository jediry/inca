/*
 * File: ustring.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The ustring class contains a generic string (which may be text or
 *      numeric data), much like a 'scalar' variable in Perl or JavaScript.
 *      The textual representation can be accessed as either Unicode or the
 *      native codepage. This specialized string is useful primarily for
 *      converting between the Unicode text used by the XML parser and numeric
 *      data or regular ASCII strings.
 */

#pragma once
#ifndef INCA_UTIL_UNICODE_STRING
#define INCA_UTIL_UNICODE_STRING

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca utilities collection
namespace inca {
    // Forward declarations
    class ustring;
};


class inca::ustring {
/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    ustring();
    ustring(const ustring &s);
    ustring(bool b);
    ustring(int i);
    ustring(double d);
    ustring(const std::string &s);
    ustring(const char * const s);
    ustring(const Unicode * const s);


/*---------------------------------------------------------------------------*
 | Type conversion functions
 *---------------------------------------------------------------------------*/
public:
    operator bool() const;
    operator int() const;
    operator double() const;
//    operator const char*() const;
    operator const std::string &() const;
    operator const Unicode*() const;

protected:
    // Cached mutable data sync functions
    void regenerateNumber() const;
    void regenerateUnicode() const;


/*---------------------------------------------------------------------------*
 | String concatenation operators
 *---------------------------------------------------------------------------*/
public:
    const ustring operator+(bool b) const ;
    const ustring operator+(int i) const;
    const ustring operator+(double d) const;
    const ustring operator+(const std::string &s) const;
    const ustring operator+(const char * const s) const;
    const ustring operator+(const Unicode * const s) const;
    const ustring operator+(const ustring &s) const;


/*---------------------------------------------------------------------------*
 | String comparison operators
 *---------------------------------------------------------------------------*/
public:
    bool operator==(bool b) const;
    bool operator==(int i) const;
    bool operator==(double d) const;
    bool operator==(const std::string & s) const;
    bool operator==(const char * const s) const;
    bool operator==(const Unicode * const s) const;
    bool operator==(const ustring & s) const;

    bool operator!=(bool b) const { return ! operator==(b); }
    bool operator!=(int i) const { return ! operator==(i); }
    bool operator!=(double d) const { return ! operator==(d); }
    bool operator!=(const std::string & s) const { return ! operator==(s); }
    bool operator!=(const char * const s) const { return ! operator==(s); }
    bool operator!=(const Unicode * const s) const {
        return ! operator==(s);
    }
    bool operator!=(const ustring &s) const { return ! operator==(s); }

private:
    std::string ascii;          // This is the "master" ascii string

    mutable std::basic_string<Unicode> unicode;
    mutable double number;      // These are declared 'mutable' so that they
    mutable bool numberValid;   // may be lazily evaluated
    mutable bool unicodeValid;
};

// Stream representation output
std::ostream & operator<<(std::ostream &o, const inca::ustring &x);

#endif
