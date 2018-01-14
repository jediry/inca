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

#pragma once
#ifndef GL_HPP
#define GL_HPP

// If requested, import Inca types
#ifdef GL_HPP_IMPORT_INCA
#   include <inca/util/Array>
#   include <inca/math/linalg.hpp>
#   include <inca/math/color.hpp>
#   define IM ::inca::math
#endif

// Import OS-specific headers
#if __MS_WINDOZE__
    // Windows OpenGL seems to need this
#   include <windows.h>

    // I'd also rather VS didn't complain about casting to boolean
//#   pragma warning (disable : 4800)
#endif

// We wrap everything into the GL namespace, just for the fun of it
namespace GL {
    // Import OpenGL and GLU
    #include <GL/gl.h>
    #include <GL/glu.h>

    // Managed (/clr) code from Visual C++ throws a System.TypeLoadException
    // due to the class definitions not being on glu.h. We fix this by putting
    // in dummy class definitions, to force it to generate metadata.
    // I don't understand why this doesn't generate conflicts.
    #if __MS_WINDOZE__
        class GLUnurbs { };
        class GLUquadric { };
        class GLUtesselator { };
    #endif

    // If requested, import GLUT and GLUI
    #ifdef GL_HPP_IMPORT_GLUT
    #   include <GL/glut.h>
    #endif
    #ifdef GL_HPP_IMPORT_GLUI
    #   include <GL/glui.h>
    #endif


    // glVertex* overloads
    inline void glVertex(GLshort x, GLshort y)                           { glVertex2s(x, y); }
    inline void glVertex(GLshort x, GLshort y, GLshort z)                { glVertex3s(x, y, z); }
    inline void glVertex(GLshort x, GLshort y, GLshort z, GLshort w)     { glVertex4s(x, y, z, w); }
    inline void glVertex(GLint x, GLint y)                               { glVertex2i(x, y); }
    inline void glVertex(GLint x, GLint y, GLint z)                      { glVertex3i(x, y, z); }
    inline void glVertex(GLint x, GLint y, GLint z, GLint w)             { glVertex4i(x, y, z, w); }
    inline void glVertex(GLfloat x, GLfloat y)                           { glVertex2f(x, y); }
    inline void glVertex(GLfloat x, GLfloat y, GLfloat z)                { glVertex3f(x, y, z); }
    inline void glVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat w)     { glVertex4f(x, y, z, w); }
    inline void glVertex(GLdouble x, GLdouble y)                         { glVertex2d(x, y); }
    inline void glVertex(GLdouble x, GLdouble y, GLdouble z)             { glVertex3d(x, y, z); }
    inline void glVertex(GLdouble x, GLdouble y, GLdouble z, GLdouble w) { glVertex4d(x, y, z, w); }
    #ifdef GL_HPP_IMPORT_INCA
        template <inca::SizeType size>
        void glVertex(const IM::Point<GLshort, size> & v) {
            switch (size) {
                case 2: glVertex2sv(v.begin()); break;
                case 3: glVertex3sv(v.begin()); break;
                case 4: glVertex4sv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const IM::Point<GLint, size> & v) {
            switch (size) {
                case 2: glVertex2iv(v.begin()); break;
                case 3: glVertex3iv(v.begin()); break;
                case 4: glVertex4iv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const IM::Point<GLfloat, size> & v) {
            switch (size) {
                case 2: glVertex2fv(v.begin()); break;
                case 3: glVertex3fv(v.begin()); break;
                case 4: glVertex4fv(v.begin()); break;
            }
        }
        template <inca::SizeType size>
        void glVertex(const IM::Point<GLdouble, size> & v) {
            switch (size) {
                case 2: glVertex2dv(v.begin()); break;
                case 3: glVertex3dv(v.begin()); break;
                case 4: glVertex4dv(v.begin()); break;
            }
        }
    #endif

    // glNormal* overloads
    inline void glNormal(GLbyte x, GLbyte y, GLbyte z)       { glNormal3b(x, y, z); }
    inline void glNormal(GLshort x, GLshort y, GLshort z)    { glNormal3s(x, y, z); }
    inline void glNormal(GLint x, GLint y, GLint z)          { glNormal3i(x, y, z); }
    inline void glNormal(GLfloat x, GLfloat y, GLfloat z)    { glNormal3f(x, y, z); }
    inline void glNormal(GLdouble x, GLdouble y, GLdouble z) { glNormal3d(x, y, z); }
    #ifdef GL_HPP_IMPORT_INCA
        inline void glNormal(const IM::Vector<GLbyte, 3> & n)   { glNormal3bv(n.begin()); }
        inline void glNormal(const IM::Vector<GLshort, 3> & n)  { glNormal3sv(n.begin()); }
        inline void glNormal(const IM::Vector<GLint, 3> & n)    { glNormal3iv(n.begin()); }
        inline void glNormal(const IM::Vector<GLfloat, 3> & n)  { glNormal3fv(n.begin()); }
        inline void glNormal(const IM::Vector<GLdouble, 3> & n) { glNormal3dv(n.begin()); }
    #endif

    // glColor* overloads
    inline void glColor(GLbyte r, GLbyte g, GLbyte b)                   { glColor3b(r, g, b); }
    inline void glColor(GLbyte r, GLbyte g, GLbyte b, GLbyte a)         { glColor4b(r, g, b, a); }
    inline void glColor(GLubyte r, GLubyte g, GLubyte b)                { glColor3ub(r, g, b); }
    inline void glColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)     { glColor4ub(r, g, b, a); }
    inline void glColor(GLshort r, GLshort g, GLshort b)                { glColor3s(r, g, b); }
    inline void glColor(GLshort r, GLshort g, GLshort b, GLshort a)     { glColor4s(r, g, b, a); }
    inline void glColor(GLushort r, GLushort g, GLushort b)             { glColor3us(r, g, b); }
    inline void glColor(GLushort r, GLushort g, GLushort b, GLushort a) { glColor4us(r, g, b, a); }
    inline void glColor(GLint r, GLint g, GLint b)                      { glColor3i(r, g, b); }
    inline void glColor(GLint r, GLint g, GLint b, GLint a)             { glColor4i(r, g, b, a); }
    inline void glColor(GLuint r, GLuint g, GLuint b)                   { glColor3ui(r, g, b); }
    inline void glColor(GLuint r, GLuint g, GLuint b, GLuint a)         { glColor4ui(r, g, b, a); }
    inline void glColor(GLfloat r, GLfloat g, GLfloat b)                { glColor3f(r, g, b); }
    inline void glColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)     { glColor4f(r, g, b, a); }
    inline void glColor(GLdouble r, GLdouble g, GLdouble b)             { glColor3d(r, g, b); }
    inline void glColor(GLdouble r, GLdouble g, GLdouble b, GLdouble a) { glColor4d(r, g, b, a); }
    #ifdef GL_HPP_IMPORT_INCA
        inline void glColor(const IM::Color<GLbyte, IM::sRGB<false> > &c)   { glColor3bv(c.begin()); }
        inline void glColor(const IM::Color<GLbyte, IM::sRGB<true> > &c)    { glColor4bv(c.begin()); }
        inline void glColor(const IM::Color<GLubyte, IM::sRGB<false> > &c)  { glColor3ubv(c.begin()); }
        inline void glColor(const IM::Color<GLubyte, IM::sRGB<true> > &c)   { glColor4ubv(c.begin()); }
        inline void glColor(const IM::Color<GLshort, IM::sRGB<false> > &c)  { glColor3sv(c.begin()); }
        inline void glColor(const IM::Color<GLshort, IM::sRGB<true> > &c)   { glColor4sv(c.begin()); }
        inline void glColor(const IM::Color<GLushort, IM::sRGB<false> > &c) { glColor3usv(c.begin()); }
        inline void glColor(const IM::Color<GLushort, IM::sRGB<true> > &c)  { glColor4usv(c.begin()); }
        inline void glColor(const IM::Color<GLint, IM::sRGB<false> > &c)    { glColor3iv(c.begin()); }
        inline void glColor(const IM::Color<GLint, IM::sRGB<true> > &c)     { glColor4iv(c.begin()); }
        inline void glColor(const IM::Color<GLuint, IM::sRGB<false> > &c)   { glColor3uiv(c.begin()); }
        inline void glColor(const IM::Color<GLuint, IM::sRGB<true> > &c)    { glColor4uiv(c.begin()); }
        inline void glColor(const IM::Color<GLfloat, IM::sRGB<false> > &c)  { glColor3fv(c.begin()); }
        inline void glColor(const IM::Color<GLfloat, IM::sRGB<true> > &c)   { glColor4fv(c.begin()); }
        inline void glColor(const IM::Color<GLdouble, IM::sRGB<false> > &c) { glColor3dv(c.begin()); }
        inline void glColor(const IM::Color<GLdouble, IM::sRGB<true> > &c)  { glColor4dv(c.begin()); }
    #endif

    // glIndex* overloads

    // glTexCoord* overloads
    inline void glTexCoord(GLshort u)                                  { glTexCoord1s(u); }
    inline void glTexCoord(GLshort u, GLshort v)                       { glTexCoord2s(u, v); }
    inline void glTexCoord(GLshort u, GLshort v, GLshort w)            { glTexCoord3s(u, v, w); }
    inline void glTexCoord(GLshort u, GLshort v, GLshort w, GLshort q) { glTexCoord4s(u, v, w, q); }
    inline void glTexCoord(GLint u)                            { glTexCoord1i(u); }
    inline void glTexCoord(GLint u, GLint v)                   { glTexCoord2i(u, v); }
    inline void glTexCoord(GLint u, GLint v, GLint w)          { glTexCoord3i(u, v, w); }
    inline void glTexCoord(GLint u, GLint v, GLint w, GLint q) { glTexCoord4i(u, v, w, q); }
    inline void glTexCoord(GLfloat u)                                  { glTexCoord1f(u); }
    inline void glTexCoord(GLfloat u, GLfloat v)                       { glTexCoord2f(u, v); }
    inline void glTexCoord(GLfloat u, GLfloat v, GLfloat w)            { glTexCoord3f(u, v, w); }
    inline void glTexCoord(GLfloat u, GLfloat v, GLfloat w, GLfloat q) { glTexCoord4f(u, v, w, q); }
    inline void glTexCoord(GLdouble u)                                     { glTexCoord1d(u); }
    inline void glTexCoord(GLdouble u, GLdouble v)                         { glTexCoord2d(u, v); }
    inline void glTexCoord(GLdouble u, GLdouble v, GLdouble w)             { glTexCoord3d(u, v, w); }
    inline void glTexCoord(GLdouble u, GLdouble v, GLdouble w, GLdouble q) { glTexCoord4d(u, v, w, q); }
    #ifdef GL_HPP_IMPORT_INCA
        inline void glTexCoord(const IM::Point<GLshort, 1> & t) { glTexCoord1sv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLshort, 2> & t) { glTexCoord2sv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLshort, 3> & t) { glTexCoord3sv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLshort, 4> & t) { glTexCoord4sv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLint, 1> & t) { glTexCoord1iv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLint, 2> & t) { glTexCoord2iv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLint, 3> & t) { glTexCoord3iv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLint, 4> & t) { glTexCoord4iv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLfloat, 1> & t) { glTexCoord1fv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLfloat, 2> & t) { glTexCoord2fv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLfloat, 3> & t) { glTexCoord3fv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLfloat, 4> & t) { glTexCoord4fv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLdouble, 1> & t) { glTexCoord1dv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLdouble, 2> & t) { glTexCoord2dv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLdouble, 3> & t) { glTexCoord3dv(t.begin()); }
        inline void glTexCoord(const IM::Point<GLdouble, 4> & t) { glTexCoord4dv(t.begin()); }
    #endif

    // glTranslate* overloads
    inline void glTranslate(GLfloat x, GLfloat y, GLfloat z)    { glTranslatef(x, y, z); }
    inline void glTranslate(GLdouble x, GLdouble y, GLdouble z) { glTranslated(x, y, z); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar>
        inline void glTranslate(const IM::Point<Scalar, 3> & dx) { glTranslate(dx[0], dx[1], dx[2]); }
        template <typename Scalar>
        inline void glTranslate(const IM::Vector<Scalar, 3> & dx) { glTranslate(dx[0], dx[1], dx[2]); }
    #endif

    // glRotate* overloads
    inline void glRotate(GLfloat a,  GLfloat x,  GLfloat y,  GLfloat z)  { glRotatef(a, x, y, z); }
    inline void glRotate(GLdouble a, GLdouble x, GLdouble y, GLdouble z) { glRotated(a, x, y, z); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar>
        inline void glRotate(Scalar an, const IM::Vector<Scalar, 3> & ax) { glRotate(an, ax[0], ax[1], ax[2]); }
    #endif

    // glScale* overloads
    inline void glScale(GLfloat s)  { glScalef(s, s, s); }
    inline void glScale(GLdouble s) { glScaled(s, s, s); }
    inline void glScale(GLfloat x, GLfloat y, GLfloat z)    { glScalef(x, y, z); }
    inline void glScale(GLdouble x, GLdouble y, GLdouble z) { glScaled(x, y, z); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar>
        inline void glScale(const IM::Vector<Scalar, 3> & s) { glScale(s[0], s[1], s[2]); }
    #endif

    // glLoadMatrix* overloads
    inline void glLoadMatrix(GLfloat const * m)  { glLoadMatrixf(m); }
    inline void glLoadMatrix(GLdouble const * m) { glLoadMatrixd(m); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar, bool rowMajAccess>
        inline void glLoadMatrix(const IM::Matrix<Scalar, 4, 4, rowMajAccess, false> & m) { glLoadMatrix(m.begin()); }
    #endif

    // glMultMatrix* overloads
    inline void glMultMatrix(GLfloat const * m)  { glMultMatrixf(m); }
    inline void glMultMatrix(GLdouble const * m) { glMultMatrixd(m); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar, bool rowMajAccess>
        inline void glMultMatrix(const IM::Matrix<Scalar, 4, 4, rowMajAccess, false> & m) { glMultMatrix(m.begin()); }
    #endif

    // glClearColor overloads
    #ifdef GL_HPP_IMPORT_INCA
        inline void glClearColor(const IM::Color<float, IM::sRGB<false> > & c)  { glClearColor(c[0], c[1], c[2], 1.0f); }
        inline void glClearColor(const IM::Color<float, IM::sRGB<true> > & c)   { glClearColor(c[0], c[1], c[2], c[3]); }
        inline void glClearColor(const IM::Color<double, IM::sRGB<false> > & c) { glClearColor(float(c[0]), float(c[1]), float(c[2]), 1.0f); }
        inline void glClearColor(const IM::Color<double, IM::sRGB<true> > & c)  { glClearColor(float(c[0]), float(c[1]), float(c[2]), float(c[3])); }
    #endif

    // glGet*v overloads
    #ifdef GL_HPP_IMPORT_INCA
        template <int size> void glGetArray(GLenum property, ::inca::Array<int, size> & array)      { glGetIntegerv(property, array.begin()); }
        template <int size> void glGetArray(GLenum property, ::inca::Array<float, size> & array)    { glGetFloatv(property, array.begin()); }
        template <int size> void glGetArray(GLenum property, ::inca::Array<double, size> & array)   { glGetDoublev(property, array.begin()); }
    #endif

    // glLight* overloads
    inline void glLight(GLenum light, GLenum param, GLint value)   { glLighti(light, param, value); }
    inline void glLight(GLenum light, GLenum param, GLfloat value) { glLightf(light, param, value); }
    inline void glLight(GLenum light, GLenum param, GLint const * values)   { glLightiv(light, param, values); }
    inline void glLight(GLenum light, GLenum param, GLfloat const * values) { glLightfv(light, param, values); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar, inca::SizeType dim>
        inline void glLight(GLenum light, GLenum param, const IM::Point<Scalar, dim> & p) { glLight(light, param, p.begin()); }
        template <typename Scalar, inca::SizeType dim>
        inline void glLight(GLenum light, GLenum param, const IM::Vector<Scalar, dim> & v) { glLight(light, param, v.begin()); }
        template <typename Scalar>
        inline void glLight(GLenum light, GLenum param, const IM::Color<Scalar, IM::sRGB<true> > & c) { glLight(light, param, c.begin()); }
    #endif

    // glFog* overloads
    inline void glFog(GLenum param, GLint value)   { glFogi(param, value); }
    inline void glFog(GLenum param, GLfloat value) { glFogf(param, value); }
    inline void glFog(GLenum param, GLint const * values)   { glFogiv(param, values); }
    inline void glFog(GLenum param, GLfloat const * values) { glFogfv(param, values); }
    #ifdef GL_HPP_IMPORT_INCA
        template <typename Scalar>
        inline void glFog(GLenum param, const IM::Color<Scalar, IM::sRGB<true> > & c) { glFog(param, c.begin()); }
    #endif
};

// Clean up the preprocessor namespace
#ifdef IM
#   undef IM
#endif

#endif
