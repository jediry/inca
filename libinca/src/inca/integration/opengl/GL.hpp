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


namespace GL {
    // Import OpenGL
    #if __MS_WINDOZE__
        // Windows OpenGL seems to need this
    #   include <windows.h>

        // I'd also rather VS didn't complain about casting to boolean
    //#   pragma warning (disable : 4800)
    #endif
    #include <GL/gl.h>
    #include <GL/glu.h>

    #ifdef GL_HPP_IMPORT_GLUT
    #   include <GL/glut.h>
    #endif

    #ifdef GL_HPP_IMPORT_GLUI
    #   include <GL/glui.h>
    #endif


    // glVertex* overloads
    void glVertex(GLint x, GLint y)                               { glVertex2i(x, y); }
    void glVertex(GLint x, GLint y, GLint z)                      { glVertex3i(x, y, z); }
    void glVertex(GLint x, GLint y, GLint z, GLint w)             { glVertex4i(x, y, z, w); }
    void glVertex(GLfloat x, GLfloat y)                           { glVertex2f(x, y); }
    void glVertex(GLfloat x, GLfloat y, GLfloat z)                { glVertex3f(x, y, z); }
    void glVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat w)     { glVertex4f(x, y, z, w); }
    void glVertex(GLdouble x, GLdouble y)                         { glVertex2d(x, y); }
    void glVertex(GLdouble x, GLdouble y, GLdouble z)             { glVertex3d(x, y, z); }
    void glVertex(GLdouble x, GLdouble y, GLdouble z, GLdouble w) { glVertex4d(x, y, z, w); }

    // glNormal* overloads

    // glColor* overloads
    void glColor(GLfloat r, GLfloat g, GLfloat b)                { glColor3f(r, g, b); }
    void glColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)     { glColor4f(r, g, b, a); }
    void glColor(GLdouble r, GLdouble g, GLdouble b)             { glColor3d(r, g, b); }
    void glColor(GLdouble r, GLdouble g, GLdouble b, GLdouble a) { glColor4d(r, g, b, a); }

    // glIndex* overloads

    // glTexCoord* overloads


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
};

#endif