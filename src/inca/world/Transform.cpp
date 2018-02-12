/*
 * File: Transform.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file contains definitions of the Transform class's methods,
 *      declared in Transform.hpp.
 */

// Include precompiled header
#include <inca/precomp.h>

// Include class definition
#include "Transform.hpp"
using namespace inca::world;
using namespace inca::math;


/*---------------------------------------------------------------------------*
 | Constructors
 *---------------------------------------------------------------------------*/
// Default constructor
Transform::Transform() : basisInvalid(true),
    position(this), worldUp(this), rotation(this), scaling(this) { }


/*---------------------------------------------------------------------------*
 |
 *---------------------------------------------------------------------------*/
void Transform::lookAt(const Point &p, const Vector &up) {
    // Make our basis match this
    Vector vFront = p - position();
    Vector vRight = vFront % up;
    buildBasis(vFront, vRight);

    // Now fix our quaternion to match
    Quaternion q;
    rotation = loadRotation3D(q, basisFront, basisUp, basisRight);

    // Oh NO you don't...I just fixed that!
    basisInvalid = false;
}


/*---------------------------------------------------------------------------*
 | Transformations relative to the current state
 *---------------------------------------------------------------------------*/
// Axis-aligned transformations
void Transform::translateX(scalar_arg_t dx) {
    translate(Vector(dx, 0.0, 0.0));
}
void Transform::translateY(scalar_arg_t dy) {
    translate(Vector(0.0, dy, 0.0));
}
void Transform::translateZ(scalar_arg_t dz) {
    translate(Vector(0.0, 0.0, dz));
}
void Transform::rotateX(scalar_arg_t angle) {
    rotate(angle, Vector(1.0, 0.0, 0.0));
}
void Transform::rotateY(scalar_arg_t angle) {
    rotate(angle, Vector(0.0, 1.0, 0.0));
}
void Transform::rotateZ(scalar_arg_t angle) {
    rotate(angle, Vector(0.0, 0.0, 1.0));
}
void Transform::scaleX(scalar_arg_t sx) {
    scale(Vector(sx, 1.0, 1.0));
}
void Transform::scaleY(scalar_arg_t sy) {
    scale(Vector(1.0, sy, 1.0));
}
void Transform::scaleZ(scalar_arg_t sz) {
    scale(Vector(1.0, 1.0, sz));
}

// Standard transforms
void Transform::translate(const Vector &d) {
    position = position() + d;
}

void Transform::rotate(const Quaternion &q) {
    rotation = q % rotation();
}

void Transform::scale(scalar_arg_t s) {
    scaling = scaling() * s;
}

void Transform::scale(const Vector &s) {
   scaling = scaling() * s;
}

void Transform::rotate(scalar_arg_t angle, const Vector &around) {
    scalar_t a_2 = angle / 2.0,
             cos_a_2 = cos(a_2),
             sin_a_2 = sin(a_2);
    rotate(Quaternion(cos_a_2, sin_a_2 * normalize(around)));
}

void Transform::orbit(scalar_arg_t angle,
                      const Vector & axis,
                      const Point & center) {
    rotate(angle, axis);
    position = math::rotate(position(), angle, axis, center);
}


/*---------------------------------------------------------------------------*
 | Current frame-of-reference controls
 *---------------------------------------------------------------------------*/
void Transform::moveLongitudinally(scalar_arg_t dist) {
    translate(dist * front());
}
void Transform::moveLaterally(scalar_arg_t dist) {
    translate(dist * right());
}
void Transform::moveVertically(scalar_arg_t dist) {
    translate(dist * up());
}
void Transform::look(scalar_arg_t hAngle, scalar_arg_t vAngle) {
    pitch(vAngle);
    yaw(hAngle);
}
void Transform::pitch(scalar_arg_t angle) {
    rotate(angle, right());
}
void Transform::yaw(scalar_arg_t angle) {
    rotate(angle, up());
}
void Transform::roll(scalar_arg_t angle) {
    rotate(angle, front());
}

void Transform::pan(scalar_arg_t hDist, scalar_arg_t vDist) {
    translate(hDist * right() + vDist * up());
}
void Transform::dolly(scalar_arg_t fDist) {
    translate(fDist * front());
}


/*---------------------------------------------------------------------------*
 | Orthonormal basis vectors (derived from primary controls above)
 *---------------------------------------------------------------------------*/
// Make basis from vectors. Ugh like vectors.
void Transform::constructBasis(const Vector & vFront, const Vector & vRight) {
    buildBasis(vFront, vRight);
    recalculateQuaternion();
}


// Orthonormal basis vectors
const Transform::Vector & Transform::up() const {
    if (basisInvalid)   recalculateBasis();
    return basisUp;
}
const Transform::Vector & Transform::down() const {
    if (basisInvalid)   recalculateBasis();
    return basisDown;
}
const Transform::Vector & Transform::left() const {
    if (basisInvalid)   recalculateBasis();
    return basisLeft;
}
const Transform::Vector & Transform::right() const {
    if (basisInvalid)   recalculateBasis();
    return basisRight;
}
const Transform::Vector & Transform::back() const {
    if (basisInvalid)   recalculateBasis();
    return basisBack;
}
const Transform::Vector & Transform::front() const {
    if (basisInvalid)   recalculateBasis();
    return basisFront;
}


/*---------------------------------------------------------------------------*
 | Conversion between quaternion and basis vector representations
 *---------------------------------------------------------------------------*/
// Find the quaternion that corresponds to the current basis vectors
void Transform::recalculateQuaternion() {
    loadRotation3D(_rotation, basisFront, basisUp, basisRight);
}

// Regenerate the basis vectors from the quaternion
void Transform::recalculateBasis() const {
    // First, clean up our quaternion
    Quaternion q = rotation;
    Quaternion qInverse = inverse(q);

    // Now, rotate the front and right vectors using these
    Vector vFront = Vector(q % Quaternion(Vector(0.0, 0.0, -1.0)) % qInverse);
    Vector vRight = Vector(q % Quaternion(Vector(1.0, 0.0, 0.0)) % qInverse);

    // Make a nice, pretty basis outta this
    buildBasis(vFront, vRight);
}

// Construct an orthonormal vector basis from a front/right vector pair
void Transform::buildBasis(const Vector &vFront, const Vector &vRight) const {
    // These need to be unit vectors
    basisFront = normalize(vFront);
    basisRight = normalize(vRight);
    basisUp    = basisRight % basisFront;   // Up is right x front

    // These are just reflections
    basisBack = -basisFront;
    basisLeft = -basisRight;
    basisDown = -basisUp;

    basisInvalid = false;       // OK. Now we're done
}

// Stuff that dates back to the loc/lookat/up days
#if 0
void Transform::orbit(scalar_arg_t hAngle, scalar_arg_t vAngle) {
    const Point &locPt = *locationPoint(),
                &lookAtPt = *lookAtPoint();

    // Find what the new look-along vector should be
    Vector newLook = cos(hAngle) * (cos(vAngle) * front() + sin(vAngle) * up())
                     + sin(hAngle) * right();
    newLook.normalize();

    // Put the new location along this vector, the same distance away as before
    *locationPoint() = lookAtPt - newLook * magnitude(lookAtPt - locPt);

    // This operation changed our basis
    basisInvalid = true;
}
void Transform::roll(scalar_arg_t angle) {
    // Figure out what the new up vector is
    *worldUpVector() = cos(angle) * up() + sin(angle) * right();

    // This operation changed our basis
    basisInvalid = true;
}

void Transform::pan(scalar_arg_t hDist, scalar_arg_t vDist) {
    // Move orthogonally to our view by the requested amounts
    Vector displacement = hDist * right() + vDist * up();
    *locationPoint() += displacement;
    *lookAtPoint() += displacement;
}
void Transform::roll(scalar_arg_t angle) {
    // Figure out what the new up vector is
    *worldUpVector() = cos(angle) * up() + sin(angle) * right();

    // This operation changed our basis
    basisInvalid = true;
}
void Transform::recalculateBasis() const {
    // First, calculate the forward normal
    basisFront = *lookAtPoint() - *locationPoint();

    // Figure out the right direction from world up and lookAt
    basisRight = basisFront % *worldUpVector();

    // Up normal is right x front
    basisUp = basisRight % basisFront;

    // These need to be unit vectors
    basisFront.normalize();
    basisUp.normalize();
    basisRight.normalize();

    // These are just reflections
    basisBack = -basisFront;
    basisLeft = -basisRight;
    basisDown = -basisUp;

    basisInvalid = false;       // OK. Now we're done
}
#endif
