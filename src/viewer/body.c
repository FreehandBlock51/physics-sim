#include "viewer/body.h"

void body_gen_transform(body_t *body, mat4 transform_matrix) {
    safe_assert(body != NULL,);

    mat4 rotation;

    glm_mat4_identity(transform_matrix);
    glm_translate(transform_matrix, vec3_to_cglm(body->position));

    glm_mat4_identity(rotation);
    glm_euler(vec3_to_cglm(body->rotation), rotation);

    glm_mat4_mul(transform_matrix, rotation, transform_matrix);
}
