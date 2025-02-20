#include "viewer/transform.h"


void transform_make(
        transform_t *transform,
        vec3_t position,
        quaternion_t rotation,
        vec3_t scale) {
    transform->position = position;
    transform->rotation = rotation;
    transform->scale = scale;
}

void transform_translate(transform_t *transform, vec3_t translation) {
    vec3_add_to(&transform->position, translation, 1);
}

void transform_set_position(transform_t *transform, vec3_t position) {
    transform->position = position;
}

void transform_rotate(transform_t *transform, vec3_t axis, phy_real_t rotation) {
    vec3_unit(&axis);
    quaternion_t rotq = quaternion_make(axis.x, axis.y, axis.z, rotation);
    quaternion_product(&transform->rotation, transform->rotation, rotq);
}

void transform_set_rotation(transform_t *transform, quaternion_t rotation) {
    transform->rotation = rotation;
}

void transform_scale(transform_t *transform, vec3_t scale) {
    vec3_add_to(&transform->scale, scale, 1);
}

void transform_set_scale(transform_t *transform, vec3_t scale) {
    transform->scale = scale;
}

/* Utility functions for matrix generation */
PRIVATE_FUNC void gen_translation_matrix(mat4x4_t *dest, vec3_t translation) {
    mat4x4_make_identity(*dest);
    *dest[0][3] = translation.x;
    *dest[1][3] = translation.y;
    *dest[2][3] = translation.z;
}

PRIVATE_FUNC void gen_rotation_matrix(mat4x4_t *dest, quaternion_t rotation) {
    // taken and modified from https://automaticaddison.com/how-to-convert-a-quaternion-to-a-rotation-matrix/

    mat4x4_make_identity(*dest);

    // First row of the rotation matrix
    *dest[0][0] = 2.0 * (rotation.w * rotation.w + rotation.x * rotation.x) - 1;
    *dest[0][1] = 2.0 * (rotation.x * rotation.y - rotation.w * rotation.z);
    *dest[0][2] = 2.0 * (rotation.x * rotation.z + rotation.w * rotation.y);

    // Second row of the rotation matrix
    *dest[1][0] = 2.0 * (rotation.x * rotation.y + rotation.w * rotation.z);
    *dest[1][1] = 2.0 * (rotation.w * rotation.w + rotation.y * rotation.y) - 1;
    *dest[1][2] = 2.0 * (rotation.y * rotation.z - rotation.w * rotation.x);

    // Third row of the rotation matrix
    *dest[2][0] = 2.0 * (rotation.x * rotation.z - rotation.w * rotation.y);
    *dest[2][1] = 2.0 * (rotation.y * rotation.z + rotation.w * rotation.x);
    *dest[2][2] = 2.0 * (rotation.w * rotation.w + rotation.z * rotation.z) - 1;
}

PRIVATE_FUNC void gen_scale_matrix(mat4x4_t *dest, vec3_t scale) {
    mat4x4_make_identity(*dest);
    *dest[0][0] = scale.x;
    *dest[1][1] = scale.y;
    *dest[2][2] = scale.z;
}

void transform_gen_matrix(transform_t transform, mat4x4_t *dest) {
    mat4x4_t intermediate = MAT4x4_IDENTITY; // used as dest for the individual generators

    // first generation applies to dest directly because we are
    // overwriting what was already in there before the call
    gen_translation_matrix(dest, transform.position);

    // after the first generation, gen to intermediate and multiply with
    // dest
    gen_rotation_matrix(&intermediate, transform.rotation);
    mat4x4_times_mat4x4(*dest, *dest, intermediate);
    gen_scale_matrix(&intermediate, transform.scale);
    mat4x4_times_mat4x4(*dest, *dest, intermediate);
}
