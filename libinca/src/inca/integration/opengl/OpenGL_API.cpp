/*
 * File: OpenGL_API.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 */

// Import class definition
#include <inca/rendering.hpp>
using namespace inca::rendering;
using namespace inca::math;
using namespace inca::imaging;
using namespace inca::world;


// Declare a convenient alias for this template specialization
typedef immediate_mode_rendering_api<OpenGL> OpenGL_API;

// Import OpenGL
#if __MS_WINDOZE__
    // Windows OpenGL seems to need this
#   include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>


namespace inca {
    namespace rendering {
/*---------------------------------------------------------------------------*
 | Rendering functions
 *---------------------------------------------------------------------------*/
#if 0
void OpenGL_API::initialize() {
    glEnable(GL_DEPTH_TEST);        // We want Z-buffering
    glCullFace(GL_BACK);            // The BACK ones, you idiot!

    // Offset filled polygons so that we can draw wireframe on top
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    // Set up a reasonable alpha blending function for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw back faces (when not culled) as lines, for wireframe
    glPolygonMode(GL_BACK, GL_LINE);

    // Handle scaling correctly
    glEnable(GL_NORMALIZE);     // We may do non-uniform scaling...
//    glEnable(GL_RESCALE_NORMAL);
}
#endif
/*---------------------------------------------------------------------------*
 | Framebuffer control functions
 *---------------------------------------------------------------------------*/
// Clear the framebuffer
void OpenGL_API::clear_framebuffer() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#if 0
// Access the framebuffer (or some sub-region)
ImagePtr OpenGL_API::framebuffer() const {

}

ImagePtr OpenGL_API::framebuffer(index_t x, index_t y,
                                     size_t w, size_t h) const {

}

void OpenGL_API::framebuffer(index_t x, index_t y) const {

}
#endif

// Change the framebuffer's size
void OpenGL_API::resize_framebuffer(size_t w, size_t h) {
    // Set up the clipping rectangle accordingly
    glViewport(0, 0, w, h);
    cerr << "new viewport size " << w << 'x' << h << endl;
}


/*---------------------------------------------------------------------------*
 | Rendering state control functions
 *---------------------------------------------------------------------------*/
void OpenGL_API::lock_z_buffer(bool locked) {
    locked ? glDepthMask(GL_FALSE) : glDepthMask(GL_TRUE);
}
bool OpenGL_API::is_z_buffer_locked() {
    GLboolean result;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &result);
    return static_cast<bool>(result);
}
void OpenGL_API::enable_z_buffer(bool enabled) {
    enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}
bool OpenGL_API::is_z_buffer_enabled() {
    return static_cast<bool>(glIsEnabled(GL_DEPTH_TEST));
}
void OpenGL_API::enable_alpha_blending(bool enabled) {
    enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}
bool OpenGL_API::is_alpha_blending_enabled() {
    return static_cast<bool>(glIsEnabled(GL_BLEND));
}
void OpenGL_API::enable_lighting(bool enabled) {
    enabled ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}
bool OpenGL_API::is_lighting_enabled() {
    return static_cast<bool>(glIsEnabled(GL_LIGHTING));
}

void OpenGL_API::set_shading_mode(ShadingMode mode) {
    switch (mode) {
        case SmoothShade:   glShadeModel(GL_SMOOTH);    break;
        case FlatShade:     glShadeModel(GL_FLAT);      break;
        case Paint:         /* Do nothing */;           break;
    }
}

ShadingMode OpenGL_API::shading_mode() {
    GLint value;
    glGetIntegerv(GL_SHADE_MODEL, &value);
    if (value == GL_SMOOTH) return SmoothShade;
    else                    return FlatShade;
}


/*---------------------------------------------------------------------------*
 | Transformation matrix functions (TI)
 *---------------------------------------------------------------------------*/
void OpenGL_API::select_projection_matrix() { glMatrixMode(GL_PROJECTION); }
void OpenGL_API::select_transformation_matrix() { glMatrixMode(GL_MODELVIEW); }
void OpenGL_API::reset_selected_matrix() { glLoadIdentity(); }
void OpenGL_API::push_matrix() { glPushMatrix(); }
void OpenGL_API::pop_matrix() { glPopMatrix(); }

template <>
void OpenGL_API::apply_translation(const math::ScalarList<float, 3> &t) {
    glTranslatef(t[0], t[1], t[2]);
}

template <>
void OpenGL_API::apply_translation(const math::ScalarList<double, 3> &t) {
    glTranslated(t[0], t[1], t[2]);
}

template <>
void OpenGL_API::unapply_translation(const math::ScalarList<float, 3> &t) {
    glTranslatef(-t[0], -t[1], -t[2]);
}

template <>
void OpenGL_API::unapply_translation(const math::ScalarList<double, 3> &t) {
    glTranslated(-t[0], -t[1], -t[2]);
}

template <>
void OpenGL_API::apply_rotation(const math::Quaternion<double> &q) {
    // If the first element of q is 1, this is an identity rotation
    if (! effectivelyEqual(q[0], 1.0)) {
        // Here we split 'q' into an axis and a rotation angle about that axis
        const double angle = radiansToDegrees(2.0 * arccos(q[0]));
        const Vector<double, 3> around = axis(q);
        glRotated(angle, around[0], around[1], around[2]);
    }
}

template <>
void OpenGL_API::unapply_rotation(const math::Quaternion<double> &q) {
    if (! effectivelyEqual(q[0], 1.0)) {
        // Note that we're getting the NEGATION of the rotation angle here
        const double angle = -radiansToDegrees(2.0 * arccos(q[0]));
        const Vector<double, 3> around = axis(q);
        glRotated(angle, around[0], around[1], around[2]);
    }
}

template <>
void OpenGL_API::apply_scaling(const math::ScalarList<float, 3> &s) {
    glScaled(s[0], s[1], s[2]);
}

template <>
void OpenGL_API::unapply_scaling(const math::ScalarList<float, 3> &s) {
    glScaled(1.0 / s[0], 1.0 / s[1], 1.0 / s[2]);
}

template <>
void OpenGL_API::apply_scaling(const math::ScalarList<double, 3> &s) {
    glScaled(s[0], s[1], s[2]);
}

template <>
void OpenGL_API::unapply_scaling(const math::ScalarList<double, 3> &s) {
    glScaled(1.0 / s[0], 1.0 / s[1], 1.0 / s[2]);
}

template <>
void OpenGL_API::apply_orthographic_projection(
                const math::Vector<float, 2> &extents,
                const math::Vector<float, 2> &clipping) {
    float dx = extents[0] / 2.0,
          dy = extents[1] / 2.0;
    glOrtho(-dx, dx, -dy, dy, clipping[0], clipping[1]);
}

template <>
void OpenGL_API::apply_orthographic_projection(
                const math::Vector<double, 2> &extents,
                const math::Vector<double, 2> &clipping) {
    double dx = extents[0] / 2.0,
           dy = extents[1] / 2.0;
    cerr << "OpenGL ortho projection (" << -dx << ", " << dx << ", " << -dy << ", " << dy << ", " << clipping[0] << ", " << clipping[1] << endl;
    glOrtho(-dx, dx, -dy, dy, clipping[0], clipping[1]);
}

template <>
void OpenGL_API::apply_perspective_projection(
                const math::Vector<float, 2> &angles,
                const math::Vector<float, 2> &clipping) {
    float aspectRatio = angles[1] / angles[0];
    gluPerspective(radiansToDegrees(angles[1]), aspectRatio,
                   clipping[0], clipping[1]);
}

template <>
void OpenGL_API::apply_perspective_projection(
                const math::Vector<double, 2> &angles,
                const math::Vector<double, 2> &clipping) {
    double aspectRatio = angles[1] / angles[0];
    gluPerspective(radiansToDegrees(angles[1]), aspectRatio,
                   clipping[0], clipping[1]);
}


/*---------------------------------------------------------------------------*
 | True-immediate mode rendering functions (TI)
 *---------------------------------------------------------------------------*/
void OpenGL_API::begin_render_immediate(PrimitiveType type) {
    switch (type) {
        case Points:                glBegin(GL_POINTS);         break;
        case Lines:                 glBegin(GL_LINES);          break;
        case LineStrip:             glBegin(GL_LINE_STRIP);     break;
        case LineLoop:              glBegin(GL_LINE_LOOP);      break;
        case Triangles:             glBegin(GL_TRIANGLES);      break;
        case TriangleStrip:         glBegin(GL_TRIANGLE_STRIP); break;
        case TriangleFan:           glBegin(GL_TRIANGLE_FAN);   break;
        case Quads:                 glBegin(GL_QUADS);          break;
        case QuadStrip:             glBegin(GL_QUAD_STRIP);     break;
        case Polygon:               glBegin(GL_POLYGON);        break;
//        case BillboardSprite:       glBegin();
        default:
            logger << "OpenGL::beginRenderImmediate(" << type
                   << "): Unsupported PrimitiveType";
            logger.warning();
    }
}

void OpenGL_API::end_render_immediate() {
    glEnd();
}

template <>
void OpenGL_API::set_background_color<float, sRGB, false>(
        const Color<float, sRGB, false> &c) {
    glClearColor(c[0], c[1], c[2], 1.0f);
}

template <>
void OpenGL_API::set_background_color<double, sRGB, false>(
        const Color<double, sRGB, false> &c) {
    glClearColor(c[0], c[1], c[2], 1.0);
}

template <>
void OpenGL_API::set_background_color<float, sRGB, true>(
        const Color<float, sRGB, true> &c) {
    glClearColor(c[0], c[1], c[2], c[3]);
}

template <>
void OpenGL_API::set_background_color<double, sRGB, true>(
        const Color<double, sRGB, true> &c) {
    glClearColor(c[0], c[1], c[2], c[3]);
}

template <>
void OpenGL_API::set_drawing_color<float, sRGB, false>(
        const Color<float, sRGB, false> &c) {
    glColor3f(c[0], c[1], c[2]);
}

template <>
void OpenGL_API::set_drawing_color<float, sRGB, true>(
        const Color<float, sRGB, true> &c) {
    glColor4f(c[0], c[1], c[2], c[3]);
}

template <>
void OpenGL_API::set_drawing_color<double, sRGB, false>(
        const Color<double, sRGB, false> &c) {
    glColor3d(c[0], c[1], c[2]);
}

template <>
void OpenGL_API::set_drawing_color<double, sRGB, true>(
        const Color<double, sRGB, true> &c) {
    glColor4d(c[0], c[1], c[2], c[3]);
}

template <>
void OpenGL_API::set_normal<double, 3>(const Vector<double, 3> &n) {
    glNormal3d(n[0], n[1], n[2]);
}

template <>
void OpenGL_API::set_texture_coordinates<float, 3>(const Point<float, 3> &t) {
    glTexCoord3f(t[0], t[1], t[2]);
}

template <>
void OpenGL_API::render_vertex<double, 2>(const Point<double, 2> &v) {
    glVertex2d(v[0], v[1]);
}


/*---------------------------------------------------------------------------*
 | Semi-immediate mode functions
 *---------------------------------------------------------------------------*/
#if 0
void OpenGL_API::render() {
    // Zero out our statistics
//    _triangleCount = 0;

    // Set up the lighting for the scene
    setRenderMode(SmoothShade);
    LightProxySet::iterator lite;
    for (lite = sceneLights.begin(); lite != sceneLights.end(); lite++)
        (*lite)->doIlluminate();
    // Transform to look thru the camera
    cameraProxy->setupView();

    // Go through and update the tessellation/LOD for each object
    Scene::ObjectArray & objects = scene()->objects();
    Transform::Point cam = *camera()->transform->locationPoint();
    for (index_t i = 0; i < objects.size(); i++) {
        SolidObject3D &obj = (SolidObject3D &) *objects[i];
        Transform::Vector look = *obj.transform->locationPoint() - cam;
        obj.updateTessellation(cam, look);
    }
#endif
#if 0
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 1.0, 1.0);
    glutSolidSphere(100.0, 10, 10);

    // Now render the scene in several passes, so everything gets rendered
    // right: first, opaque objects, then wireframe, last transparent objects
    for (unsigned int p = 0; p <= LAST_GL_RENDER_PASS; p++) {
        RenderPass pass = static_cast<RenderPass>(p);

        // Set up lighting, depth buffering, etc.
        initializeRenderPass(pass);

        // Draw scene objects
        ObjectProxySet::iterator i;
        for (i = objectsForPass[pass].begin();
             i != objectsForPass[pass].end(); i++) {

            ObjectProxyPtr obj = *i;
            pushTransform(obj->object()->transform);
            _triangleCount += obj->doRender(pass);
            popTransform();
        }
    }
#endif
    };
};

