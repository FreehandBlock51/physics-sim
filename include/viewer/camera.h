#pragma once

/**
 * Utilities and definitions for a camera.
 */

#include <cglm/cglm.h>
#include "common/defines.h"
#include "viewer/window.h"

struct Camera {
    vec3 position;
    vec3 forward;
    vec3 up;
    float fov;
    float near_plane;
    float far_plane;
};
typedef struct Camera camera_t;

/**
 * Creates a camera
 * @returns 0 on success, -1 on invalid input
 */
int camera_make(camera_t *camera, vec3 position, vec3 forward, vec3 up, float fov, float near_plane, float far_plane);

/**
 * Generates a view matrix for the given camera
 */
void camera_gen_view_matrix(camera_t camera, mat4 matrix);

/**
 * @brief Generates a projection matrix for the given camera
 * @param camera The camera used to generate the matrix 
 * @param window The window used to get the aspect ratio
 * @param matrix The destination matrix used to store the output
 */
void camera_gen_projection_matrix(camera_t camera, window_t window, mat4 matrix);
