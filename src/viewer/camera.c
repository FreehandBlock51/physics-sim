#include "viewer/camera.h"

#include "common/math.h"

int camera_make(camera_t *camera, vec3 position, vec3 forward, vec3 up, float fov, float aspect_width, float aspect_height, float near_plane, float far_plane) {
    if (near_plane > far_plane) {
        return -1;
    }

    glm_vec3_copy(position, camera->position);
    glm_vec3_normalize_to(forward, camera->forward);
    glm_vec3_normalize_to(up, camera->up);
    camera->fov = fov;
    camera->aspect_width = aspect_width;
    camera->aspect_height = aspect_height;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;
    return 0;
}

void camera_gen_view_matrix(camera_t camera, mat4 matrix) {
    glm_vec3_normalize(camera.forward);
    glm_vec3_normalize(camera.up);
    glm_look(camera.position, camera.forward, camera.up, matrix);
}

void camera_gen_projection_matrix(camera_t camera, mat4 matrix) {
    glm_perspective(camera.fov, camera.aspect_width / camera.aspect_height, camera.near_plane, camera.far_plane, matrix);
}
