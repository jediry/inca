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
#include <inca/util/metaprogramming/Nothing.hpp>
using namespace inca;
using namespace inca::rendering;
using namespace inca::math;

// Import C++-ified OpenGL
#define GL_HPP_IMPORT_INCA
#include "GL.hpp"
using namespace GL;


/*---------------------------------------------------------------------------*
 | OpenGL-specific non-member functions
 *---------------------------------------------------------------------------*/
// Convert between Inca's enumeration for primitives and OpenGL's
inline GLenum translate(PrimitiveType type) {
    switch (type) {
        case Points:        return GL_POINTS;
        case Lines:         return GL_LINES;
        case LineStrip:     return GL_LINE_STRIP;
        case LineLoop:      return GL_LINE_LOOP;
        case Triangles:     return GL_TRIANGLES;
        case TriangleStrip: return GL_TRIANGLE_STRIP;
        case TriangleFan:   return GL_TRIANGLE_FAN;
        case Quads:         return GL_QUADS;
        case QuadStrip:     return GL_QUAD_STRIP;
//        case Polygon:       return GL_POLYGON;
        default:
            std::cerr << "translate(" << type << ") => GLenum: "
                         "unrecognized PrimitiveType" << std::endl;
            return GL_INVALID_ENUM;
    }
}


// Metafunction to determine the GLenum value for a type
template <typename T> struct gl_type { };
template <> struct gl_type<GLbyte>   { static const GLenum value = GL_BYTE; };
template <> struct gl_type<GLubyte>  { static const GLenum value = GL_UNSIGNED_BYTE; };
template <> struct gl_type<GLshort>  { static const GLenum value = GL_SHORT; };
template <> struct gl_type<GLushort> { static const GLenum value = GL_UNSIGNED_SHORT; };
template <> struct gl_type<GLint>    { static const GLenum value = GL_INT; };
template <> struct gl_type<GLuint>   { static const GLenum value = GL_UNSIGNED_INT; };
template <> struct gl_type<GLfloat>  { static const GLenum value = GL_FLOAT; };
template <> struct gl_type<GLdouble> { static const GLenum value = GL_DOUBLE; };



#define GL_GET_BOOLEAN_VALUE(PROPERTY, VALUE) {                     \
    GLboolean result;                                               \
    GL::glGetBooleanv(PROPERTY, &result);                           \
    VALUE = (result == GL_TRUE);                                    \
}
#define GL_GET_INTEGER_VALUE(PROPERTY, VALUE) {                     \
    GL::glGetIntegerv(PROPERTY, &VALUE);                            \
}
#define GL_GET_FLOAT_VALUE(PROPERTY, VALUE) {                       \
    GL::glGetFloatv(PROPERTY, &VALUE);                              \
}
#define GL_GET_DOUBLE_VALUE(PROPERTY, VALUE) {                      \
    GL::glGetDoublev(PROPERTY, &VALUE);                             \
}
#define GL_GET_ARRAY_VALUE(PROPERTY, VALUE) {                       \
    GL::glGetArray(PROPERTY, VALUE);                                \
}
#define GL_GET_POINTER_VALUE(PROPERTY, VALUE) {                     \
    GL::glGetPointerv(PROPERTY, &VALUE);                            \
}
#define GL_ENABLE(PROPERTY, ENABLED) {                              \
    if (ENABLED)    GL::glEnable(PROPERTY);                         \
    else            GL::glDisable(PROPERTY);                        \
}


namespace inca {
    namespace rendering {

// Simplify naming of the things we'll be working with
typedef OpenGLTraits API;

typedef inca::math::Point<GLshort, 1>       Point1s;
typedef inca::math::Point<GLshort, 2>       Point2s;
typedef inca::math::Point<GLshort, 3>       Point3s;
typedef inca::math::Point<GLshort, 4>       Point4s;
typedef inca::math::Point<GLint, 1>         Point1i;
typedef inca::math::Point<GLint, 2>         Point2i;
typedef inca::math::Point<GLint, 3>         Point3i;
typedef inca::math::Point<GLint, 4>         Point4i;
typedef inca::math::Point<GLfloat, 1>       Point1f;
typedef inca::math::Point<GLfloat, 2>       Point2f;
typedef inca::math::Point<GLfloat, 3>       Point3f;
typedef inca::math::Point<GLfloat, 4>       Point4f;
typedef inca::math::Point<GLdouble, 1>      Point1d;
typedef inca::math::Point<GLdouble, 2>      Point2d;
typedef inca::math::Point<GLdouble, 3>      Point3d;
typedef inca::math::Point<GLdouble, 4>      Point4d;

typedef inca::math::Vector<GLbyte, 2>       Vector2b;
typedef inca::math::Vector<GLbyte, 3>       Vector3b;
typedef inca::math::Vector<GLbyte, 4>       Vector4b;
typedef inca::math::Vector<GLshort, 2>      Vector2s;
typedef inca::math::Vector<GLshort, 3>      Vector3s;
typedef inca::math::Vector<GLshort, 4>      Vector4s;
typedef inca::math::Vector<GLint, 2>        Vector2i;
typedef inca::math::Vector<GLint, 3>        Vector3i;
typedef inca::math::Vector<GLint, 4>        Vector4i;
typedef inca::math::Vector<GLfloat, 2>      Vector2f;
typedef inca::math::Vector<GLfloat, 3>      Vector3f;
typedef inca::math::Vector<GLfloat, 4>      Vector4f;
typedef inca::math::Vector<GLdouble, 2>     Vector2d;
typedef inca::math::Vector<GLdouble, 3>     Vector3d;
typedef inca::math::Vector<GLdouble, 4>     Vector4d;

typedef inca::math::Color<GLbyte, inca::math::sRGB<false> >     Color3b;
typedef inca::math::Color<GLbyte, inca::math::sRGB<true> >      Color4b;
typedef inca::math::Color<GLubyte, inca::math::sRGB<false> >    Color3ub;
typedef inca::math::Color<GLubyte, inca::math::sRGB<true> >     Color4ub;
typedef inca::math::Color<GLshort, inca::math::sRGB<false> >    Color3s;
typedef inca::math::Color<GLshort, inca::math::sRGB<true> >     Color4s;
typedef inca::math::Color<GLushort, inca::math::sRGB<false> >   Color3us;
typedef inca::math::Color<GLushort, inca::math::sRGB<true> >    Color4us;
typedef inca::math::Color<GLint, inca::math::sRGB<false> >      Color3i;
typedef inca::math::Color<GLint, inca::math::sRGB<true> >       Color4i;
typedef inca::math::Color<GLuint, inca::math::sRGB<false> >     Color3ui;
typedef inca::math::Color<GLuint, inca::math::sRGB<true> >      Color4ui;
typedef inca::math::Color<GLfloat, inca::math::sRGB<false> >    Color3f;
typedef inca::math::Color<GLfloat, inca::math::sRGB<true> >     Color4f;
typedef inca::math::Color<GLdouble, inca::math::sRGB<false> >   Color3d;
typedef inca::math::Color<GLdouble, inca::math::sRGB<true> >    Color4d;

typedef API::Color     RGB;
typedef API::Normal    Normal;
typedef API::TexCoord  TexCoord;


/*---------------------------------------------------------------------------*
 | IMR -- "Current" sub-object control functions
 *---------------------------------------------------------------------------*/

// The currently selected matrix stack
template <>
void API::getHardwareState<API::CurrentMatrixStack>(IDType & id) {
    GLint glID;
    GL_GET_INTEGER_VALUE(GL_MATRIX_MODE, glID);
    id = glID;
}
template <>
void API::setHardwareState<API::CurrentMatrixStack>(IDType id) {
//    if (id == GL_PROJECTION) std::cerr << "Proj is current\n";
//    if (id == GL_MODELVIEW) std::cerr << "ModV is current\n";
    GL::glMatrixMode(id);
}

SizeType API::matrixStackCount() {
    SizeType count = 0;
    count = 2;          // FIXME: We should check for the validity of color matrix here
    return count;
}
SizeType API::lightingUnitCount() {
    GLint count;
    GL_GET_INTEGER_VALUE(GL_MAX_LIGHTS, count)
    return count;
}
SizeType API::texturingUnitCount() {
#ifdef GL_MAX_TEXTURE_UNITS_ARB
    GLint count;
    GL_GET_INTEGER_VALUE(GL_MAX_TEXTURE_UNITS_ARB, count)
    return count;
#else
    return 1;
#endif
}
IDType API::matrixStackID(IndexType index) {
    switch (index) {
        case 0:     return GL_PROJECTION;
        case 1:     return GL_MODELVIEW;
        default:
            std::cerr << "OpenGL::matrixStackID(" << index << "): "
                         "invalid matrix stack index" << std::endl;
            return GL_INVALID_ENUM;
    }
}
IDType API::lightingUnitID(IndexType index)  { return GL_LIGHT0 + index; }
IDType API::texturingUnitID(IndexType index) {
#ifdef GL_MAX_TEXTURE_UNITS_ARB
    return GL_TEXTURE0_ARB + index;
#else
    return 0;
#endif
}

IndexType API::projectionMatrixIndex()  { return 0; }
IndexType API::viewMatrixIndex()        { return 1; }
IndexType API::worldMatrixIndex()       { return -1; }
IndexType API::colorMatrixIndex()       { return -1; }


/*---------------------------------------------------------------------------*
 | IMR -- miscellaneous functions
 *---------------------------------------------------------------------------*/
// Buffer clearing
void API::clearBuffers(IDType ids) {
    GL::glClear(ids);
}


/*---------------------------------------------------------------------------*
 | IMR::Rasterizer -- Rasterization mode toggles
 *---------------------------------------------------------------------------*/
// Depth buffering
template <>
void API::getHardwareState<API::DepthBuffering>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_DEPTH_TEST, enabled);
}
template <>
void API::setHardwareState<API::DepthBuffering>(bool enabled) {
    GL_ENABLE(GL_DEPTH_TEST, enabled);
}

// Alpha blending
template <>
void API::getHardwareState<API::AlphaBlending>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_BLEND, enabled);
}
template <>
void API::setHardwareState<API::AlphaBlending>(bool enabled) {
    GL_ENABLE(GL_BLEND, enabled);
}

// Polygon face culling
template <>
void API::getHardwareState<API::FaceCulling>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_CULL_FACE, enabled);
}
template <>
void API::setHardwareState<API::FaceCulling>(bool enabled) {
    GL_ENABLE(GL_CULL_FACE, enabled);
}

// Hardware lighting
template <>
void API::getHardwareState<API::Lighting>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_LIGHTING, enabled);
}
template <>
void API::setHardwareState<API::Lighting>(bool enabled) {
    GL_ENABLE(GL_LIGHTING, enabled);
}

// Point antialiasing
template <>
void API::getHardwareState<API::PointSmoothing>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_POINT_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::PointSmoothing>(bool enabled) {
    GL_ENABLE(GL_POINT_SMOOTH, enabled);
}

// Line antialiasing
template <>
void API::getHardwareState<API::LineSmoothing>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_LINE_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::LineSmoothing>(bool enabled) {
    GL_ENABLE(GL_LINE_SMOOTH, enabled);
}

// Polygon antialiasing
template <>
void API::getHardwareState<API::PolygonSmoothing>(bool & enabled) {
    GL_GET_BOOLEAN_VALUE(GL_POLYGON_SMOOTH, enabled);
}
template <>
void API::setHardwareState<API::PolygonSmoothing>(bool enabled) {
    GL_ENABLE(GL_POLYGON_SMOOTH, enabled);
}


/*---------------------------------------------------------------------------*
 | IMR::Rasterizer -- Rasterization parameter functions
 *---------------------------------------------------------------------------*/
// Background color
template <>
void API::getHardwareState<API::BackgroundColor>(RGB & color) {
    GL_GET_ARRAY_VALUE(GL_COLOR_CLEAR_VALUE, color);
}
template <>
void API::setHardwareState<API::BackgroundColor>(const RGB & color) {
    glClearColor(color);
}

// Point diameter
template <>
void API::getHardwareState<API::PointDiameter>(GLfloat & diameter) {
    GL_GET_FLOAT_VALUE(GL_POINT_SIZE, diameter);
}
template <>
void API::setHardwareState<API::PointDiameter>(GLfloat diameter) {
    glPointSize(diameter);
}

// Line width
template <>
void API::getHardwareState<API::LineWidth>(GLfloat & width) {
    GL_GET_FLOAT_VALUE(GL_LINE_WIDTH, width);
}
template <>
void API::setHardwareState<API::LineWidth>(GLfloat width) {
    glLineWidth(width);
}

// Polygon offset
template <>
void API::getHardwareState<API::PolygonOffset>(GLfloat & offset) {
    GL_GET_FLOAT_VALUE(GL_POLYGON_OFFSET_UNITS, offset);
}
template <>
void API::setHardwareState<API::PolygonOffset>(GLfloat offset) {
    glPolygonOffset(0.0f, offset);
}

// Shading mode
template <>
void API::getHardwareState<API::ShadingModel>(::inca::rendering::ShadingModel & model) {
    GLint result;
    GL_GET_INTEGER_VALUE(GL_SHADE_MODEL, result);
    switch (result) {
        case GL_SMOOTH: model = SmoothShade; break;
        case GL_FLAT:   model = FlatShade;   break;
        default:        model = Paint;       break;
    }
}
template <>
void API::setHardwareState<API::ShadingModel>(::inca::rendering::ShadingModel model) {
    switch (model) {
        case SmoothShade: glShadeModel(GL_SMOOTH); break;
        case FlatShade:   glShadeModel(GL_FLAT);   break;
        default:          glShadeModel(GL_SMOOTH); break;
    }
}

// Drawing color
template <>
void API::getHardwareState<API::CurrentColor>(RGB & color) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_COLOR, color);
}
template <>
void API::setHardwareState<API::CurrentColor>(const RGB & color) {
    GL::glColor(color);
}

// Drawing normal
template <>
void API::getHardwareState<API::CurrentNormal>(Normal & normal) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_NORMAL, normal);
}
template <>
void API::setHardwareState<API::CurrentNormal>(const Normal & normal) {
    GL::glNormal(normal);
}

// Current texture coordinates
template <>
void API::getHardwareState<API::CurrentTexCoord>(TexCoord & texCoord) {
    GL_GET_ARRAY_VALUE(GL_CURRENT_TEXTURE_COORDS, texCoord);
}
template <>
void API::setHardwareState<API::CurrentTexCoord>(const TexCoord & texCoord) {
    GL::glTexCoord(texCoord);
}

// Current edge flag
template <>
void API::getHardwareState<API::CurrentEdgeFlag>(bool & edge) {
    GL_GET_BOOLEAN_VALUE(GL_EDGE_FLAG, edge);
}
template <>
void API::setHardwareState<API::CurrentEdgeFlag>(bool edge) {
    GL::glEdgeFlag(edge);
}


/*---------------------------------------------------------------------------*
 | IMR::Rasterizer -- Vertex array functions
 *---------------------------------------------------------------------------*/
// The folloing macros implement the getHardwareState function for all the
// vertex data array properties (e.g., pointer, stride).
#define DATA_ARRAY_GETTER(NAME, ENUM)                                       \
template <>                                                                 \
void API::getHardwareState<API::NAME ## ArrayPointer>(void * & ptr) {       \
    if (glIsEnabled(ENUM))                                                  \
        GL_GET_POINTER_VALUE(ENUM ## _POINTER, ptr)                         \
    else                                                                    \
        ptr = NULL;                                                         \
}                                                                           \
template <>                                                                 \
void API::getHardwareState<API::NAME ## ArrayStride>(DifferenceType & stride) {   \
    GL_GET_INTEGER_VALUE(ENUM ## _STRIDE, stride);                          \
}

DATA_ARRAY_GETTER(Vertex,   GL_VERTEX_ARRAY)
DATA_ARRAY_GETTER(Normal,   GL_NORMAL_ARRAY)
DATA_ARRAY_GETTER(TexCoord, GL_TEXTURE_COORD_ARRAY)
DATA_ARRAY_GETTER(Color,    GL_COLOR_ARRAY)
DATA_ARRAY_GETTER(EdgeFlag, GL_EDGE_FLAG_ARRAY)

#undef DATA_ARRAY_GETTER


// The following macros implement the setHardwareState function
// for all the permissable combinations of data type and dimensionality for
// the vertex data arrays. Since glEdgeFlagPointer and glNormalPointer have
// unusual signatures, they must be handled differently.
#define DATA_ARRAY_SETTER(NAME, ENUM, TYPE)                                 \
template <>                                                                 \
void API::setHardwareState<API::NAME ## ArrayPointer, TYPE>(TYPE const * ptr,\
                                            DifferenceType stride) {        \
    if (ptr == NULL)                                                        \
        GL::glDisableClientState(ENUM);                                     \
    else {                                                                  \
        GL::glEnableClientState(ENUM);                                      \
        GL::gl ## NAME ## Pointer(TYPE::dimensionality,                     \
                                  gl_type<TYPE::ElementType>::value,        \
                                  stride, ptr);                             \
    }                                                                       \
}

#define NORMAL_ARRAY_SETTER(TYPE)                                           \
template <>                                                                 \
void API::setHardwareState<API::NormalArrayPointer, TYPE>(TYPE const * ptr, \
                                            DifferenceType stride) {        \
    if (ptr == NULL)                                                        \
        GL::glDisableClientState(GL_NORMAL_ARRAY);                          \
    else {                                                                  \
        GL::glEnableClientState(GL_NORMAL_ARRAY);                           \
        GL::glNormalPointer(gl_type<TYPE::ElementType>::value,              \
                            stride, ptr);                                   \
    }                                                                       \
}
#define EDGE_FLAG_ARRAY_SETTER(TYPE)                                        \
template <>                                                                 \
void API::setHardwareState<API::EdgeFlagArrayPointer, TYPE>(TYPE const * ptr,\
                                            DifferenceType stride) {        \
    if (ptr == NULL)                                                        \
        GL::glDisableClientState(GL_EDGE_FLAG_ARRAY);                       \
    else {                                                                  \
        GL::glEnableClientState(GL_EDGE_FLAG_ARRAY);                        \
        GL::glEdgeFlagPointer(stride, ptr);                                 \
    }                                                                       \
}

#define NOTHING_ARRAY_SETTER(NAME, ENUM)                                    \
template <>                                                                 \
void API::setHardwareState<API::NAME ## ArrayPointer, Nothing>(Nothing const * ptr,\
                                            DifferenceType stride) {        \
    GL::glDisableClientState(ENUM);                                         \
}

DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point2s)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point3s)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point4s)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point2i)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point3i)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point4i)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point2f)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point3f)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point4f)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point2d)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point3d)
DATA_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY, Point4d)
NOTHING_ARRAY_SETTER(Vertex, GL_VERTEX_ARRAY)

NORMAL_ARRAY_SETTER(Vector3b)
NORMAL_ARRAY_SETTER(Vector3s)
NORMAL_ARRAY_SETTER(Vector3i)
NORMAL_ARRAY_SETTER(Vector3f)
NORMAL_ARRAY_SETTER(Vector3d)
NOTHING_ARRAY_SETTER(Normal, GL_NORMAL_ARRAY)

DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point1s)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point2s)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point3s)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point4s)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point1i)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point2i)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point3i)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point4i)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point1f)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point2f)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point3f)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point4f)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point1d)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point2d)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point3d)
DATA_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY, Point4d)
NOTHING_ARRAY_SETTER(TexCoord, GL_TEXTURE_COORD_ARRAY)

DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3b)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4b)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3ub)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4ub)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3s)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4s)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3us)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4us)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3i)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4i)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3ui)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4ui)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3f)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4f)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color3d)
DATA_ARRAY_SETTER(Color, GL_COLOR_ARRAY, Color4d)
NOTHING_ARRAY_SETTER(Color, GL_COLOR_ARRAY)

EDGE_FLAG_ARRAY_SETTER(GLboolean)
NOTHING_ARRAY_SETTER(EdgeFlag, GL_EDGE_FLAG_ARRAY)


#undef DATA_ARRAY_SETTER
#undef NORMAL_ARRAY_SETTER
#undef EDGE_FLAG_ARRAY_SETTER
#undef NOTHING_ARRAY_SETTER


/*---------------------------------------------------------------------------*
 | IMR::Rasterizer -- Primitive rendering functions
 *---------------------------------------------------------------------------*/

// Start/end rendering a geometric primitive
void API::beginPrimitive(PrimitiveType type) { GL::glBegin(translate(type)); }
void API::endPrimitive() { GL::glEnd(); }

// Render a vertex from the current vertex array
void API::renderVertexIndex(IndexType i) { GL::glArrayElement(i); }

// Render a range of vertices from the current arrays in a single call
void API::renderArrayRange(PrimitiveType type, IndexType from, SizeType count) {
    GL::glDrawArrays(translate(type), from, count);
}


#define RENDER_VERTEX(TYPE)                                                 \
    template <>                                                             \
    void API::renderVertexAt<TYPE>(TYPE x, TYPE y) {                        \
        glVertex(x, y);                                                     \
    }                                                                       \
    template <>                                                             \
    void API::renderVertexAt<TYPE>(TYPE x, TYPE y, TYPE z) {                \
        glVertex(x, y, z);                                                  \
    }                                                                       \
    template <>                                                             \
    void API::renderVertexAt<TYPE>(TYPE x, TYPE y, TYPE z, TYPE w) {        \
        glVertex(x, y, z, w);                                               \
    }                                                                       \
    template <>                                                             \
    void API::renderVertexAt<Point<TYPE, 2> >(const Point<TYPE, 2> & v) {   \
        glVertex(v);                                                        \
    }                                                                       \
    template <>                                                             \
    void API::renderVertexAt<Point<TYPE, 3> >(const Point<TYPE, 3> & v) {   \
        glVertex(v);                                                        \
    }                                                                       \
    template <>                                                             \
    void API::renderVertexAt<Point<TYPE, 4> >(const Point<TYPE, 4> & v) {   \
        glVertex(v);                                                        \
    }

RENDER_VERTEX(GLshort)
RENDER_VERTEX(GLint)
RENDER_VERTEX(GLfloat)
RENDER_VERTEX(GLdouble)

#undef RENDER_VERTEX


/*---------------------------------------------------------------------------*
 | IMR::MatrixStack -- stack depth functions
 *---------------------------------------------------------------------------*/

// Retrieve current stack depth
template <>
void API::getImplementationLimit<API::MatrixStackDepth>(IDType id, IndexType & max) {
    switch (id) {
        case GL_MODELVIEW:
            GL_GET_INTEGER_VALUE(GL_MAX_MODELVIEW_STACK_DEPTH, max)
            break;
        case GL_PROJECTION:
            GL_GET_INTEGER_VALUE(GL_MAX_PROJECTION_STACK_DEPTH, max)
            break;
//        case GL_COLOR:
//            GL_GET_INTEGER_VALUE(GL_MAX_COLOR_STACK_DEPTH, max)
//            break;
        case GL_TEXTURE:
            GL_GET_INTEGER_VALUE(GL_MAX_TEXTURE_STACK_DEPTH, max)
            break;
        default:
            std::cerr << "OpenGL::getImplementationLimit<"
                         "MatrixStackDepth>(" << id << "): "
                         "unrecognized matrix ID" << std::endl;
            max = 0;
    }
}

// Retrieve stack depth limit
template <>
void API::getHardwareState<API::MatrixStackDepth>(IDType id, IndexType & depth) {
    switch (id) {
        case GL_MODELVIEW:
            GL_GET_INTEGER_VALUE(GL_MODELVIEW_STACK_DEPTH, depth)
            break;
        case GL_PROJECTION:
            GL_GET_INTEGER_VALUE(GL_PROJECTION_STACK_DEPTH, depth)
            break;
//        case GL_COLOR:
//            GL_GET_INTEGER_VALUE(GL_COLOR_STACK_DEPTH, depth)
//            break;
        case GL_TEXTURE:
            GL_GET_INTEGER_VALUE(GL_TEXTURE_STACK_DEPTH, depth)
            break;
        default:
            std::cerr << "OpenGL::getHardwareState<"
                         "MatrixStackDepth>(" << id << "): "
                         "unrecognized matrix ID" << std::endl;
            depth = 0;
    }
}


// Push a matrix onto the current stack
void API::pushMatrix(IDType id) { GL::glPushMatrix(); }

// Pop a matrix off the current stack
void API::popMatrix(IDType id) { GL::glPopMatrix(); }


/*---------------------------------------------------------------------------*
 | IMR::MatrixStack -- Matrix get/set/modify functions
 *---------------------------------------------------------------------------*/

// Reset to the identity matrix
void API::resetMatrix(IDType id) {
//    std::cerr << "reset\n";
    GL::glLoadIdentity();
}

// Retrieve the current contents of the stack
template <>
void API::getHardwareState<API::CurrentMatrix>(IDType id, Matrix & m) {
    switch (id) {
        case GL_MODELVIEW:
//            std::cerr << "get MV\n";
            glGetFloatv(GL_MODELVIEW_MATRIX, m.begin());
            break;
        case GL_PROJECTION:
//            std::cerr << "get P\n";
            glGetFloatv(GL_PROJECTION_MATRIX, m.begin());
            break;
//        case GL_COLOR:
//            glGetFloatv(GL_COLOR_MATRIX, m.begin());
//            break;
        case GL_TEXTURE:
            glGetFloatv(GL_TEXTURE_MATRIX, m.begin());
            break;
    }
}

// Set the current transformation matrix
template <>
void API::setHardwareState<API::CurrentMatrix>(IDType id, const Matrix & m) {
//    std::cerr << "load\n";
    GL::glLoadMatrix(m);
}

// Pre-multiply the top of the stack by 'm' (i.e.: MS = m % MS)
void API::premultiplyMatrix(IDType id, const Matrix & m) {
    Matrix tmp;
    getHardwareState<API::CurrentMatrix>(id, tmp);
    GL::glLoadMatrix(m);
    GL::glMultMatrix(tmp);
}

// Post-multiply the top of the stack by 'm' (i.e.: MS = MS % m)
void API::postmultiplyMatrix(IDType id, const Matrix & m) {
    GL::glMultMatrix(m);
}


// Scale by a uniform scaling factor
void API::scaleMatrix(IDType id, geometry_t s) {
    GL::glScale(s, s, s);
}

// Scale non-uniformly in the three directions
void API::scaleMatrix(IDType id, const Vector3D & s) {
    GL::glScale(s);
}

// Rotate by an angle (in radians) around an axis
void API::rotateMatrix(IDType id, geometry_t angle, const Vector3D & around) {
    GL::glRotate(::inca::math::radiansToDegrees(angle), around);
}

// Translate by a vector
template <>
void API::translateMatrix<Point<GLfloat, 3> >(IDType id, const Point<GLfloat, 3> & v) {
    GL::glTranslate(v);
}
template <>
void API::translateMatrix<Vector<GLfloat, 3> >(IDType id, const Vector<GLfloat, 3> & v) {
    GL::glTranslate(v);
}


/*---------------------------------------------------------------------------*
 | IMR::MatrixStack -- Matrix get/set/modify functions
 *---------------------------------------------------------------------------*/
template <>
void API::getHardwareState<API::ViewportBounds>(Region & r) {
    inca::Array<GLint, 2> bounds[2];
    glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(bounds));
    r.setBasesAndSizes(bounds[0], bounds[1]);
}
template <>
void API::setHardwareState<API::ViewportBounds>(const Region & r) {
    glViewport(r.base(0), r.base(1), r.size(0), r.size(1));
}


/*---------------------------------------------------------------------------*
 | IMR::LightingUnit -- Light functions
 *---------------------------------------------------------------------------*/
// Light enabled
template <>
void API::getHardwareState<API::LightingUnitEnabled>(IDType id, bool & enabled) {
    GL_GET_BOOLEAN_VALUE(id, enabled);
}
template <>
void API::setHardwareState<API::LightingUnitEnabled>(IDType id, bool enabled) {
    GL_ENABLE(id, enabled);
}

// Light position
template <>
void API::setHardwareState<API::LightingUnitPosition>(IDType id, const Point3D & p) {
    Point3D::scalar_t args[4] = { p[0], p[1], p[2], Point3D::scalar_t(1) };
    GL::glLight(id, GL_POSITION, args);
}

// Light color
template <>
void API::setHardwareState<API::LightingUnitAmbientColor>(IDType id, const Color & c) {
    GL::glLight(id, GL_AMBIENT, c.elements());
}
template <>
void API::setHardwareState<API::LightingUnitDiffuseColor>(IDType id, const Color & c) {
    GL::glLight(id, GL_DIFFUSE, c.elements());
}
template <>
void API::setHardwareState<API::LightingUnitSpecularColor>(IDType id, const Color & c) {
    GL::glLight(id, GL_SPECULAR, c.elements());
}

    }
}
