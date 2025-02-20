#include "common/vec4.h"

#include <math.h>

void vec4_add_to(vec4_t *dest, vec4_t source, phy_real_t factor) {
    dest->x += source.x * factor;
    dest->y += source.y * factor;
    dest->z += source.z * factor;
    dest->w += source.w * factor;
}

void vec4_multiply_by(vec4_t *vec, phy_real_t factor) {
    vec->x *= factor;
    vec->y *= factor;
    vec->z *= factor;
    vec->w *= factor;
}

void vec4_clear(vec4_t *vec) {
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
    vec->w = 0;
}

phy_real_t vec4_distance_to(vec4_t from, vec4_t to) {
    phy_real_t dx = to.x - from.x;
    phy_real_t dy = to.y - from.y;
    phy_real_t dz = to.z - from.z;
    phy_real_t dw = to.w - from.w;
    return sqrt(
        (dx * dx) +
        (dy * dy) +
        (dz * dz) +
        (dw * dw)
    );
}

phy_real_t vec4_magnitude_sqr(vec4_t vec) {
    return
        (vec.x * vec.x) +
        (vec.y * vec.y) +
        (vec.z * vec.z) +
        (vec.w * vec.w);
}

phy_real_t vec4_magnitude(vec4_t vec) {
    return sqrt(
        (vec.x * vec.x) +
        (vec.y * vec.y) +
        (vec.z * vec.z) +
        (vec.w * vec.w)
    );
}

void vec4_unit(vec4_t *vec) {
    phy_real_t magnitude = vec4_magnitude(*vec);
    vec4_multiply_by(vec, 1 / magnitude);
}

void vec4_cross_product(vec4_t *dest, vec4_t a, vec4_t b) {
    dest->x =
        (a.x * b.x) - (a.y * b.y) - (a.z * b.z) - (a.w * b.w);
    dest->y =
        (a.x * b.y) + (a.y * b.x) + (a.z * b.w) - (a.w * b.z);
    dest->z =
        (a.x * b.z) - (a.y * b.w) + (a.z * b.x) + (a.w * b.y);
    dest->w =
        (a.x * b.w) + (a.y * b.z) - (a.z * b.y) + (a.w * b.x);
}

phy_real_t vec4_dot_product(vec4_t a, vec4_t b) {
    return
        (a.x * b.x) +
        (a.y * b.y) +
        (a.z * b.z) +
        (a.w * b.w);
}

void quaternion_conjugate(quaternion_t *q) {
    phy_real_t magnitude_sqr = quaternion_magnitude_sqr(*q);
    vec4_multiply_by(q, 1 / magnitude_sqr);
}

// both quaternion rotation methods taken and modified from https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
void vec3_rotate_by_quaternion_pure(vec3_t *dest, vec3_t vec, quaternion_t q) {
    quaternion_t expanded_vec = vec4_make(vec.x, vec.y, vec.z, 0);
    quaternion_product(&expanded_vec, q, expanded_vec);
    quaternion_conjugate(&q);
    quaternion_product(&expanded_vec, expanded_vec, q);
    dest->x = expanded_vec.x;
    dest->y = expanded_vec.y;
    dest->z = expanded_vec.z;
}

void vec3_rotate_by_quaternion_fast(vec3_t *dest, vec3_t source, quaternion_t q) {
    // Extract the vector part of the quaternion
    vec3_t q_vec = vec3_make(q.x, q.y, q.z);

    // Extract the scalar part of the quaternion
    phy_real_t scalar = q.w;

    // calculate dot products
    phy_real_t dot_q_s = vec3_dot_product(q_vec, source);
    phy_real_t dot_q_q = vec3_dot_product(q_vec, q_vec);

    // calculate cross products
    vec3_t cross_q_s;
    vec3_cross_product(&cross_q_s, q_vec, source);

    // Do the math
    /// vprime = 2.0f * dot(q_vec, source) * q_vec
    ///        + (scalar*scalar - dot(q_vec, q_vec)) * source
    ///        + 2.0f * scalar * cross(q_vec, source);

    /// vprime = 2.0f * dot(q_vec, source) * q_vec
    *dest = q_vec;
    vec3_multiply_by(dest, 2.0 * dot_q_s);

    ///        + (scalar*scalar - dot(q_vec, q_vec)) * source
    vec3_add_to(dest, source, scalar * scalar - dot_q_q);

    ///        + 2.0f * scalar * cross(q_vec, source);
    vec3_add_to(dest, cross_q_s, 2.0 * scalar);
}
#define vec3_rotate_by_quaternion(dest, source, q) vec3_rotate_by_quaternion_fast(dest, source, q)
