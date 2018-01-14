/** -*- C++ -*-
 *
 * \file    LightingWidget.hpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The LightingWidget class is a PassThruWidget subclass that sets up the
 *      lighting environment, then renders any widgets chained after it. This
 *      allows the rendering of an object to be abstracted from the lighting
 *      conditions under which it is rendered. It provides functions for
 *      modifying dynamically the lighting parameters, and also contains
 *      key/mouse bindings for calling these functions.
 *
 * HACK: This is highly preliminary. It could be done better, and more efficiently.
 *      but it's kinda waiting improved Renderer support.
 * TODO: This would be cleaner with Inca Light objects.
 */

#pragma once
#ifndef INCA_UI_LIGHTING_WIDGET
#define INCA_UI_LIGHTING_WIDGET

// Import system configuration
#include <inca/inca-common.h>


// This is part of the Inca user-interface layer
namespace inca {
    namespace ui {
        // Forward declarations
        class LightingWidget;

        // Pointer typedefs
        typedef shared_ptr<LightingWidget>       LightingWidgetPtr;
        typedef shared_ptr<LightingWidget const> LightingWidgetConstPtr;
    };
};

// Import superclass definition
#include "PassThruWidget.hpp"

// Import math definitions
#include <inca/math.hpp>


class inca::ui::LightingWidget : public inca::ui::PassThruWidget {
/*---------------------------------------------------------------------------*
 | Type declarations
 *---------------------------------------------------------------------------*/
private:
    // Set up this class to own properties
    PROPERTY_OWNING_OBJECT(LightingWidget);

public:
    // Lighting properties typedefs
    INCA_MATH_SCALAR_TYPES(float, IS_NOT_WITHIN_TEMPLATE);
    INCA_MATH_COLOR_TYPES(scalar_t, sRGB, true);
    INCA_MATH_LINALG_TYPES(scalar_t, 3);
    

/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
public:
    // Constructor with optional component name
    explicit LightingWidget(const std::string & nm = std::string());


/*---------------------------------------------------------------------------*
 | Lighting controls
 *---------------------------------------------------------------------------*/
public:
    bool lightingEnabled() const;
    void setLightingEnabled(bool enabled);
    void toggleLightingEnabled();

    SizeType lightCount() const;    // How many lights are available

    IndexType selectedLightIndex() const;
    void setSelectedLightIndex(IndexType index);

    bool lightEnabled(IndexType index) const;
    void setLightEnabled(IndexType index, bool enabled);
    void toggleLightEnabled(IndexType index);
    
    const Color & ambientColor(IndexType index) const;
    void setAmbientColor(IndexType index, const Color & c);
    const Color & diffuseColor(IndexType index) const;
    void setDiffuseColor(IndexType index, const Color & c);
    const Color & specularColor(IndexType index) const;
    void setSpecularColor(IndexType index, const Color & c);
    const Point & position(IndexType index) const;
    void setPosition(IndexType index, const Point & p);
    
    bool drawingLightVectors() const;
    void setDrawingLightVectors(bool draw);
    scalar_t lightVectorLength() const;
    void setLightVectorLength(scalar_arg_t length);

protected:
    bool _lightingEnabled;
    
    IndexType _selectedLightIndex;
    
    static const int LIGHT_COUNT = 8;
    bool _enabled[LIGHT_COUNT];
    Color _ambientColor[LIGHT_COUNT];
    Color _diffuseColor[LIGHT_COUNT];
    Color _specularColor[LIGHT_COUNT];
    Point _position[LIGHT_COUNT];
    
    bool _drawingLightVectors;
    scalar_t _lightVectorLength;
    Point    _vectorOrigin;
    

/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
public:
    // Set up the lighting environment
    void render() const;
    
    // Key events
    void keyPressed(const KeyEvent & e);
};

#endif
