/*
 * File: ustring.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the ustring class, defined in ustring.hpp.
 */


// Import the class definition
#include "ustring.hpp"
using namespace inca;

// Import the XML parser stuff that we need to do code-page conversion
#if XML_SUPPORT_ENABLED
#   if USING_XERCES_PARSER
#       include <xercesc/util/PlatformUtils.hpp>
#       include <xercesc/util/XMLString.hpp>
#   endif
#endif

#if __GNUC__
/*****************************************************************************
 * Implementation of char_traits for uchar, which the SGI STL doesn't implement,
 * which isn't surprising, as uchar is really an unsigned short. We need this
 * in order to make a basic_string out of uchar.
 *****************************************************************************/
void std::char_traits<uchar>::assign(uchar &c1, const uchar &c2) {
    c1 = c2;
}

bool std::char_traits<uchar>::eq(const uchar &c1, const uchar &c2) {
    return c1 == c2;
}

bool std::char_traits<uchar>::lt(const uchar &c1, const uchar &c2) {
    return c1 < c2;
}

int std::char_traits<uchar>::compare(const uchar *s1, const uchar *s2, size_t n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
    }
    return 0;
}
    
size_t std::char_traits<uchar>::length(const uchar *s) {
    size_t size;
    for (size = 0; s[size] != 0; size++) /* */;
    return size;
}

const uchar * std::char_traits<uchar>::find(const uchar *s, size_t n, const uchar &c) {
    size_t size;
    for (size = 0; s[size] != c && size < n; size++) /* */;
    return (uchar*)(s + size);
}

uchar * std::char_traits<uchar>::move(uchar *dst, const uchar *src, size_t n) {
    for (size_t i = 0; i < n; i++)
        dst[i] = src[i];
    return dst;
}

uchar * std::char_traits<uchar>::copy(uchar *dst, const uchar *src, size_t n) {
    for (size_t i = 0; i < n; i++)
        dst[i] = src[i];
    return dst;
}

uchar * std::char_traits<uchar>::assign(uchar *dst, size_t n, uchar c) {
    for (size_t i = 0; i < n; i++)
        dst[i] = c;
    return dst;
}

uchar std::char_traits<uchar>::to_char_type(const unsigned long &c) {
    return (uchar)c;
}

unsigned long std::char_traits<uchar>::to_int_type(const uchar &c) {
    return (unsigned long)c;
}
#endif


/*****************************************************************************
 * Implementation of ustring, which converts between 16-bit unicode characters,
 * 8-bit ASCII characters, and numeric data types.
 *****************************************************************************/
#define MAX_INTEGER_LENGTH  15
#define MAX_DOUBLE_LENGTH   35


/*---------------------------------------------------------------------------*
 | Constructors/destructor
 *---------------------------------------------------------------------------*/
// Default constructor
ustring::ustring() : numberValid(false), unicodeValid(false), ascii() { }

// Copy constructor
ustring::ustring(const ustring &s) {
    ascii = s.ascii;                // Copy over guaranteed-valid fields
    numberValid = s.numberValid;
    unicodeValid = s.unicodeValid;

    if (numberValid)    number = s.number;
    if (unicodeValid)   unicode = s.unicode;
}

// Copy/assignment from boolean
ustring::ustring(bool b) {
    if (b) {
        number = 1;
        ascii = "true";
    } else {
        number = 0;
        ascii = "false";
    }

    // Synchronize the cached state of the object
    numberValid = true;
    unicodeValid = false;
}

// Copy/assignment from integer
ustring::ustring(int i) {
    number = i;
    char temp[MAX_INTEGER_LENGTH];
    sprintf(temp, "%d", i); // Convert to a char * string
    ascii = temp;           // Translate into an STL string
 
    // Synchronize the cached state of the object
    numberValid = true;
    unicodeValid = false;
}

// Copy/assignment from double
ustring::ustring(double d) {
    number = d;
    char temp[MAX_DOUBLE_LENGTH];
    sprintf(temp, "%f", d); // Convert to a char * string
    ascii = temp;      // Translate into an STL string
 
    // Synchronize the cached state of the object
    numberValid = true;
    unicodeValid = false;
}

// Copy/assignment from ASCII string
ustring::ustring(const string &s) {
    ascii = s;
 
    // Synchronize the cached state of the object
    numberValid = false;
    unicodeValid = false;
}

// Copy/assignment from C-style ASCII string
ustring::ustring(const char * const s) {
    if (s == NULL)  // Handle the case where this is NULL
        ascii = "";
    else
        ascii = s;
 
    // Synchronize the cached state of the object
    numberValid = false;
    unicodeValid = false;
}

// Copy/assignment from Unicode string
ustring::ustring(const uchar * const s) {
    if (s == NULL) {    // Handle the case where this is NULL
        ascii = "";
        unicodeValid = false;
    } else {
#if XML_SUPPORT_ENABLED
        unicode = s;
        ascii = XMLString::transcode(s);
#else
        int length = unicode.length();
        ascii.clear();
        ascii.reserve(length);
        for (int i = 0; i < length; i++)
            ascii.push_back(static_cast<char>(s[i]));
#endif
        unicodeValid = true;
    }

    // Synchronize the cached state of the object
    numberValid = false;
}


/*---------------------------------------------------------------------------*
 | Type conversion functions
 *---------------------------------------------------------------------------*/
ustring::operator bool() const {
    regenerateNumber();
    return (number != 0);   // Avoid explicit cast-to-bool, as MSVC warns
}

ustring::operator int() const {
    regenerateNumber();
    return static_cast<int>(number);
}

ustring::operator double() const {
    regenerateNumber();
    return number;
}

//ustring::operator const char*() const { return ascii.c_str(); }

ustring::operator string() const { return ascii; }

ustring::operator const uchar*() const {
    regenerateUnicode();
    return unicode.c_str();
}

inline void ustring::regenerateNumber() const {
    if ( ! numberValid)
        number = (atof(ascii.c_str()));
}

inline void ustring::regenerateUnicode() const {
    if (! unicodeValid) {
#if XML_SUPPORT_ENABLED
        unicode = XMLString::transcode(ascii.c_str());
#else       // Just copy directly over
        const char *str = ascii.c_str();
        int length = strlen(str);
        unicode.clear();
        unicode.reserve(length);
        for (int i = 0; i < length; i++)
            unicode.push_back(static_cast<uchar>(str[i]));
#endif
    }
}


/*---------------------------------------------------------------------------*
 | String concatenation operators
 *---------------------------------------------------------------------------*/
const ustring ustring::operator+(bool b) const {
    return ustring(ascii + (b ? "true" : "false"));
}
const ustring ustring::operator+(int i) const {
    char temp[MAX_INTEGER_LENGTH];
    sprintf(temp, "%d", i); // Convert to a char * string
    return ustring(ascii + temp);
}
const ustring ustring::operator+(double d) const {
    char temp[MAX_DOUBLE_LENGTH];
    sprintf(temp, "%f", d); // Convert to a char * string
    return ustring(ascii + temp);
}
const ustring ustring::operator+(const string &s) const {
    return ustring(ascii + s);
}
const ustring ustring::operator+(const char * const s) const {
    return ustring(ascii + s);
}
const ustring ustring::operator+(const uchar * const s) const {
    return ustring((unicode + s).c_str());
}
const ustring ustring::operator+(const ustring &s) const {
    return ustring(ascii + s.ascii);
}


/*---------------------------------------------------------------------------*
 | String comparison operators
 *---------------------------------------------------------------------------*/
bool ustring::operator==(bool b) const {
    regenerateNumber();
    return b == (number != 0);  // Avoid explicit cast-to-bool, as MSVC warns
}

bool ustring::operator==(int i) const {
    regenerateNumber();
    return (i == static_cast<int>(number));
}

bool ustring::operator==(double d) const {
    regenerateNumber();
    return (d == number);
}

bool ustring::operator==(const string &s) const {
    return (ascii == s);
}

bool ustring::operator==(const char * const s) const {
    return (ascii == s);
}

bool ustring::operator==(const uchar *const s) const {
    return (*this == ustring(s));
}

bool ustring::operator==(const ustring &s) const {
    return (ascii == s.ascii);
}


// Stream representation output
std::ostream & operator<<(std::ostream &o, const ustring &s) {
    return o << static_cast<std::string>(s);
}
