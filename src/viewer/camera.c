#include "viewer/camera.h"

#include "common/math.h"

int camera_make(camera_t *camera, vec3 position, vec3 forward, vec3 up, float fov, float near_plane, float far_plane) {
    if (near_plane > far_plane) {
        return -1;
    }

    glmc_vec3_copy(position, camera->position);
    glmc_vec3_normalize_to(forward, camera->forward);
    glmc_vec3_normalize_to(up, camera->up);
    camera->fov = fov;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;
    return 0;
}

void camera_gen_view_matrix(camera_t camera, mat4 matrix) {
    glmc_vec3_normalize(camera.forward);
    glmc_vec3_normalize(camera.up);
    glmc_look(camera.position, camera.forward, camera.up, matrix);
}

void camera_gen_projection_matrix(camera_t camera, window_t window, mat4 matrix) {
    int width, height;
    window_get_size(window, &width, &height);
    glmc_perspective(camera.fov, (float)height / (float)width, camera.near_plane, camera.far_plane, matrix);
}
