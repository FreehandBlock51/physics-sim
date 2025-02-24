#pragma once
/**
 * Definitions and utility functions for 4-dimensional vectors and
 * quaternions
 */

#include <math.h>
#include "common/defines.h"
#include "common/vec3.h"

union Vec4 {
    phy_real_t raw[4];
    struct {
        phy_real_t x;
        phy_real_t y;
        phy_real_t z;
        phy_real_t w;
    };
};

typedef union Vec4 vec4_t;
#define vec4_make(x, y, z, w) ((vec4_t){ { x, y, z, w } })
#define VEC4_ZERO vec4_make(0, 0, 0, 0)

#define vec4_to_cglm(vec) (vec.raw)

typedef union Vec4 quaternion_t;
#define quaternion_make(x, y, z, angle) ((quaternion_t){ { \
    x*sin(angle/2.0),                                      \
    y*sin(angle/2.0),                                      \
    z*sin(angle/2.0),                                      \
    cos(angle/2.0)                                         \
} })

/**
 * Adds a source vector (multiplied by a factor) into a destination
 * vector
 */
void vec4_add_to(vec4_t *dest, vec4_t source, phy_real_t factor);

/**
 * Multiplies a vector by a scalar factor
 */
void vec4_multiply_by(vec4_t *vec, phy_real_t factor);

/**
 * Resets a vector to (0,0,0,0)
 */
void vec4_clear(vec4_t *vec);

/**
 * Calculates the distance between two vectors
 */
phy_real_t vec4_distance_to(vec4_t from, vec4_t to);

/**
 * Calculates the square of a vector's magnitude (x^2 + y^2 + z^2 + w^2)
 */
phy_real_t vec4_magnitude_sqr(vec4_t vec);

/**
 * Calculates the square of a quaternion's magnitude
 * (x^2 + y^2 + z^2 + w^2)
 */
#define quaternion_magnitude_sqr(q) vec4_magnitude_sqr(q)

/**
 * Calculates a vector's magnitude (sqrt(x^2 + y^2 + z^2 + w^2))
 */
phy_real_t vec4_magnitude(vec4_t vec);

/**
 * Calculates a quaternion's magnitude (sqrt(x^2 + y^2 + z^2 + w^2))
 */
#define quaternion_magnitude(q) vec4_magnitude(q)

/**
 * Converts a vector into a unit vector, which has a magnitude of 1,
 * with the same direction as the original vector
 */
void vec4_unit(vec4_t *vec);

/**
 * Calculates the cross product of a and b (a x b) and stores the result
 * in the destination vector
 */
void vec4_cross_product(vec4_t *dest, vec4_t a, vec4_t b);

/**
 * Calculates the dot product of a and b (a * b)
 */
phy_real_t vec4_dot_product(vec4_t a, vec4_t b);

/**
 * Calculates the product of a and b (a x b) and stores the result
 * in the destination vector
 */
#define quaternion_product(dest, a, b) vec4_cross_product(dest, a, b)

/**
 * Calculates the complex conjugate of a quaternion.
 * A quaternion q has a conjugate q' such that q*q'=1
 */
void quaternion_conjugate(quaternion_t *q);

/**
 * Rotates a vector using a quaternion.
 * This variant adhieres closest to the mathmatical definition,
 * but is slower.
 */
void vec3_rotate_by_quaternion_pure(vec3_t *dest, vec3_t vec, quaternion_t q);

/**
 * Rotates a vector by a quaternion.
 * This variant uses math tricks to be faster.
 */
void vec3_rotate_by_quaternion_fast(vec3_t *dest, vec3_t source, quaternion_t q);

/**
 * Rotates a vector by a quaternion
 */
#define vec3_rotate_by_quaternion(dest, source, q) vec3_rotate_by_quaternion_fast(dest, source, q)
