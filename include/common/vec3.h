#pragma once
/**
 * Definition and functions for three-dimensional vectors
 */

#include "common/defines.h"

union Vec3 {
    phy_real_t raw[3];
    struct {
        phy_real_t x;
        phy_real_t y;
        phy_real_t z;
    };
};
typedef union Vec3 vec3_t;

#define vec3_make(x, y, z) ((vec3_t){ { x, y, z } })
#define VEC3_ZERO   vec3_make( 0,  0,  0)
#define VEC3_ONE    vec3_make( 1,  1,  1)
#define VEC3_UP     vec3_make( 0, +1,  0)
#define VEC3_DOWN   vec3_make( 0, -1,  0)
#define VEC3_LEFT   vec3_make(-1,  0,  0)
#define VEC3_RIGHT  vec3_make(+1,  0,  0)
#define VEC3_FRONT  vec3_make( 0,  0, +1)
#define VEC3_BACK   vec3_make( 0,  0, -1)

#define vec3_to_cglm(vec) (vec.raw)

/**
 * Adds a source vector (multiplied by a factor) into a destination
 * vector
 */
void vec3_add_to(vec3_t *dest, vec3_t source, phy_real_t factor);

/**
 * Multiplies a vector by a scalar factor
 */
void vec3_multiply_by(vec3_t *dest, phy_real_t factor);

/**
 * Resets a vector to (0,0,0)
 */
void vec3_clear(vec3_t *vec);

/**
 * Calculates the distance between two vectors
 */
phy_real_t vec3_distance_to(vec3_t from, vec3_t to);

/**
 * Calculates the square of the distance between two vectors
 */
phy_real_t vec3_distance_sqr(vec3_t from, vec3_t to);

/**
 * Calculates the square of a vector's magnitude (x^2 + y^2 + z^2)
 */
phy_real_t vec3_magnitude_sqr(vec3_t vec);

/**
 * Calculates a vector's magnitude (sqrt(x^2 + y^2 + z^2))
 */
phy_real_t vec3_magnitude(vec3_t vec);

/**
 * Converts a vector into a unit vector, which has a magnitude of 1,
 * with the same direction as the original vector
 */
void vec3_unit(vec3_t *vec);

/**
 * Rotates a vector around the X-axis.
 * The rotation should be given in radians
 */
void vec3_rotate_x(vec3_t *vec, phy_real_t rotation);

/**
 * Rotates a vector around the Y-axis.
 * The rotation should be given in radians
 */
void vec3_rotate_y(vec3_t *vec, phy_real_t rotation);

/**
 * Rotates a vector around the Z-axis.
 * The rotation should be given in radians
 */
void vec3_rotate_z(vec3_t *vec, phy_real_t rotation);

/**
 * Rotates a vector around the X-, Y-, and Z-axes.
 * The rotations should be given in radians
 */
void vec3_rotate(vec3_t *vec, phy_real_t xrot, phy_real_t yrot, phy_real_t zrot);

/**
 * Calculates the cross product of a and b (a x b) and stores the result
 * in the destination vector
 */
void vec3_cross_product(vec3_t *dest, vec3_t a, vec3_t b);

/**
 * Calculates the dot product of a and b (a * b),
 * equivalent to ||a|| * ||b|| * cos(angle between a and b)
 */
phy_real_t vec3_dot_product(vec3_t a, vec3_t b);

/**
 * Gets the portion of a given vector
 * 'in the same direction' as another
 * vector.  For example, if the original is (1,2,4)
 * and the direction is (0,1,0), the result would
 * be (0,2,0)
 */
void vec3_get_portion_in_direction(vec3_t *result, vec3_t original, vec3_t direction);

#ifdef _STDIO_H

/**
 * Prints a prefix string, followed by a string representation of a
 * given vector, followed by a postfix string
 */
#define print_vec3(pre, vec, post) printf(pre "(%f, %f, %f)" post, vec.x, vec.y, vec.z)

#endif
