/*
 * File: GL.hpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This is a C++ friendly wrapper around OpenGL. It places all of the
 *      OpenGL functions into the GL namespace, and adds overloaded versions
 *      of each of the functions that have multiple versions
 *      (e.g. glVertex3f(), glVertex3d(), et. al. -> glVertex()).
 */

#ifndef GL_HPP
#define GL_HPP

// If requested, import Inca types
#ifdef GL_HPP_IMPORT_INCA
#   include <inca/util/Array>
#   include <inca/math/linalg.hpp>
#   include <inca/math/color.hpp>
#endif

namespace GL {
    // Import OS-specific headers
    #if __MS_WINDOZE__
        // Windows OpenGL seems to need this
    #   include <windows.h>

        // I'd also rather VS didn't complain about casting to boolean
    //#   pragma warning (disable : 4800)
    #endif

    // Import OpenGL and GLU
    #include <GL/gl.h>
    #include <GL/glu.h>

    // If requested, import GLUT and GLUI
    #ifdef GL_HPP_IMPORT_GLUT
    #   include <GL/glut.h>
    #endif
    #ifdef GL_HPP_IMPORT_GLUI
    #   include <GL/glui.h>
    #endif

    // If we're also handling Inca types, import the namespaces
    #ifdef GL_HPP_IMPORT_INCA
        using namespace inca;
        using namespace inca::math;
    #endif    


    // glVertex* overloads
    void glVertex(GLshort x, GLshort y)                           { glVertex2s(x, y); }
    void glVertex(GLshort x, GLshort y, GLshort z)                { glVertex3s(x, y, z); }
    void glVertex(GLshort x, GLshort y, GLshort z, GLshort w)     { glVertex4s(x, y, z, w); }
    void glVertex(GLint x, GLint y)                               { glVertex2i(x, y); }
    void glVertex(GLint x, GLint y, GLint z)                      { glVertex3i(x, y, z); }
    void glVertex(GLint x, GLint y, GLint z, GLint w)             { glVertex4i(x, y, z, w); }
    void glVertex(GLfloat x, GLfloat y)                           { glVertex2f(x, y); }
    void glVertex(GLfloat x, GLfloat y, GLfloat z)                { glVertex3f(x, y, z); }
    void glVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat w)     { glVertex4f(x, y, z, w); }
    void glVertex(GLdouble x, GLdouble y)                         { glVertex2d(x, y); }
    void glVertex(GLdouble x, GLdouble y, GLdouble z)             { glVertex3d(x, y, z); }
    void glVertex(GLdouble x, GLdouble y, GLdouble z, GLdouble w) { glVertex4d(x, y, z, w); }
    #ifdef GL_HPP_IMPORT_INCA
        template <inca::SizeType size>
        void glVertex(const Point<GLshort, size> & v) {
            switch (size) {
                case 2: glVertex2sv(v.begin()); break;
                case 3: glVertex3sv(v.begin()); break;
                case 4: glVertex4sv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const Point<GLint, size> & v) {
            switch (size) {
                case 2: glVertex2iv(v.begin()); break;
                case 3: glVertex3iv(v.begin()); break;
                case 4: glVertex4iv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const Point<GLfloat, size> & v) {
            switch (size) {
                case 2: glVertex2fv(v.begin()); break;
                case 3: glVertex3fv(v.begin()); break;
                case 4: glVertex4fv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const Point<GLdouble, size> & v) {
            switch (size) {
                case 2: glVertex2dv(v.begin()); break;
                case 3: glVertex3dv(v.begin()); break;
                case 4: glVertex4dv(v.begin()); break;
            }
        }
    #endif    

    // glNormal* overloads
    void glNormal(GLbyte x, GLbyte y, GLbyte z)       { glNormal3b(x, y, z); }
    void glNormal(GLshort x, GLshort y, GLshort z)    { glNormal3s(x, y, z); }
    void glNormal(GLint x, GLint y, GLint z)          { glNormal3i(x, y, z); }
    void glNormal(GLfloat x, GLfloat y, GLfloat z)    { glNormal3f(x, y, z); }
    void glNormal(GLdouble x, GLdouble y, GLdouble z) { glNormal3d(x, y, z); }
    #ifdef GL_HPP_IMPORT_INCA
        void glNormal(const Vector<GLbyte, 3> & n)   { glNormal3bv(n.begin()); }
        void glNormal(const Vector<GLshort, 3> & n)  { glNormal3sv(n.begin()); }
        void glNormal(const Vector<GLint, 3> & n)    { glNormal3iv(n.begin()); }
        void glNormal(const Vector<GLfloat, 3> & n)  { glNormal3fv(n.begin()); }
        void glNormal(const Vector<GLdouble, 3> & n) { glNormal3dv(n.begin()); }
    #endif    

    // glColor* overloads
    void glColor(GLbyte r, GLbyte g, GLbyte b)                   { glColor3b(r, g, b); }
    void glColor(GLbyte r, GLbyte g, GLbyte b, GLbyte a)         { glColor4b(r, g, b, a); }
    void glColor(GLubyte r, GLubyte g, GLubyte b)                { glColor3ub(r, g, b); }
    void glColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)     { glColor4ub(r, g, b, a); }
    void glColor(GLshort r, GLshort g, GLshort b)                { glColor3s(r, g, b); }
    void glColor(GLshort r, GLshort g, GLshort b, GLshort a)     { glColor4s(r, g, b, a); }
    void glColor(GLushort r, GLushort g, GLushort b)             { glColor3us(r, g, b); }
    void glColor(GLushort r, GLushort g, GLushort b, GLushort a) { glColor4us(r, g, b, a); }
    void glColor(GLint r, GLint g, GLint b)                      { glColor3i(r, g, b); }
    void glColor(GLint r, GLint g, GLint b, GLint a)             { glColor4i(r, g, b, a); }
    void glColor(GLuint r, GLuint g, GLuint b)                   { glColor3ui(r, g, b); }
    void glColor(GLuint r, GLuint g, GLuint b, GLuint a)         { glColor4ui(r, g, b, a); }
    void glColor(GLfloat r, GLfloat g, GLfloat b)                { glColor3f(r, g, b); }
    void glColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)     { glColor4f(r, g, b, a); }
    void glColor(GLdouble r, GLdouble g, GLdouble b)             { glColor3d(r, g, b); }
    void glColor(GLdouble r, GLdouble g, GLdouble b, GLdouble a) { glColor4d(r, g, b, a); }
    #ifdef GL_HPP_IMPORT_INCA
        void glColor(const Color<GLbyte, sRGB<false> > &c)   { glColor3bv(c.begin()); }
        void glColor(const Color<GLbyte, sRGB<true> > &c)    { glColor4bv(c.begin()); }
        void glColor(const Color<GLubyte, sRGB<false> > &c)  { glColor3ubv(c.begin()); }
        void glColor(const Color<GLubyte, sRGB<true> > &c)   { glColor4ubv(c.begin()); }
        void glColor(const Color<GLshort, sRGB<false> > &c)  { glColor3sv(c.begin()); }
        void glColor(const Color<GLshort, sRGB<true> > &c)   { glColor4sv(c.begin()); }
        void glColor(const Color<GLushort, sRGB<false> > &c) { glColor3usv(c.begin()); }
        void glColor(const Color<GLushort, sRGB<true> > &c)  { glColor4usv(c.begin()); }
        void glColor(const Color<GLint, sRGB<false> > &c)    { glColor3iv(c.begin()); }
        void glColor(const Color<GLint, sRGB<true> > &c)     { glColor4iv(c.begin()); }
        void glColor(const Color<GLuint, sRGB<false> > &c)   { glColor3uiv(c.begin()); }
        void glColor(const Color<GLuint, sRGB<true> > &c)    { glColor4uiv(c.begin()); }
        void glColor(const Color<GLfloat, sRGB<false> > &c)  { glColor3fv(c.begin()); }
        void glColor(const Color<GLfloat, sRGB<true> > &c)   { glColor4fv(c.begin()); }
        void glColor(const Color<GLdouble, sRGB<false> > &c) { glColor3dv(c.begin()); }
        void glColor(const Color<GLdouble, sRGB<true> > &c)  { glColor4dv(c.begin()); }
    #endif

    // glIndex* overloads

    // glTexCoord* overloads
    void glTexCoord(GLshort u)                                  { glTexCoord1s(u); }
    void glTexCoord(GLshort u, GLshort v)                       { glTexCoord2s(u, v); }
    void glTexCoord(GLshort u, GLshort v, GLshort w)            { glTexCoord3s(u, v, w); }
    void glTexCoord(GLshort u, GLshort v, GLshort w, GLshort q) { glTexCoord4s(u, v, w, q); }
    void glTexCoord(GLint u)                            { glTexCoord1i(u); }
    void glTexCoord(GLint u, GLint v)                   { glTexCoord2i(u, v); }
    void glTexCoord(GLint u, GLint v, GLint w)          { glTexCoord3i(u, v, w); }
    void glTexCoord(GLint u, GLint v, GLint w, GLint q) { glTexCoord4i(u, v, w, q); }
    void glTexCoord(GLfloat u)                                  { glTexCoord1f(u); }
    void glTexCoord(GLfloat u, GLfloat v)                       { glTexCoord2f(u, v); }
    void glTexCoord(GLfloat u, GLfloat v, GLfloat w)            { glTexCoord3f(u, v, w); }
    void glTexCoord(GLfloat u, GLfloat v, GLfloat w, GLfloat q) { glTexCoord4f(u, v, w, q); }
    void glTexCoord(GLdouble u)                                     { glTexCoord1d(u); }
    void glTexCoord(GLdouble u, GLdouble v)                         { glTexCoord2d(u, v); }
    void glTexCoord(GLdouble u, GLdouble v, GLdouble w)             { glTexCoord3d(u, v, w); }
    void glTexCoord(GLdouble u, GLdouble v, GLdouble w, GLdouble q) { glTexCoord4d(u, v, w, q); }
    #ifdef GL_HPP_IMPORT_INCA
        void glTexCoord(const Point<GLshort, 1> & t) { glTexCoord1sv(t.begin()); }
        void glTexCoord(const Point<GLshort, 2> & t) { glTexCoord2sv(t.begin()); }
        void glTexCoord(const Point<GLshort, 3> & t) { glTexCoord3sv(t.begin()); }
        void glTexCoord(const Point<GLshort, 4> & t) { glTexCoord4sv(t.begin()); }
        void glTexCoord(const Point<GLint, 1> & t) { glTexCoord1iv(t.begin()); }
        void glTexCoord(const Point<GLint, 2> & t) { glTexCoord2iv(t.begin()); }
        void glTexCoord(const Point<GLint, 3> & t) { glTexCoord3iv(t.begin()); }
        void glTexCoord(const Point<GLint, 4> & t) { glTexCoord4iv(t.begin()); }
        void glTexCoord(const Point<GLfloat, 1> & t) { glTexCoord1fv(t.begin()); }
        void glTexCoord(const Point<GLfloat, 2> & t) { glTexCoord2fv(t.begin()); }
        void glTexCoord(const Point<GLfloat, 3> & t) { glTexCoord3fv(t.begin()); }
        void glTexCoord(const Point<GLfloat, 4> & t) { glTexCoord4fv(t.begin()); }
        void glTexCoord(const Point<GLdouble, 1> & t) { glTexCoord1dv(t.begin()); }
        void glTexCoord(const Point<GLdouble, 2> & t) { glTexCoord2dv(t.begin()); }
        void glTexCoord(const Point<GLdouble, 3> & t) { glTexCoord3dv(t.begin()); }
        void glTexCoord(const Point<GLdouble, 4> & t) { glTexCoord4dv(t.begin()); }
    #endif

    // glTranslate* overloads
    void glTranslate(GLfloat x, GLfloat y, GLfloat z)    { glTranslatef(x, y, z); }
    void glTranslate(GLdouble x, GLdouble y, GLdouble z) { glTranslated(x, y, z); }

    // glRotate* overloads
    void glRotate(GLfloat a,  GLfloat x,  GLfloat y,  GLfloat z)  { glRotatef(a, x, y, z); }
    void glRotate(GLdouble a, GLdouble x, GLdouble y, GLdouble z) { glRotated(a, x, y, z); }

    // glScale* overloads
    void glScale(GLfloat s)  { glScalef(s, s, s); }
    void glScale(GLdouble s) { glScaled(s, s, s); }
    void glScale(GLfloat x, GLfloat y, GLfloat z)    { glScalef(x, y, z); }
    void glScale(GLdouble x, GLdouble y, GLdouble z) { glScaled(x, y, z); }

    // glLoadMatrix* overloads
    void glLoadMatrix(GLfloat const * m)  { glLoadMatrixf(m); }
    void glLoadMatrix(GLdouble const * m) { glLoadMatrixd(m); }

    // glMultMatrix* overloads
    void glMultMatrix(GLfloat const * m)  { glMultMatrixf(m); }
    void glMultMatrix(GLdouble const * m) { glMultMatrixd(m); }

    // glClearColor overloads
    #ifdef GL_HPP_IMPORT_INCA
        void glClearColor(const Color<float, sRGB<false> > & c)  { glClearColor(c[0], c[1], c[2], 1.0f); }
        void glClearColor(const Color<float, sRGB<true> > & c)   { glClearColor(c[0], c[1], c[2], c[3]); }
        void glClearColor(const Color<double, sRGB<false> > & c) { glClearColor(float(c[0]), float(c[1]), float(c[2]), 1.0f); }
        void glClearColor(const Color<double, sRGB<true> > & c)  { glClearColor(float(c[0]), float(c[1]), float(c[2]), float(c[3])); }
    #endif

    // glGet*v overloads
    #ifdef GL_HPP_IMPORT_INCA
        template <int size> void glGetArray(GLenum property, Array<int, size> & array)      { glGetIntegerv(property, array.begin()); }
        template <int size> void glGetArray(GLenum property, Array<float, size> & array)    { glGetFloatv(property, array.begin()); }
        template <int size> void glGetArray(GLenum property, Array<double, size> & array)   { glGetDoublev(property, array.begin()); }
    #endif
};

#endif
