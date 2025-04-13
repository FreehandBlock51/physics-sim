#pragma once

/**
 * Utilities for calculating constraints on rigidbodies
 */

#include "sim/body.h"

/**
 * Represents a spring connecting two rigidbodies
 */
struct SpringConstraint {
    body_t *a;
    vec3_t a_endpoint;
    body_t *b;
    vec3_t b_endpoint;
    phy_real_t spring_constant;
    phy_real_t equilibrium_distance;
};
typedef struct SpringConstraint spring_t;

/**
 * Creates a spring constraint that will apply a force proportional to
 * a given spring constant on two given bodies, with each force being
 * applied at the given endpoints.
 * Both endpoints are relative to the positions of their respective
 * rigidbodies.
 */
void spring_create(spring_t *spring, body_t *a, vec3_t a_endpoint, body_t *b, vec3_t b_endpoint, phy_real_t spring_constant, phy_real_t equilibrium_distance);

/**
 * Applies the spring force & torque to the attached rigidbodies
 */
void spring_apply_constraint(spring_t spring);
