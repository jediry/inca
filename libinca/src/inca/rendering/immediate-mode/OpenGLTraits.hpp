/*
 * File: OpenGLTraits.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file serves as a convenience include file for including all of the
 *      rendering classes and functions.
 */

#ifndef INCA_RENDERING_IMMEDIATE_MODE_OPENGL_TRAITS
#define INCA_RENDERING_IMMEDIATE_MODE_OPENGL_TRAITS

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca rendering subsystem
namespace inca {
    namespace rendering {
        // Forward declarations
        class OpenGLTraits;
    };
};

// Import Inca scalar, color, and linear-algebra types
#include <inca/math/scalar.hpp>
#include <inca/math/color.hpp>
#include <inca/math/linalg.hpp>
#include <inca/util/Region>

// Import rendering enumerated types
#include "types.hpp"


class inca::rendering::OpenGLTraits {
public:
    typedef float geometry_t;   // Geometry/matrix precision
    typedef float color_t;      // Color component precision
    typedef float tex_coord_t;  // Texture coordinate precision
    typedef float timer_t;      // Timer precision

    typedef math::Point<geometry_t, 2>                      Point2D;
    typedef math::Vector<geometry_t, 2>                     Vector2D;
    typedef math::Point<geometry_t, 3>                      Point3D;
    typedef math::Vector<geometry_t, 3>                     Vector3D;
    typedef math::Point<geometry_t, 4>                      Point4D;
    typedef math::Vector<geometry_t, 4>                     Vector4D;
    typedef math::Quaternion<geometry_t>                    Quaternion;
    typedef math::Matrix<geometry_t, 4, 4, true, false>     Matrix;
    typedef math::Color<color_t, math::sRGB<true> >         Color;
    typedef math::Vector<geometry_t, 3>                     Normal;
    typedef math::Point<geometry_t, 2>                      TexCoord;
    typedef Region<2>                                       Region;
    typedef math::Point<int, 2>                             Pixel;
    typedef math::Vector<int, 2>                            Dimension;

//protected:
    // Grant the ImmediateModeRenderer access to the hardware functions
    template <class API, template <inca::SizeType> class CP>
        friend class ImmediateModeRenderer;


    // Properties and functions for the IMR::Rasterizer object
    enum Property {
        // Renderer properties
        CurrentMatrixStack, CurrentTexturingUnit,

        // Rasterizer properties
        DepthBuffering, AlphaBlending, FaceCulling, Lighting,
        PointSmoothing, LineSmoothing, PolygonSmoothing, Fog,
        BackgroundColor, PolygonOffset, PointDiameter, LineWidth,
        ShadingModel, CurrentColor, CurrentNormal, CurrentTexCoord, CurrentEdgeFlag,
        FogModel, FogStartDepth, FogEndDepth, FogDensity, FogColor,
        CulledFace, TextureType,
        VertexArrayPointer,   VertexArrayStride,
        NormalArrayPointer,   NormalArrayStride,
        TexCoordArrayPointer, TexCoordArrayStride,
        ColorArrayPointer,    ColorArrayStride,
        EdgeFlagArrayPointer, EdgeFlagArrayStride,

        // Matrix stack properties
        CurrentMatrix,
        MatrixStackDepth,

        // Viewport properties
        ViewportBounds,

        // Lighting unit properties
        LightingUnitEnabled,
        LightingUnitPosition,
        LightingUnitAmbientColor,
        LightingUnitDiffuseColor,
        LightingUnitSpecularColor,

        // Texturing unit properties
        TexturingUnitEnabled,
    };
    static const Property FirstRendererProperty = CurrentMatrixStack;
    static const Property LastRendererProperty  = CurrentMatrixStack;
    static const Property FirstRasterizerProperty = DepthBuffering;
    static const Property LastRasterizerProperty  = EdgeFlagArrayStride;
    static const Property FirstMatrixStackProperty = CurrentMatrix;
    static const Property LastMatrixStackProperty  = MatrixStackDepth;
    static const Property FirstViewportProperty = ViewportBounds;
    static const Property LastViewportProperty  = ViewportBounds;
    static const Property FirstLightingUnitProperty = LightingUnitEnabled;
    static const Property LastLightingUnitProperty  = LightingUnitSpecularColor;
    static const Property FirstTexturingUnitProperty = TexturingUnitEnabled;
    static const Property LastTexturingUnitProperty  = TexturingUnitEnabled;

    // Hardware property get/set functions
    template <Property p> static void getImplementationLimit(SizeType & sz);
    template <Property p> static void getImplementationLimit(IDType id, IndexType & idx);
    template <Property p> static void getHardwareState(IDType & id);
    template <Property p> static void setHardwareState(IDType id);
    template <Property p> static void getHardwareState(IDType id, IndexType & idx);
    template <Property p> static void setHardwareState(IDType id, IndexType idx);
    template <Property p> static void getHardwareState(bool & enabled);
    template <Property p> static void setHardwareState(bool enabled);
    template <Property p> static void getHardwareState(geometry_t & value);
    template <Property p> static void setHardwareState(geometry_t value);
    template <Property p> static void getHardwareState(Normal & value);
    template <Property p> static void setHardwareState(const Normal & value);
    template <Property p> static void getHardwareState(Color & value);
    template <Property p> static void setHardwareState(const Color & value);
    template <Property p> static void getHardwareState(TexCoord & value);
    template <Property p> static void setHardwareState(const TexCoord & value);
    template <Property p> static void getHardwareState(::inca::rendering::ShadingModel & model);
    template <Property p> static void setHardwareState(::inca::rendering::ShadingModel model);
    template <Property p> static void getHardwareState(::inca::rendering::FogModel & model);
    template <Property p> static void setHardwareState(::inca::rendering::FogModel model);

    template <Property p> static void getHardwareState(::inca::rendering::PolygonFace & face);
    template <Property p> static void setHardwareState(::inca::rendering::PolygonFace face);
    template <Property p> static void getHardwareState(::inca::rendering::TextureType & type);
    template <Property p> static void setHardwareState(::inca::rendering::TextureType type);

    template <Property p> static void getHardwareState(IDType id, bool & enabled);
    template <Property p> static void setHardwareState(IDType id, bool enabled);

    template <Property p> static void getHardwareState(IDType id, Color & c);
    template <Property p> static void setHardwareState(IDType id, const Color & c);
    template <Property p> static void getHardwareState(IDType id, Point3D & p);
    template <Property p> static void setHardwareState(IDType id, const Point3D & p);

    template <Property p> static void getHardwareState(IDType id, Matrix & m);
    template <Property p> static void setHardwareState(IDType id, const Matrix & m);

    template <Property p> static void getHardwareState(Region & r);
    template <Property p> static void setHardwareState(const Region & r);

    template <Property p> static void getHardwareState(DifferenceType & stride);
    template <Property p> static void getHardwareState(void * & ptr);
    template <Property p, typename T> static void setHardwareState(T const * ptr, DifferenceType stride);

    // Functions used by the IMR object
    static SizeType matrixStackCount();
    static SizeType lightingUnitCount();
    static SizeType texturingUnitCount();

    static IDType matrixStackID(IndexType index);
    static IDType lightingUnitID(IndexType index);
    static IDType texturingUnitID(IndexType index);

    static IndexType projectionMatrixIndex();
    static IndexType viewMatrixIndex();
    static IndexType worldMatrixIndex();
    static IndexType colorMatrixIndex();

    static void clearBuffers(IDType ids);


    // Functions used by the IMR::Rasterizer object
    static void beginPrimitive(PrimitiveType type);
    static void endPrimitive();
    static void renderVertexIndex(IndexType index);
    static void renderArrayRange(PrimitiveType type, IndexType from, SizeType count);
    template <typename V>
    static void renderVertexAt(const V & v);                    // Array vertex
    template <typename S>
    static void renderVertexAt(S s0, S s1);             // 2D vertex
    template <typename S>
    static void renderVertexAt(S s0, S s1, S s2);       // 3D vertex
    template <typename S>
    static void renderVertexAt(S s0, S s1, S s2, S s3); // 4D vertex


    // Functions used by the IMR::MatrixStack object
    static void pushMatrix(IDType id);
    static void popMatrix(IDType id);
    static void resetMatrix(IDType id);
    static void premultiplyMatrix(IDType id, const Matrix & m);
    static void postmultiplyMatrix(IDType id, const Matrix & m);
    static void scaleMatrix(IDType id, geometry_t s);
    static void scaleMatrix(IDType id, const Vector3D & s);
    static void rotateMatrix(IDType id, geometry_t angle, const Vector3D & axis);

    template <typename V>
    static void translateMatrix(IDType id, const V & v);


    // Functions used for IMR::Texture manipulation
    template <typename scalar, typename colorspace, SizeType dim>
    static IDType createTexture(const math::Color<scalar, colorspace> * texels,
                                const inca::Array<SizeType, dim> & sizes);
    static void deleteTexture(IDType textureID);
    static void bindTexture(IDType textureID);
};

#endif
