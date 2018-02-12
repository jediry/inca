/** -*- C++ -*-
 *
 * \file    LightingWidget.cpp
 *
 * \author  Ryan L. Saunders
 *
 * Copyright 2005, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the scene lighting control declared in
 *      LightingWidget.hpp.
 */

// Include precompiled header
#include <inca/precomp.h>

// Import class definition
#include "LightingWidget.hpp"
using namespace inca;
using namespace inca::ui;
using namespace inca::rendering;


// Lighting defaults
#define DEFAULT_AMBIENT                 Color(0.1f, 0.1f, 0.1f, 1.0f)
#define DEFAULT_DIFFUSE                 Color(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_SPECULAR                Color(0.5f, 0.5f, 0.5f, 1.0f)
#define DEFAULT_POSITION                Point(0.0f, 0.0f, 100.0f)
#define LIGHT_INCREMENT                 Color(0.1f, 0.1f, 0.1f, 1.0f)

#define DEFAULT_LIGHT_VECTOR_LENGTH     scalar_t(10)
#define DEFAULT_VECTOR_ORIGIN           Point(0.0f)
#define LIGHT_VECTOR_START_COLOR        Color(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_VECTOR_END_COLOR          Color(0.7f, 0.7f, 0.7f, 0.0f)

// Convenience typedefs
typedef LightingWidget::scalar_t    scalar_t;
typedef LightingWidget::Color       Color;
typedef LightingWidget::Point       Point;


/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
// Constructor with optional component name
LightingWidget::LightingWidget(const std::string & nm)
        : PassThruWidget(nm), _lightingEnabled(true),
          _drawingLightVectors(true), _vectorOrigin(DEFAULT_VECTOR_ORIGIN),
          _lightVectorLength(DEFAULT_LIGHT_VECTOR_LENGTH) {
    // Initialize all of the lights to the "off" state
    for (IndexType i = 0; i < lightCount(); ++i) {
        setLightEnabled(i, false);
        setAmbientColor(i, DEFAULT_AMBIENT);
        setDiffuseColor(i, DEFAULT_DIFFUSE);
        setSpecularColor(i, DEFAULT_SPECULAR);
        setPosition(i, DEFAULT_POSITION);
    }
}


/*---------------------------------------------------------------------------*
 | Lighting controls
 *---------------------------------------------------------------------------*/
bool LightingWidget::lightingEnabled() const {
    return _lightingEnabled;
}
void LightingWidget::setLightingEnabled(bool enabled) {
    _lightingEnabled = enabled;
}
void LightingWidget::toggleLightingEnabled() {
    _lightingEnabled = ! _lightingEnabled;
}

SizeType LightingWidget::lightCount() const {
    return LIGHT_COUNT;
}

IndexType LightingWidget::selectedLightIndex() const {
    return _selectedLightIndex;
}
void LightingWidget::setSelectedLightIndex(IndexType index) {
    _selectedLightIndex = index;
}

bool LightingWidget::lightEnabled(IndexType index) const {
    return _enabled[index];
}
void LightingWidget::setLightEnabled(IndexType index, bool enabled) {
    _enabled[index] = enabled;
}
void LightingWidget::toggleLightEnabled(IndexType index) {
    _enabled[index] = ! _enabled[index];
}


const Color & LightingWidget::ambientColor(IndexType index) const {
    return _ambientColor[index];
}
void LightingWidget::setAmbientColor(IndexType index, const Color & c) {
    _ambientColor[index] = c;
}
const Color & LightingWidget::diffuseColor(IndexType index) const {
    return _diffuseColor[index];
}
void LightingWidget::setDiffuseColor(IndexType index, const Color & c) {
    _diffuseColor[index] = c;
}
const Color & LightingWidget::specularColor(IndexType index) const {
    return _specularColor[index];
}
void LightingWidget::setSpecularColor(IndexType index, const Color & c) {
    _specularColor[index] = c;
}

const Point & LightingWidget::position(IndexType index) const {
    return _position[index];
}
void LightingWidget::setPosition(IndexType index, const Point & p) {
    _position[index] = p;
}

bool LightingWidget::drawingLightVectors() const {
    return _drawingLightVectors;
}
void LightingWidget::setDrawingLightVectors(bool draw) {
    _drawingLightVectors = draw;
}
scalar_t LightingWidget::lightVectorLength() const {
    return _lightVectorLength;
}
void LightingWidget::setLightVectorLength(scalar_arg_t length) {
    _lightVectorLength = length;
}


/*---------------------------------------------------------------------------*
 | Rendering & event-handling functions
 *---------------------------------------------------------------------------*/
void LightingWidget::render() const {
    Renderer & r = renderer();
    Renderer::Rasterizer & rasterizer = r.rasterizer();

    // Enable lighting and shading if needed
    rasterizer.setLightingEnabled(lightingEnabled());
    if (lightingEnabled()) {
        for (IndexType i = 0; i < lightCount(); ++i) {
            Renderer::LightingUnit & light = r.lightingUnit(i);
            if (lightEnabled(i)) {
                light.setPosition(position(i));
                light.setAmbientColor(ambientColor(i));
                light.setDiffuseColor(diffuseColor(i));
                light.setSpecularColor(specularColor(i));
            }
            light.setEnabled(lightEnabled(i));
        }
    }

    // Cascade to the next Widget
    PassThruWidget::render();
    
    // Turn lighting back off
    rasterizer.setLightingEnabled(false);

    // Draw to-light vectors if requested
    if (drawingLightVectors()) {
        bool blend = rasterizer.isAlphaBlendingEnabled();
        bool smooth = rasterizer.isLineSmoothingEnabled();
        rasterizer.setAlphaBlendingEnabled(true);
        rasterizer.setLineSmoothingEnabled(true);
        rasterizer.beginPrimitive(Lines);
            for (IndexType i = 0; i < lightCount(); ++i)
                if (lightEnabled(i)) {
                    Vector toLight = math::normalize(position(i) - _vectorOrigin);
                    rasterizer.setCurrentColor(LIGHT_VECTOR_START_COLOR);
                    rasterizer.vertexAt(_vectorOrigin);
                    rasterizer.setCurrentColor(LIGHT_VECTOR_END_COLOR);
                    rasterizer.vertexAt(_vectorOrigin
                                        + toLight * lightVectorLength());
                }    
        rasterizer.endPrimitive();
        rasterizer.setAlphaBlendingEnabled(blend);
        rasterizer.setLineSmoothingEnabled(smooth);
    }
}

void LightingWidget::keyPressed(const KeyEvent & e) {
    switch (e.keyCode()) {
    case Key1: case Key2: case Key3: case Key4:
    case Key5: case Key6: case Key7: case Key8:
        setSelectedLightIndex(e.keyCode() - Key1);
        break;
    case KeyBackwardSlash:
        toggleLightEnabled(selectedLightIndex());
        requestRedisplay();
        break;
    case KeyLeftBracket:
        setSpecularColor(selectedLightIndex(),
                        specularColor(selectedLightIndex()) - LIGHT_INCREMENT);
        requestRedisplay();
        break;
    case KeyRightBracket:
        setSpecularColor(selectedLightIndex(),
                        specularColor(selectedLightIndex()) + LIGHT_INCREMENT);
        requestRedisplay();
        break;
    case KeyColon:
        setDiffuseColor(selectedLightIndex(),
                        diffuseColor(selectedLightIndex()) - LIGHT_INCREMENT);
        requestRedisplay();
        break;
    case KeyQuote:
        setDiffuseColor(selectedLightIndex(),
                        diffuseColor(selectedLightIndex()) + LIGHT_INCREMENT);
        requestRedisplay();
        break;
    case KeyLessThan:
        setAmbientColor(selectedLightIndex(),
                        ambientColor(selectedLightIndex()) - LIGHT_INCREMENT);
        requestRedisplay();
        break;
    case KeyGreaterThan:
        setAmbientColor(selectedLightIndex(),
                        ambientColor(selectedLightIndex()) + LIGHT_INCREMENT);
        requestRedisplay();
        break;
    default:
        PassThruWidget::keyPressed(e);
        break;
    }
}
