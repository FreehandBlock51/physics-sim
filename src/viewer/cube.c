#include "viewer/cube.h"

#include "viewer/aabb.h"

void ccube_make_transform(ccube_t cube, mat4 transform) {
    glm_quat_mat4(vec4_to_cglm(cube.rotation), transform);
    glm_translate(transform, vec3_to_cglm(cube.position));
}

void ccube_gen_vertices(ccube_t cube, GLfloat vertices[CCUBE_VERTEX_ARRAY_SIZE], GLuint indices[CCUBE_INDEX_ARRAY_SIZE]) {
    // since a cube is just a rotated bounding box, we can delegate our
    // generating to aabb.h and use transform matrix for rotation
    bbox_t cube_box;
    bbox_make(&cube_box, cube.position.x, cube.position.y, cube.position.z, cube.length, cube.width, cube.height);
    bbox_gen_vertices(cube_box, vertices, indices);
}
