#include "common/vec3.h"

#include <assert.h>
#include <stddef.h>
#include <math.h>

void vec3_add_to(vec3_t *dest, vec3_t source, phy_real_t factor) {
    assert(dest != NULL); // so that nullptr crashes in debug
    if (dest == NULL) {
        return; // so that nullptr is ignored in release
    }
    dest->x += source.x * factor;
    dest->y += source.y * factor;
    dest->z += source.z * factor;
}

void vec3_multiply_by(vec3_t *dest, phy_real_t factor) {
    assert(dest != NULL);
    if (dest == NULL) {
        return;
    }
    dest->x *= factor;
    dest->y *= factor;
    dest->z *= factor;
}

void vec3_clear(vec3_t *vec) {
    assert(vec != NULL);
    if (vec == NULL) {
        return;
    }
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
}

phy_real_t vec3_distance_to(vec3_t from, vec3_t to) {
    phy_real_t dx = to.x - from.x;
    phy_real_t dy = to.y - from.y;
    phy_real_t dz = to.z - from.z;
    return sqrt(
        (dx * dx) +
        (dy * dy) +
        (dz * dz)
    );
}

phy_real_t vec3_distance_sqr(vec3_t from, vec3_t to) {
    phy_real_t dx = to.x - from.x;
    phy_real_t dy = to.y - from.y;
    phy_real_t dz = to.z - from.z;
    return
        (dx * dx) +
        (dy * dy) +
        (dz * dz);
}

phy_real_t vec3_magnitude_sqr(vec3_t vec) {
    return
        (vec.x * vec.x) +
        (vec.y * vec.y) +
        (vec.z * vec.z);
}

phy_real_t vec3_magnitude(vec3_t vec) {
    return sqrt(
        (vec.x * vec.x) +
        (vec.y * vec.y) +
        (vec.z * vec.z)
    );
}

void vec3_unit(vec3_t *vec) {
    assert(vec != NULL);
    if (vec == NULL) {
        return;
    }
    phy_real_t magnitude = vec3_magnitude(*vec);
    if (magnitude != 0) {
        // ensure that we aren't dividing by zero --
        // we don't want the unit vector to be full of NaNs
        vec3_multiply_by(vec, 1.0 / magnitude);
    }
}

void vec3_rotate_x(vec3_t *vec, phy_real_t xrot) {
    assert(vec != NULL);
    if (vec == NULL) {
        return;
    }
    vec3_t copy = *vec;
    vec->x = copy.x * 1 + copy.y * 0 + copy.z * 0;
    vec->y = copy.x * 0 + copy.y * cos(xrot) + copy.z * sin(xrot);
    vec->z = copy.x * 0 - copy.y * sin(xrot) + copy.z * cos(xrot);
}

void vec3_rotate_y(vec3_t *vec, phy_real_t yrot) {
    vec3_t copy = *vec;
    vec->x = copy.x * cos(yrot) + copy.y * 0 - copy.z * sin(yrot);
    vec->y = copy.x * 0 + copy.y * 1 + copy.z * 0;
    vec->z = copy.x * sin(yrot) + copy.y * 0 + copy.z * cos(yrot);
}

void vec3_rotate_z(vec3_t *vec, phy_real_t zrot) {
    assert(vec != NULL);
    if (vec == NULL) {
        return;
    }
    vec3_t copy = *vec;
    vec->x = copy.x * cos(zrot) + copy.y * sin(zrot) + copy.z * 0;
    vec->y = copy.x * -sin(zrot) + copy.y * cos(zrot) + copy.z * 0;
    vec->z = copy.x * 0 + copy.y * 0 + copy.z * 1;
}

void vec3_rotate(vec3_t *vec, phy_real_t xrot, phy_real_t yrot, phy_real_t zrot) {
    assert(vec != NULL);
    if (vec == NULL) {
        return;
    }
    vec3_rotate_z(vec, zrot);
    vec3_rotate_y(vec, yrot);
    vec3_rotate_x(vec, xrot);
}

void vec3_cross_product(vec3_t *dest, vec3_t a, vec3_t b) {
    assert(dest != NULL);
    if (dest == NULL) {
        return;
    }
    dest->x = (a.y * b.z) - (a.z * b.y);
    dest->y = (a.z * b.x) - (a.x * b.z);
    dest->z = (a.x * b.y) - (a.y * b.x);
}

phy_real_t vec3_dot_product(vec3_t a, vec3_t b) {
    return
        (a.x * b.x) +
        (a.y * b.y) +
        (a.z * b.z);
}

/**
 * Gets the portion of a given vector
 * 'in the same direction' as another
 * vector.  For example, if the original is (1,2,4)
 * and the direction is (0,1,0), the result would
 * be (0,2,0)
 */
void vec3_get_portion_in_direction(vec3_t *result, vec3_t original, vec3_t direction) {
    assert(result != NULL);
    if (result == NULL) {
        return;
    }
    // get unit vectors
    vec3_t orig_unit = original;
    vec3_unit(&orig_unit);
    vec3_t dir_unit = direction;
    vec3_unit(&dir_unit);
    // get magnitudes
    phy_real_t orig_magnitude = vec3_magnitude(original);
    // get dot products
    phy_real_t dot_units = vec3_dot_product(orig_unit, dir_unit);
    // do the math
    // result = dir_unit * |original| * (orig_unit <dot> dir_unit)
    *result = dir_unit;
    vec3_multiply_by(result, orig_magnitude * dot_units);
}
