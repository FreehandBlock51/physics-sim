#pragma once

/**
 * Utilities for transformation matricies
 */

#include "common/defines.h"
#include "common/vec3.h"
#include "common/vec4.h"
#include "common/mat4x4.h"

/**
 * Stores information about an object's transformations that can be
 * converted into a transformation matrix
 */
struct Transform {
    vec3_t position;
    quaternion_t rotation;
    vec3_t scale;
};
typedef struct Transform transform_t;

/**
 * Initializes the given transform
 */
void transform_make(
        transform_t *transform,
        vec3_t position,
        quaternion_t rotation,
        vec3_t scale);

/**
 * Initializes a blank transform
 * (position and rotation are 0; scale is 1)
 */
#define transform_default(transform) \
transform_make(transform, VEC3_ZERO, quaternion_make(1, 0, 0, 0), VEC3_ONE)

/**
 * Translates a transform by a given vector
 */
void transform_translate(transform_t *transform, vec3_t translation);

/**
 * Sets a transform's position
 */
void transform_set_position(transform_t *transform, vec3_t position);

/**
 * rotates a transform around a given axis.
 * (rotation is given in radians)
 */
void transform_rotate(transform_t *transform, vec3_t axis, phy_real_t rotation);

/**
 * Sets a transform's rotation
 */
void transform_set_rotation(transform_t *transform, quaternion_t rotation);

/**
 * scales a transform by a given vector
 */
void transform_scale(transform_t *transform, vec3_t scale);

/**
 * Sets a transform's scale
 */
void transform_set_scale(transform_t *transform, vec3_t scale);

/**
 * Generates a transformation matrix
 */
void transform_gen_matrix(transform_t transform, mat4x4_t dest);
