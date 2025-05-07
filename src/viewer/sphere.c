#include "viewer/sphere.h"

#include "common/defines.h"

void csphere_make_transform(csphere_t sphere, mat4 transform) {
    glm_mat4_identity(transform);
    glm_translate(transform, vec3_to_cglm(sphere.position));
}

void csphere_gen_vertices(csphere_t sphere, GLfloat vertices[CSPHERE_VERTEX_ARRAY_SIZE]) {
    safe_assert(vertices != NULL,);

    size_t i = 0;
    for (float u = 0; u < 2 * GLM_PIf; u += GLM_PI_4f) {
        for (float v = 0; v < 2 * GLM_PIf; v += GLM_PI_4f) {
            vertices[i++] = sphere.radius;
            vertices[i++] = u;
            vertices[i++] = v;
        }
    }
}
