/*
 * File: Transform.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The Transform class encapsulates an object's relationship
 *      to the rest of a scene. An object can be translated, rotated and
 *      scaled, simply by having its Transform object modified.
 *
 *      A Transform may be used to construct a local orthonormal coordinate
 *      basis, through the up(), down(), left(), right(), front(), back()
 *      functions. These basis vectors cannot be directly set, but are
 *      lazily evaluated from the user-level rotation controls. The default
 *      orientation of a Transform is right() = +X, up() = +Y, front() = -Z.
 *
 *      A Transform is composed of a Point (representing its location),
 *      a quaternion (representing the rotations away from the world
 *      coordinate axes), and a Vector (representing scaling factors
 *      along the world axes (before rotation and translation are applied).
 */

#ifndef INCA_WORLD_TRANSFORM
#define INCA_WORLD_TRANSFORM

// Import system configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class Transform;

        // Pointer typedefs
        typedef shared_ptr<Transform> TransformPtr;
    };
};

// Import Inca::Math library
#include <inca/math.hpp>

// Import the superclass definition
#include "WorldObject.hpp"


class inca::world::Transform : public WorldObject {
private:
    // Convenience typedefs
    typedef Transform ThisType;

public:
    // Import typedefs for the scalar model we're using for geometry
    INCA_MATH_SCALAR_TYPEDEFS(double, IS_NOT_WITHIN_TEMPLATE, /* */, /* */);

    // Import typedefs for Point, Vector, etc.
    INCA_MATH_VECTOR_TYPEDEFS(scalar_t, 3, /* */, /* */);


/*---------------------------------------------------------------------------*
 | Constructor
 *---------------------------------------------------------------------------*/
public:
    // Default initializer
    Transform();


/*---------------------------------------------------------------------------*
 | High-level controls
 *---------------------------------------------------------------------------*/
public:
    // The origin of our local coordinate system
    rw_property(Point, position, Point(0, 0, 0));

    // What direction is "up"
    rw_property_cm(Vector, worldUp, Vector(0, 1, 0));
        property_mutator(Vector, worldUp) {
            _worldUp = value;
            basisInvalid = true;
        }

    // The absolute rotation of our system w/r/t the world coordinate axes
    rw_property_cm(Quaternion, rotation, Quaternion(1.0, 0.0, 0.0, 0.0));
        property_mutator(Quaternion, rotation) {
            _rotation = value;
            basisInvalid = true;
        }

    // The ratio of our coordinate scale w/r/t the world coordinate system
    rw_property(Vector, scaling, Vector(1.0, 1.0, 1.0));


    // Look from the current position towards 'p' with up being close to 'up'
    void lookAt(const Point &p, const Vector &up);


/*---------------------------------------------------------------------------*
 | Transformations relative to the current state
 *---------------------------------------------------------------------------*/
public:
    // World-coordinate relative controls
    void translate(const Vector &v);
    void scale(scalar_arg_t s);                         // Scale uniformly
    void scale(const Vector &s);                        // Scale non-uniformly
    void rotate(const Quaternion &q);
    void rotate(scalar_arg_t angle, const Vector &v);   // Rotate about a vector
    void rotateX(scalar_arg_t angle);                   // Rotate around X
    void rotateY(scalar_arg_t angle);                   // Rotate around Y
    void rotateZ(scalar_arg_t angle);                   // Rotate around Z


    // Local-coordinate relative controls
    void look(scalar_arg_t hAngle, scalar_arg_t vAngle);
    void yaw(scalar_arg_t angle);
    void pitch(scalar_arg_t angle);
    void roll(scalar_arg_t angle);
    void pan(scalar_arg_t hDist, scalar_arg_t vDist);
    void dolly(scalar_arg_t fDist);


/*---------------------------------------------------------------------------*
 | Orthonormal basis vectors (derived from primary controls above)
 *---------------------------------------------------------------------------*/
 public:
    void recalculateBasis() const;  // Rebuild Vs from Q
    void recalculateQuaternion();   // Make our Q resemble our Vs
    void buildBasis(const Vector &vFront, const Vector &vRight) const;
    const Vector & up() const;
    const Vector & down() const;
    const Vector & left() const;
    const Vector & right() const;
    const Vector & front() const;
    const Vector & back() const;

protected:
    // Orthonormal basis vectors (not directly modifiable)
    mutable bool basisInvalid;
    mutable Vector basisUp, basisDown, basisLeft,
                   basisRight, basisBack, basisFront;
};

#endif
