/*
 * File: opengl-ImmediateModeRenderer.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the ImmediateModeRenderer functions in terms of
 *      the OpenGL API.
 */

// Import type definitions
#include <inca/rendering/immediate-mode/OpenGLTraits.hpp>
#include <inca/rendering/immediate-mode/types.hpp>
using namespace inca;
using namespace inca::rendering;

// Import C++-ified OpenGL
#define GL_HPP_IMPORT_INCA
#include "GL.hpp"
using namespace GL;


// Simplify naming of the things we'll be working with
#define API OpenGLTraits
//using API::Point;
//using API::Vector;
//using API::Matrix;
//using API::Pixel;
//using API::Dimension;
typedef API::Color RGB;
using API::Normal;
using API::TexCoord;



// Macros to make long names shorter
//#define RENDERER        ImmediateModeRenderer<OpenGLTraits, ExclusiveAccessCachePolicy>
//#define RASTERIZER      RENDERER::Rasterizer
//#define VIEWPORT        RENDERER::Viewport
//#define MATRIX_STACK    RENDERER::MatrixStack
//#define LIGHTING_UNIT   RENDERER::LightingUnit
//#define TEXTURE_UNIT    RENDERER::TextureUnit
//#define COLOR_BUFFER    RENDERER::Buffer<RENDERER::Color>
//#define DEPTH_BUFFER    RENDERER::Buffer<RENDERER::geometry_t>


/*---------------------------------------------------------------------------*
 | OpenGL-specific non-member functions
 *---------------------------------------------------------------------------*/
/*int OpenGL_currentMatrix(const RENDERER &r) {
    if (! intPropertyCache[MATRIX_MODE].valid) {
        glGetFloatv(GL_MATRIX_MODE, &intPropertyCache[MATRIX_MODE].value);
        intPropertyCache[MATRIX_MODE].valid = true;
    }
    return intPropertyCache[MATRIX_MODE].value;
}
void OpenGL_ensureMatrixIsCurrent(const RENDERER &r, IDType id) {
    // Only if the current matrix != this one do we change
    if (OpenGL_currentMatrix(r) != id)
        glMatrixMode(id);
}*/

#define GL_GET_BOOLEAN_VALUE(PROPERTY, VALUE) {                     \
    GLboolean result;                                               \
    glGetBooleanv(PROPERTY, &result);                               \
    VALUE = (result == GL_TRUE);                                    \
}
#define GL_GET_INTEGER_VALUE(PROPERTY, VALUE) {                     \
    glGetIntegerv(PROPERTY, &VALUE);                                \
}
#define GL_GET_ARRAY_VALUE(PROPERTY, VALUE) {                       \
    glGetArray(PROPERTY, VALUE);                                    \
}
#define GL_ENABLE(PROPERTY, ENABLED) {                              \
    if (ENABLED)    glEnable(PROPERTY);                             \
    else            glDisable(PROPERTY);                            \
}


/*---------------------------------------------------------------------------*
 | Rendering state control functions
 *---------------------------------------------------------------------------*/
// Depth buffering
template <>
void API::getHardwareState<API::DepthBuffering, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_DEPTH_TEST, enabled);
}
template <>
void API::setHardwareState<API::DepthBuffering, bool>(const bool & enabled) {
    GL_ENABLE(GL_DEPTH_TEST, enabled);
}

// Alpha blending
template <>
void API::getHardwareState<API::AlphaBlending, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_BLEND, enabled);
}
template <>
void API::setHardwareState<API::AlphaBlending, bool>(const bool & enabled) {
    GL_ENABLE(GL_BLEND, enabled);
}

// Polygon face culling
template <>
void API::getHardwareState<API::FaceCulling, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_CULL_FACE, enabled);
}
template <>
void API::setHardwareState<API::FaceCulling, bool>(const bool & enabled) {
    GL_ENABLE(GL_CULL_FACE, enabled);
}

// Hardware lighting
template <>
void API::getHardwareState<API::Lighting, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_LIGHTING, enabled);
}
template <>
void API::setHardwareState<API::Lighting, bool>(const bool & enabled) {
    GL_ENABLE(GL_LIGHTING, enabled);
}

// Point antialiasing
template <>
void API::getHardwareState<API::PointSmoothing, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_POINT_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::PointSmoothing, bool>(const bool & enabled) {
    GL_ENABLE(GL_POINT_SMOOTH, enabled);
}

// Line antialiasing
template <>
void API::getHardwareState<API::LineSmoothing, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_LINE_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::LineSmoothing, bool>(const bool & enabled) {
    GL_ENABLE(GL_LINE_SMOOTH, enabled);
}

// Polygon antialiasing
template <>
void API::getHardwareState<API::PolygonSmoothing, bool>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_POLYGON_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::PolygonSmoothing, bool>(const bool & enabled) {
    GL_ENABLE(GL_POLYGON_SMOOTH, enabled);
}

// Shading mode
template <>
void API::getHardwareState<API::CurrentShadingModel, ShadingModel>(ShadingModel & model) {
    GLint result;
    GL_GET_INTEGER_VALUE(GL_SHADE_MODEL, result);
    switch (result) {
        case GL_SMOOTH: model = SmoothShade; break;
        case GL_FLAT:   model = FlatShade;   break;
        default:        model = Paint;       break;
    }
}
template <>
void API::setHardwareState<API::CurrentShadingModel, ShadingModel>(const ShadingModel & model) {
    switch (model) {
        case SmoothShade: glShadeModel(GL_SMOOTH); break;
        case FlatShade:   glShadeModel(GL_FLAT);   break;
        default:          glShadeModel(GL_SMOOTH); break;
    }
}

// Background color
template <>
void API::getHardwareState<API::BackgroundColor, RGB>(RGB & color) {
    GL_GET_ARRAY_VALUE(GL_COLOR_CLEAR_VALUE, color);
}
template <>
void API::setHardwareState<API::BackgroundColor, RGB>(const RGB & color) {
    glClearColor(color);
}

// Drawing color
template <>
void API::getHardwareState<API::CurrentColor, RGB>(RGB & color) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_COLOR, color);
}
template <>
void API::setHardwareState<API::CurrentColor, RGB>(const RGB & color) {
    glColor(color);
}

// Drawing normal
template <>
void API::getHardwareState<API::CurrentNormal, Normal>(Normal & normal) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_NORMAL, normal);
}
template <>
void API::setHardwareState<API::CurrentNormal, Normal>(const Normal & normal) {
    glNormal(normal);
}

// Current texture coordinates
template <>
void API::getHardwareState<API::CurrentTexCoord, TexCoord>(TexCoord & texCoord) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_TEXTURE_COORDS, texCoord);
}
template <>
void API::setHardwareState<API::CurrentTexCoord, TexCoord>(const TexCoord & texCoord) {
    glTexCoord(texCoord);
}

// Current edge flag
template <>
void API::getHardwareState<API::CurrentEdgeFlag, bool>(bool & edge) {
    GL_GET_BOOLEAN_VALUE(GL_EDGE_FLAG, edge);
}
template <>
void API::setHardwareState<API::CurrentEdgeFlag, bool>(const bool & edge) {
    glEdgeFlag(edge);
}

#if 0
/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer::Rasterizer functions
 *---------------------------------------------------------------------------*/

RASTERIZER::Rasterizer(RENDERER &r, IDType myID) : Component(r, myID) {

}



/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer::Viewport functions
 *---------------------------------------------------------------------------*/
template <>
MATRIX_STACK::Viewport(RENDERER &r, IDType myID) : Component(r, myID) {

}


/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer::MatrixStack functions
 *---------------------------------------------------------------------------*/
// Constructor
template <>
MATRIX_STACK::MatrixStack(RENDERER &r, IDType myID)
        : Component(r, myID), depth(1), currentValid(false) {
    // Query the implementation's depth limitation
    switch (id) {
        case GL_MODELVIEW:
            glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &maxDepth);
            break;
        case GL_PROJECTION:
            glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &maxDepth);
            break;
//        case GL_COLOR:
//            glGetIntegerv(GL_COLOR_STACK_DEPTH, &maxDepth);
//            break;
        case GL_TEXTURE:
            glGetIntegerv(GL_TEXTURE_STACK_DEPTH, &maxDepth);
            break;
    }
}

// Retrieve current stack depth
template <>
int MATRIX_STACK::stackDepth() const {
    return depth;
}

// Retrieve stack depth limit
template <>
int MATRIX_STACK::maximumStackDepth() const {
    return maxDepth;
}

// Is the stack full?
template <>
bool MATRIX_STACK::isFull() const {
    return depth == maxDepth;
}

// Is the stack empty?
template <>
bool MATRIX_STACK::isEmpty() const {
    return depth == 1;
}

// Push a matrix onto the stack
template <>
void MATRIX_STACK::push() const {
    if (isFull()) {
        // Whoops! Can't do that!
        cerr << "MatrixStack::push(): Exceeded maximum depth of "
             << maxDepth << endl;
    } else {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        glPushMatrix();
    }
}

// Pop a matrix off the stack
template <>
void MATRIX_STACK::pop() const {
    if (isEmpty()) {
        // Whoops! Can't do that!
        cerr << "MatrixStack::pop(): Can't pop an empty stack" << endl;
    } else {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        glPopMatrix();
        currentValid = inverseValid = false;
    }
}


// Reset to the identity matrix
template <>
void MATRIX_STACK::reset() const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glLoadIdentity();
    currentValid = inverseValid = false;
}

// Load a matrix onto the stack
template <>
void MATRIX_STACK::load(const Matrix &m) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glLoadMatrix(m.begin()); // Copy the contents of 'm' onto the stack
    currentValid = inverseValid = false;
}

// Retrieve the current contents of the stack
template <>
const RENDERER::Matrix & MATRIX_STACK::get() const {
    if (! currentValid) {   // Aww, crud. Gotta get it from OpenGL
        switch (id) {
            case GL_MODELVIEW:
                glGetFloatv(GL_MODELVIEW_MATRIX, current.begin());
                break;
            case GL_PROJECTION:
                glGetFloatv(GL_PROJECTION_MATRIX, current.begin());
                break;
//            case GL_COLOR:
//                glGetFloatv(GL_COLOR_MATRIX, current.begin());
//                break;
            case GL_TEXTURE:
                glGetFloatv(GL_TEXTURE_MATRIX, current.begin());
                break;
        }
        currentValid = true;    // OK. NOW it's valid.
    }
    return current;
}

// Retrieve the inverse of the current matrix
template <>
const RENDERER::Matrix & MATRIX_STACK::getInverse() const {
    if (!inverseValid) {    // Gotta calculate it!
        get();                  // Make sure the uninverted matrix is good
        inverse = inca::math::inverse(current);
        inverseValid = true;
    }
    return inverse;
}

// Pre-multiply the top of the stack by 'm' (i.e.: MS = m % MS)
template <>
void MATRIX_STACK::preMultiply(const Matrix &m) const {
    get();                      // Ensure that the cached matrix is current
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glLoadMatrix(m.begin()); // Load 'm' onto the stack
    glMultMatrix(current.begin());   // Multiply by prior 'top'
    currentValid = inverseValid = false;
}

// Post-multiply the top of the stack by 'm' (i.e.: MS = MS % m)
template <>
void MATRIX_STACK::postMultiply(const Matrix &m) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glMultMatrix(m.begin()); // Multiply by 'm'
    currentValid = inverseValid = false;
}


// Scale by a uniform scaling factor
template <>
void MATRIX_STACK::scale(geometry_t s) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glScale(s, s, s);           // Scale uniformly
    currentValid = inverseValid = false;
}

// Scale non-uniformly in the three directions
template <>
void MATRIX_STACK::scale(const Vector3D &s) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glScale(s[0], s[1], s[2]);  // Scale in three directions
    currentValid = inverseValid = false;
}

// Unscale by a uniform scaling factor
template <>
void MATRIX_STACK::unscale(geometry_t s) const {
    geometry_t s_1 = 1 / s;     // Calculate the inverse of 's'
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glScale(s_1, s_1, s_1);     // Scale uniformly
    currentValid = inverseValid = false;
}

// Unscale non-uniformly in the three directions
template <>
void MATRIX_STACK::unscale(const Vector3D &s) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glScale(1 / s[0], 1 / s[1], 1 / s[2]);  // Scale in three directions
    currentValid = inverseValid = false;
}

// Rotate by a quaternion
template <>
void MATRIX_STACK::rotate(const Quaternion &q) const {
    // If the first element of q is 1, skip (this is an identity rotation)
    if (! effectivelyEqual(q[0], geometry_t(1))) {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        geometry_t alpha = angle(q);    // Extract into something OpenGL
        Vector3D around = axis(q);      // can understand
        glRotate(alpha, around[0], around[1], around[2]);
        currentValid = inverseValid = false;
    }
}

// Rotate by an angle (in radians) around an axis
template <>
void MATRIX_STACK::rotate(geometry_t angle, const Vector3D &axis) const {
    // If the angle is zero, skip (this is an identity rotation)
    if (! effectivelyZero(angle)) {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        glRotate(radiansToDegrees(angle), axis[0], axis[1], axis[2]);
        currentValid = inverseValid = false;
    }
}

// Unrotate by a quaternion
template <>
void MATRIX_STACK::unrotate(const Quaternion &q) const {
    // If the first element of q is 1, skip (this is an identity rotation)
    if (! effectivelyEqual(q[0], geometry_t(1))) {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        geometry_t alpha = -angle(q);   // Note that we're getting the
        Vector3D around = axis(q);      // negation of the angle here
        glRotate(alpha, around[0], around[1], around[2]);
        currentValid = inverseValid = false;
    }
}

// Unrotate by an angle (in radians) around an axis
template <>
void MATRIX_STACK::unrotate(geometry_t angle, const Vector3D &axis) const {
    // If the angle is zero, skip (this is an identity rotation)
    if (! effectivelyZero(angle)) {
        OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
        glRotate(radiansToDegrees(-angle), axis[0], axis[1], axis[2]);
        currentValid = inverseValid = false;
    }
}

// Translate by a vector
template <>
void MATRIX_STACK::translate(const Vector3D &v) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glTranslate(v[0], v[1], v[2]);
    currentValid = inverseValid = false;
}

// Untranslate by a vector
template <>
void MATRIX_STACK::untranslate(const Vector3D &v) const {
    OpenGL_ensureMatrixIsCurrent(renderer, id);  // Make sure we operate on the right matrix
    glTranslate(-v[0], -v[1], -v[2]);
    currentValid = inverseValid = false;
}

// Transform a Point from world to local coordinates
template <>
RENDERER::Point3D MATRIX_STACK::transform(const Point3D &p) const {
    get();                      // Ensure that the cached matrix is current
    return current % p;         // Multiply through the matrix
}

// Transform a Vector from world to local coordinates
template <>
RENDERER::Vector3D MATRIX_STACK::transform(const Vector3D &v) const {
    get();                      // Ensure that the cached matrix is current
    return current % v;         // Multiply through the matrix
}

// Transform a Point from local to world coordinates
template <>
RENDERER::Point3D MATRIX_STACK::untransform(const Point3D &p) const {
    getInverse();               // Ensure that the cached inverse is current
    return inverse % p;         // Multiply through the matrix
}

// Transform a Vector from local to world coordinates
template <>
RENDERER::Vector3D MATRIX_STACK::untransform(const Vector3D &v) const {
    getInverse();               // Ensure that the cached inverse is current
    return inverse % v;         // Multiply through the matrix
}

// If this matrix stack is not selec
void MATRIX_STACK::ensureActive() const {
    // Figure out which matrix is selected
    if (! renderer.intPropertyCache[MATRIX_MODE].valid) {
        glGetIntegerv(GL_MATRIX_MODE, &renderer.intPropertyCache[MATRIX_MODE].value);
        renderer.intPropertyCache[MATRIX_MODE].valid = true;
    }
    // If it's not me, take some action
    if (renderer.intPropertyCache[MATRIX_MODE].value != id)
        glMatrixMode(id);
}


/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer initialization
 *---------------------------------------------------------------------------*/
// Default constructor
template <>
RENDERER::ImmediateModeRenderer() {
    // XXX Total HACK -- should get this from device/context
    int numMatrixStacks = 2;
    int numLightingUnits = 8;
    int numTextureUnits = 4;
    int numColorBuffers = 2;
    int numScalarBuffers = 1;
    int numStatistics = 2;

    // Create the matrix stacks (symbolically named)
    _matrixStacks.resize(numMatrixStacks);
    _matrixStacks[PROJECTION_MATRIX] = MatrixStack(*this, GL_PROJECTION);
    _matrixStacks[MODELVIEW_MATRIX]  = MatrixStack(*this, GL_MODELVIEW);

    // Create the lighting units (enumerated)
    for (int i = 0; i < numLightingUnits; ++i)
        _lightingUnits.push_back(LightingUnit(*this, GL_LIGHT0 + i));

    // Create the texture units (enumerated)
//    for (int i = 0; i < numTextureUnits; ++i)
//        _textureUnits.push_back(TextureUnit(*this,

    // Create the color buffers (symbolically named)
    _colorBuffers.resize(numColorBuffers);
    _colorBuffers[FRONT_BUFFER] = ColorBuffer(*this, GL_FRONT);
    _colorBuffers[BACK_BUFFER]  = ColorBuffer(*this, GL_BACK);

    // Create the scalar buffers (symbolically named)
    _scalarBuffers.resize(numScalarBuffers);
    _scalarBuffers[DEPTH_BUFFER] = ScalarBuffer(*this, GL_FRONT);
}


/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer rendering-state control functions
 *---------------------------------------------------------------------------*/
// Background color
template <>
RENDERER::Color RENDERER::backgroundColor() const {
    if (! colorPropertyCache[COLOR_CLEAR_VALUE].valid) {
        glGetFloatv(GL_COLOR_CLEAR_VALUE, colorPropertyCache[COLOR_CLEAR_VALUE].value.begin());
        colorPropertyCache[COLOR_CLEAR_VALUE].valid = true;
    }
    return colorPropertyCache[COLOR_CLEAR_VALUE].value;
}
template <>
void RENDERER::setBackgroundColor(const Color &c) {
    glClearColor(c[0], c[1], c[2], c[3]);
    colorPropertyCache[COLOR_CLEAR_VALUE].value = c;
    colorPropertyCache[COLOR_CLEAR_VALUE].valid = true;
}

// Fragment depth-test
template <>
bool RENDERER::isDepthBufferingEnabled() const {
    if (! boolPropertyCache[DEPTH_TEST].valid) {
        GLboolean b;
        glGetBooleanv(GL_DEPTH_TEST, &b);
        boolPropertyCache[DEPTH_TEST].value = (b != 0);
        boolPropertyCache[DEPTH_TEST].valid = true;
    }
    return boolPropertyCache[DEPTH_TEST].value;
}
template <>
void RENDERER::enableDepthBuffering(bool enabled) {
    glEnable(GL_DEPTH_TEST);
    boolPropertyCache[DEPTH_TEST].value = enabled;
    boolPropertyCache[DEPTH_TEST].valid = true;
}

template <>
GLfloat RENDERER::pointDiameter() const {
    if (! floatPropertyCache[POINT_SIZE].valid) {
        glGetFloatv(GL_POINT_SIZE, &floatPropertyCache[POINT_SIZE].value);
        floatPropertyCache[POINT_SIZE].valid = true;
    }
    return floatPropertyCache[POINT_SIZE].value;
}
template <>
void RENDERER::setPointDiameter(GLfloat size) {
    glPointSize(size);
    floatPropertyCache[POINT_SIZE].value = size;
    floatPropertyCache[POINT_SIZE].valid = true;
}

template <>
GLfloat RENDERER::lineWidth() const {
    if (! floatPropertyCache[LINE_WIDTH].valid) {
        glGetFloatv(GL_LINE_WIDTH, &floatPropertyCache[LINE_WIDTH].value);
        floatPropertyCache[LINE_WIDTH].valid = true;
    }
    return floatPropertyCache[LINE_WIDTH].value;
}
template <>
void RENDERER::setLineWidth(GLfloat width) {
    glLineWidth(width);
    floatPropertyCache[LINE_WIDTH].value = width;
    floatPropertyCache[LINE_WIDTH].valid = true;
}

template <>
bool RENDERER::isPointSmoothingEnabled() const {
    if (! boolPropertyCache[POINT_SMOOTH].valid) {
        GLboolean b;
        glGetBooleanv(GL_POINT_SMOOTH, &b);
        boolPropertyCache[POINT_SMOOTH].value = (b != 0);
        boolPropertyCache[POINT_SMOOTH].valid = true;
    }
    return boolPropertyCache[POINT_SMOOTH].value;
}
template <>
void RENDERER::enablePointSmoothing(bool enabled) {
    glEnable(GL_POINT_SMOOTH);
    boolPropertyCache[POINT_SMOOTH].value = enabled;
    boolPropertyCache[POINT_SMOOTH].valid = true;
}

template <>
bool RENDERER::isLineSmoothingEnabled() const {
    if (! boolPropertyCache[LINE_SMOOTH].valid) {
        GLboolean b;
        glGetBooleanv(GL_LINE_SMOOTH, &b);
        boolPropertyCache[LINE_SMOOTH].value = (b != 0);
        boolPropertyCache[LINE_SMOOTH].valid = true;
    }
    return boolPropertyCache[LINE_SMOOTH].value;
}
template <>
void RENDERER::enableLineSmoothing(bool enabled) {
    glEnable(GL_LINE_SMOOTH);
    boolPropertyCache[LINE_SMOOTH].value = enabled;
    boolPropertyCache[LINE_SMOOTH].valid = true;
}

template <>
bool RENDERER::isBlendingEnabled() const {
    if (! boolPropertyCache[BLEND].valid) {
        GLboolean b;
        glGetBooleanv(GL_BLEND, &b);
        boolPropertyCache[BLEND].value = (b != 0);
        boolPropertyCache[BLEND].valid = true;
    }
    return boolPropertyCache[BLEND].value;
}
template <>
void RENDERER::enableBlending(bool enabled) {
    glEnable(GL_BLEND);
    boolPropertyCache[BLEND].value = enabled;
    boolPropertyCache[BLEND].valid = true;
}

template <>
bool RENDERER::isLightingEnabled() const {
    if (! boolPropertyCache[LIGHTING].valid) {
        GLboolean b;
        glGetBooleanv(GL_LIGHTING, &b);
        boolPropertyCache[LIGHTING].value = (b != 0);
        boolPropertyCache[LIGHTING].valid = true;
    }
    return boolPropertyCache[LIGHTING].value;
}
template <>
void RENDERER::enabledLighting(bool enabled) {
    glEnable(GL_LIGHTING);
    boolPropertyCache[LIGHTING].value = enabled;
    boolPropertyCache[LIGHTING].valid = true;
}

template <>
void RENDERER::setShadingMode(ShadingMode mode) {
    cerr << "Set shading more not impl.\n";
}


/*---------------------------------------------------------------------------*
 | ImmediateModeRenderer primitive rendering functions
 *---------------------------------------------------------------------------*/
// Start/end rendering a geometric primitive
template <>
void RENDERER::beginPrimitive(PrimitiveType type) {
    switch (type) {
        case Points:        glBegin(GL_POINTS);         break;
        case Lines:         glBegin(GL_LINES);          break;
        case LineStrip:     glBegin(GL_LINE_STRIP);     break;
        case LineLoop:      glBegin(GL_LINE_LOOP);      break;
        case Triangles:     glBegin(GL_TRIANGLES);      break;
        case TriangleStrip: glBegin(GL_TRIANGLE_STRIP); break;
        case TriangleFan:   glBegin(GL_TRIANGLE_FAN);   break;
        case Quads:         glBegin(GL_QUADS);          break;
        case QuadStrip:     glBegin(GL_QUAD_STRIP);     break;
        case Polygon:       glBegin(GL_POLYGON);        break;
        default:
            cerr << "OpenGLRenderer::beginPrimitive(" << type << "): "
                    "unsupported PrimitiveType\n";
    }
    glBegin(type);
}
template <>
void RENDERER::endPrimitive() {
    glEnd();
}


// Current drawing color
template<> template <>      // Specialization for GLbyte w/o alpha
void RENDERER::setColor<GLbyte, sRGB>(const math::Color<GLbyte, sRGB> &c) {
    glColor3bv(c.begin());
}
template <> template <>     // Specialization for GLbyte w/ alpha
void RENDERER::setColor<GLbyte, sRGBA>(const math::Color<GLbyte, sRGBA> &c) {
    glColor4bv(c.begin());
}
template <> template <>     // Specialization for GLubyte w/o alpha
void RENDERER::setColor<GLubyte, sRGB>(const math::Color<GLubyte, sRGB> &c) {
    glColor3ubv(c.begin());
}
template <> template <>     // Specialization for GLubyte w/ alpha
void RENDERER::setColor<GLubyte, sRGBA>(const math::Color<GLubyte, sRGBA> &c) {
    glColor4ubv(c.begin());
}
template <> template <>     // Specialization for GLshort w/o alpha
void RENDERER::setColor<GLshort, sRGB>(const math::Color<GLshort, sRGB> &c) {
    glColor3sv(c.begin());
}
template <> template <>     // Specialization for GLshort w/ alpha
void RENDERER::setColor<GLshort, sRGBA>(const math::Color<GLshort, sRGBA> &c) {
    glColor4sv(c.begin());
}
template <> template <>     // Specialization for GLushort w/o alpha
void RENDERER::setColor<GLushort, sRGB>(const math::Color<GLushort, sRGB> &c) {
    glColor3usv(c.begin());
}
template <> template <>     // Specialization for GLushort w/ alpha
void RENDERER::setColor<GLushort, sRGBA>(const math::Color<GLushort, sRGBA> &c) {
    glColor4usv(c.begin());
}
template <> template <>     // Specialization for GLint w/o alpha
void RENDERER::setColor<GLint, sRGB>(const math::Color<GLint, sRGB> &c) {
    glColor3iv(c.begin());
}
template <> template <>     // Specialization for GLint w/ alpha
void RENDERER::setColor<GLint, sRGBA>(const math::Color<GLint, sRGBA> &c) {
    glColor4iv(c.begin());
}
template <> template <>     // Specialization for GLuint w/o alpha
void RENDERER::setColor<GLuint, sRGB>(const math::Color<GLuint, sRGB> &c) {
    glColor3uiv(c.begin());
}
template <> template <>     // Specialization for GLuint w/ alpha
void RENDERER::setColor<GLuint, sRGBA>(const math::Color<GLuint, sRGBA> &c) {
    glColor4uiv(c.begin());
}
template <> template <>     // Specialization for GLfloat w/o alpha
void RENDERER::setColor<GLfloat, sRGB>(const math::Color<GLfloat, sRGB> &c) {
    glColor3fv(c.begin());
}
template <> template <>     // Specialization for GLfloat w/ alpha
void RENDERER::setColor<GLfloat, sRGBA>(const math::Color<GLfloat, sRGBA> &c) {
    glColor4fv(c.begin());
}
template <> template <>     // Specialization for GLdouble w/o alpha
void RENDERER::setColor<GLdouble, sRGB>(const math::Color<GLdouble, sRGB> &c) {
    glColor3dv(c.begin());
}
template <> template <>     // Specialization for GLdouble w/ alpha
void RENDERER::setColor<GLdouble, sRGBA>(const math::Color<GLdouble, sRGBA> &c) {
    glColor4dv(c.begin());
}


// Current vertex normal
template <> template <>     // Specialization for bytes
void RENDERER::setNormal<GLbyte, 3>(const Vector<GLbyte, 3> &n) {
    glNormal3bv(n.begin());
}
template <> template <>     // Specialization for GLshort integers
void RENDERER::setNormal<GLshort, 3>(const Vector<GLshort, 3> &n) {
    glNormal3sv(n.begin());
}
template <> template <>     // Specialization for integers
void RENDERER::setNormal<GLint, 3>(const Vector<GLint, 3> &n) {
    glNormal3iv(n.begin());
}
template <> template <>     // Specialization for GLfloats
void RENDERER::setNormal<GLfloat, 3>(const Vector<GLfloat, 3> &n) {
    glNormal3fv(n.begin());
}
template <> template <>     // Specialization for GLfloats
void RENDERER::setNormal<GLdouble, 3>(const Vector<GLdouble, 3> &n) {
    glNormal3dv(n.begin());
}

// Current texture coordinates
template <> template <>     // Specialization for 1D GLshort integers
void RENDERER::setTexCoords<GLshort, 1>(const Point<GLshort, 1> &t) {
    glTexCoord1sv(t.begin());
}
template <> template <>     // Specialization for 2D GLshort integers
void RENDERER::setTexCoords<GLshort, 2>(const Point<GLshort, 2> &t) {
    glTexCoord2sv(t.begin());
}
template <> template <>     // Specialization for 3D GLshort integers
void RENDERER::setTexCoords<GLshort, 3>(const Point<GLshort, 3> &t) {
    glTexCoord3sv(t.begin());
}
template <> template <>     // Specialization for 4D GLshort integers
void RENDERER::setTexCoords<GLshort, 4>(const Point<GLshort, 4> &t) {
    glTexCoord4sv(t.begin());
}
template <> template <>     // Specialization for 1D integers
void RENDERER::setTexCoords<GLint, 1>(const Point<GLint, 1> &t) {
    glTexCoord1iv(t.begin());
}
template <> template <>     // Specialization for 2D integers
void RENDERER::setTexCoords<GLint, 2>(const Point<GLint, 2> &t) {
    glTexCoord2iv(t.begin());
}
template <> template <>     // Specialization for 3D integers
void RENDERER::setTexCoords<GLint, 3>(const Point<GLint, 3> &t) {
    glTexCoord3iv(t.begin());
}
template <> template <>     // Specialization for 4D integers
void RENDERER::setTexCoords<GLint, 4>(const Point<GLint, 4> &t) {
    glTexCoord4iv(t.begin());
}
template <> template <>     // Specialization for 1D GLfloats
void RENDERER::setTexCoords<GLfloat, 1>(const Point<GLfloat, 1> &t) {
    glTexCoord1fv(t.begin());
}
template <> template <>     // Specialization for 2D GLfloats
void RENDERER::setTexCoords<GLfloat, 2>(const Point<GLfloat, 2> &t) {
    glTexCoord2fv(t.begin());
}
template <> template <>     // Specialization for 3D GLfloats
void RENDERER::setTexCoords<GLfloat, 3>(const Point<GLfloat, 3> &t) {
    glTexCoord3fv(t.begin());
}
template <> template <>     // Specialization for 4D GLfloats
void RENDERER::setTexCoords<GLfloat, 4>(const Point<GLfloat, 4> &t) {
    glTexCoord4fv(t.begin());
}
template <> template <>     // Specialization for 1D GLdoubles
void RENDERER::setTexCoords<GLdouble, 1>(const Point<GLdouble, 1> &t) {
    glTexCoord1dv(t.begin());
}
template <> template <>     // Specialization for 2D GLdoubles
void RENDERER::setTexCoords<GLdouble, 2>(const Point<GLdouble, 2> &t) {
    glTexCoord2dv(t.begin());
}
template <> template <>     // Specialization for 3D GLdoubles
void RENDERER::setTexCoords<GLdouble, 3>(const Point<GLdouble, 3> &t) {
    glTexCoord3dv(t.begin());
}
template <> template <>     // Specialization for 4D GLdoubles
void RENDERER::setTexCoords<GLdouble, 4>(const Point<GLdouble, 4> &t) {
    glTexCoord4dv(t.begin());
}


// Current edge-of-primitive flag
template <>
void RENDERER::setEdgeFlag(bool isEdge) {
    glEdgeFlag(isEdge);
}


// Create a vertex at a point
template <> template <>     // Specialization for 2D GLshort integers
void RENDERER::vertexAt<GLshort, 2>(const Point<GLshort, 2> &p) {
    glVertex2sv(p.begin());
}
template <> template <>     // Specialization for 3D GLshort integers
void RENDERER::vertexAt<GLshort, 3>(const Point<GLshort, 3> &p) {
    glVertex3sv(p.begin());
}
template <> template <>     // Specialization for 4D GLshort integers
void RENDERER::vertexAt<GLshort, 4>(const Point<GLshort, 4> &p) {
    glVertex4sv(p.begin());
}
template <> template <>     // Specialization for 2D integers
void RENDERER::vertexAt<GLint, 2>(const Point<GLint, 2> &p) {
    glVertex2iv(p.begin());
}
template <> template <>     // Specialization for 3D integers
void RENDERER::vertexAt<GLint, 3>(const Point<GLint, 3> &p) {
    glVertex3iv(p.begin());
}
template <> template <>     // Specialization for 4D integers
void RENDERER::vertexAt<GLint, 4>(const Point<GLint, 4> &p) {
    glVertex4iv(p.begin());
}
template <> template <>     // Specialization for 2D GLfloats
void RENDERER::vertexAt<GLfloat, 2>(const Point<GLfloat, 2> &p) {
    glVertex2fv(p.begin());
}
template <> template <>     // Specialization for 3D GLfloats
void RENDERER::vertexAt<GLfloat, 3>(const Point<GLfloat, 3> &p) {
    glVertex3fv(p.begin());
}
template <> template <>     // Specialization for 4D GLfloats
void RENDERER::vertexAt<GLfloat, 4>(const Point<GLfloat, 4> &p) {
    glVertex4fv(p.begin());
}
template <> template <>     // Specialization for 2D GLdoubles
void RENDERER::vertexAt<GLdouble, 2>(const Point<GLdouble, 2> &p) {
    glVertex2dv(p.begin());
}
template <> template <>     // Specialization for 3D GLdoubles
void RENDERER::vertexAt<GLdouble, 3>(const Point<GLdouble, 3> &p) {
    glVertex3dv(p.begin());
}
template <> template <>     // Specialization for 4D GLdoubles
void RENDERER::vertexAt<GLdouble, 4>(const Point<GLdouble, 4> &p) {
    glVertex4dv(p.begin());
}


// Vertex array data
template <> template <>     // Specialization for 2D GLshort integers
void RENDERER::setVertexArray< Point<GLshort, 2> >(const VertexArray<Point<GLshort, 2> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, va.vertices());
}
template <> template <>     // Specialization for 3D GLshort integers
void RENDERER::setVertexArray< Point<GLshort, 3> >(const VertexArray<Point<GLshort, 3> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, va.vertices());
}
template <> template <>     // Specialization for 4D GLshort integers
void RENDERER::setVertexArray< Point<GLshort, 4> >(const VertexArray<Point<GLshort, 4> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_SHORT, 0, va.vertices());
}
template <> template <>     // Specialization for 2D integers
void RENDERER::setVertexArray< Point<GLint, 2> >(const VertexArray<Point<GLint, 2> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, va.vertices());
}
template <> template <>     // Specialization for 3D integers
void RENDERER::setVertexArray< Point<GLint, 3> >(const VertexArray<Point<GLint, 3> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_INT, 0, va.vertices());
}
template <> template <>     // Specialization for 4D integers
void RENDERER::setVertexArray< Point<GLint, 4> >(const VertexArray<Point<GLint, 4> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_INT, 0, va.vertices());
}
template <> template <>     // Specialization for 2D GLfloats
void RENDERER::setVertexArray< Point<GLfloat, 2> >(const VertexArray<Point<GLfloat, 2> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, va.vertices());
}
template <> template <>     // Specialization for 3D GLfloats
void RENDERER::setVertexArray< Point<GLfloat, 3> >(const VertexArray<Point<GLfloat, 3> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, va.vertices());
}
template <> template <>     // Specialization for 4D GLfloats
void RENDERER::setVertexArray< Point<GLfloat, 4> >(const VertexArray<Point<GLfloat, 4> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, va.vertices());
}
template <> template <>     // Specialization for 2D GLdoubles
void RENDERER::setVertexArray< Point<GLdouble, 2> >(const VertexArray<Point<GLdouble, 2> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_DOUBLE, 0, va.vertices());
}
template <> template <>     // Specialization for 3D GLdoubles
void RENDERER::setVertexArray< Point<GLdouble, 3> >(const VertexArray<Point<GLdouble, 3> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, va.vertices());
}
template <> template <>     // Specialization for 4D GLdoubles
void RENDERER::setVertexArray< Point<GLdouble, 4> >(const VertexArray<Point<GLdouble, 4> > &va) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_DOUBLE, 0, va.vertices());
}


// Normal array data
template <> template <>     // Specialization for bytes
void RENDERER::setNormalArray< Vector<GLbyte, 3> >(const NormalArray< Vector<GLbyte, 3> > &na) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_BYTE, 0, na.normals());
}
template <> template <>     // Specialization for GLshort integers
void RENDERER::setNormalArray< Vector<GLshort, 3> >(const NormalArray< Vector<GLshort, 3> > &na) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_SHORT, 0, na.normals());
}
template <> template <>     // Specialization for integers
void RENDERER::setNormalArray< Vector<GLint, 3> >(const NormalArray< Vector<GLint, 3> > &na) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_INT, 0, na.normals());
}
template <> template <>     // Specialization for GLfloats
void RENDERER::setNormalArray< Vector<GLfloat, 3> >(const NormalArray< Vector<GLfloat, 3> > &na) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, na.normals());
}
template <> template <>     // Specialization for GLdoubles
void RENDERER::setNormalArray< Vector<GLdouble, 3> >(const NormalArray< Vector<GLdouble, 3> > &na) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, na.normals());
}
template <> template <>     // Disable specialization for Omitted
void RENDERER::setNormalArray<Omitted>(const NormalArray<Omitted> &na) {
    glDisableClientState(GL_NORMAL_ARRAY);
}

// Texture coordinate array data
template <> template <>     // Specialization for 1D GLshort integers
void RENDERER::setTexCoordArray< Point<GLshort, 1> >(const TexCoordArray< Point<GLshort, 1> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_SHORT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 2D GLshort integers
void RENDERER::setTexCoordArray< Point<GLshort, 2> >(const TexCoordArray< Point<GLshort, 2> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_SHORT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 3D GLshort integers
void RENDERER::setTexCoordArray< Point<GLshort, 3> >(const TexCoordArray< Point<GLshort, 3> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_SHORT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 4D GLshort integers
void RENDERER::setTexCoordArray< Point<GLshort, 4> >(const TexCoordArray< Point<GLshort, 4> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_SHORT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 1D integers
void RENDERER::setTexCoordArray< Point<GLint, 1> >(const TexCoordArray< Point<GLint, 1> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_INT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 2D integers
void RENDERER::setTexCoordArray< Point<GLint, 2> >(const TexCoordArray< Point<GLint, 2> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_INT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 3D integers
void RENDERER::setTexCoordArray< Point<GLint, 3> >(const TexCoordArray< Point<GLint, 3> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_INT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 4D integers
void RENDERER::setTexCoordArray< Point<GLint, 4> >(const TexCoordArray< Point<GLint, 4> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_INT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 1D GLfloats
void RENDERER::setTexCoordArray< Point<GLfloat, 1> >(const TexCoordArray< Point<GLfloat, 1> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_FLOAT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 2D GLfloats
void RENDERER::setTexCoordArray< Point<GLfloat, 2> >(const TexCoordArray< Point<GLfloat, 2> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 3D GLfloats
void RENDERER::setTexCoordArray< Point<GLfloat, 3> >(const TexCoordArray< Point<GLfloat, 3> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 4D GLfloats
void RENDERER::setTexCoordArray< Point<GLfloat, 4> >(const TexCoordArray< Point<GLfloat, 4> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, 0, tca.texCoords());
}
template <> template <>     // Specialization for 1D GLdoubles
void RENDERER::setTexCoordArray< Point<GLdouble, 1> >(const TexCoordArray< Point<GLdouble, 1> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(1, GL_DOUBLE, 0, tca.texCoords());
}
template <> template <>     // Specialization for 2D GLdoubles
void RENDERER::setTexCoordArray< Point<GLdouble, 2> >(const TexCoordArray< Point<GLdouble, 2> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_DOUBLE, 0, tca.texCoords());
}
template <> template <>     // Specialization for 3D GLdoubles
void RENDERER::setTexCoordArray< Point<GLdouble, 3> >(const TexCoordArray< Point<GLdouble, 3> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_DOUBLE, 0, tca.texCoords());
}
template <> template <>     // Specialization for 4D GLdoubles
void RENDERER::setTexCoordArray< Point<GLdouble, 4> >(const TexCoordArray< Point<GLdouble, 4> > &tca) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_DOUBLE, 0, tca.texCoords());
}
template <> template <>     // Disable specialization for Omitted
void RENDERER::setTexCoordArray<Omitted>(const TexCoordArray<Omitted> &tca) {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


// Color array data
template <> template <>     // Specialization for GLbyte w/o alpha
void RENDERER::setColorArray< Color<GLbyte, sRGB> >(const ColorArray< Color<GLbyte, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_BYTE, 0, ca.colors());
}
template <> template <>     // Specialization for GLbyte w/ alpha
void RENDERER::setColorArray< Color<GLbyte, sRGBA> >(const ColorArray< Color<GLbyte, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_BYTE, 0, ca.colors());
}
template <> template <>     // Specialization for GLubyte w/o alpha
void RENDERER::setColorArray< Color<GLubyte, sRGB> >(const ColorArray< Color<GLubyte, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, ca.colors());
}
template <> template <>     // Specialization for GLubyte w/ alpha
void RENDERER::setColorArray< Color<GLubyte, sRGBA> >(const ColorArray< Color<GLubyte, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, ca.colors());
}
template <> template <>     // Specialization for GLshort w/o alpha
void RENDERER::setColorArray< Color<GLshort, sRGB> >(const ColorArray< Color<GLshort, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_SHORT, 0, ca.colors());
}
template <> template <>     // Specialization for GLshort w/ alpha
void RENDERER::setColorArray< Color<GLshort, sRGBA> >(const ColorArray< Color<GLshort, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_SHORT, 0, ca.colors());
}
template <> template <>     // Specialization for GLushort w/o alpha
void RENDERER::setColorArray< Color<GLushort, sRGB> >(const ColorArray< Color<GLushort, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_SHORT, 0, ca.colors());
}
template <> template <>     // Specialization for GLushort w/ alpha
void RENDERER::setColorArray< Color<GLushort, sRGBA> >(const ColorArray< Color<GLushort, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_SHORT, 0, ca.colors());
}
template <> template <>     // Specialization for GLint w/o alpha
void RENDERER::setColorArray< Color<GLint, sRGB> >(const ColorArray< Color<GLint, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_INT, 0, ca.colors());
}
template <> template <>     // Specialization for GLint w/ alpha
void RENDERER::setColorArray< Color<GLint, sRGBA> >(const ColorArray< Color<GLint, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_INT, 0, ca.colors());
}
template <> template <>     // Specialization for GLuint w/o alpha
void RENDERER::setColorArray< Color<GLuint, sRGB> >(const ColorArray< Color<GLuint, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_INT, 0, ca.colors());
}
template <> template <>     // Specialization for GLuint w/ alpha
void RENDERER::setColorArray< Color<GLuint, sRGBA> >(const ColorArray< Color<GLuint, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_INT, 0, ca.colors());
}
template <> template <>     // Specialization for GLfloat w/o alpha
void RENDERER::setColorArray< Color<GLfloat, sRGB> >(const ColorArray< Color<GLfloat, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, ca.colors());
}
template <> template <>     // Specialization for GLfloat w/ alpha
void RENDERER::setColorArray< Color<GLfloat, sRGBA> >(const ColorArray< Color<GLfloat, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, ca.colors());
}
template <> template <>     // Specialization for GLdouble w/o alpha
void RENDERER::setColorArray< Color<GLdouble, sRGB> >(const ColorArray< Color<GLdouble, sRGB> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_DOUBLE, 0, ca.colors());
}
template <> template <>     // Specialization for GLdouble w/ alpha
void RENDERER::setColorArray< Color<GLdouble, sRGBA> >(const ColorArray< Color<GLdouble, sRGBA> > &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_DOUBLE, 0, ca.colors());
}
template <> template <>     // Disable specialization for Omitted
void RENDERER::setColorArray<Omitted>(const ColorArray<Omitted> &ca) {
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}


// Edge flag array data
template <> template <>     // Specialization for bool (the only choice)
void RENDERER::setEdgeFlagArray<bool>(const EdgeFlagArray<bool> &efa) {
    glEnableClientState(GL_EDGE_FLAG_ARRAY);
    glEdgeFlagPointer(0, efa.edgeFlags());
}
template <> template <>     // Disable specialization for Omitted
void RENDERER::setEdgeFlagArray<Omitted>(const EdgeFlagArray<Omitted> &efa) {
    glDisableClientState(GL_EDGE_FLAG_ARRAY);
}

#endif