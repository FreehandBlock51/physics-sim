/**
 * The entry point for the simulation -- graphical mode
 */

#include <stdio.h>
#include <malloc.h>
#include <gl_includes.h>
#include <cglm/cglm.h>
#include "sim/aabb.h"
#include "sim/body.h"
#include "common/defines.h"
#include "viewer/window.h"
#include "viewer/shader.h"
#include "viewer/color.h"
#include "viewer/camera.h"
#include "viewer/aabb.h"
#include "viewer/body.h"
#include "viewer/model.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Physics Sim"
const color_t WINDOW_BACKGROUND_COLOR = COLOR_PURPLE;

#include "shaders/matrix_vertex.h"
#include "shaders/uniform_color_fragment.h"

static GLfloat box1_vertices[BBOX_VERTEX_ARRAY_SIZE];
static GLuint  box1_indices[BBOX_INDEX_ARRAY_SIZE];
static GLfloat box2_vertices[BBOX_VERTEX_ARRAY_SIZE];
static GLuint  box2_indices[BBOX_INDEX_ARRAY_SIZE];

#define CAMERA_MOVE_SPEED 0.05
#define CAMERA_ROTATE_SPEED 0.0025

#define PHY_FRAMES_PER_STEP -1

int graphic_main(void) {
    vec3 GLOBAL_UP = { 0, 1, 0 };


    int result;
    window_t *window = calloc(1, sizeof(window_t));
    if ((result = window_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, window))) {
        return result;
    }

    bbox_t box1, box2;
    bbox_make(&box1, 0, 0, 0, 3, 3, 3);
    bbox_make(&box2, 0, 0, 0, 1, 1, 1);
    body_t body1, body2;
    body_make(&body1, vec3_make(0, 0, 0), VEC3_ZERO, VEC3_UP, VEC3_ZERO, bbox_get_volume(box1), 0, 0);
    body_make(&body2, vec3_make(-1, 1, 7), VEC3_ZERO, VEC3_DOWN, VEC3_ZERO, bbox_get_volume(box2), 0, 0);
    bbox_gen_vertices(box1, box1_vertices, box1_indices);
    bbox_gen_vertices(box2, box2_vertices, box2_indices);

    l_printf("Building shaders...\n");

    l_printf("Compiling vertex shader...\n");
    shader_t vertex_shader;
    if ((result = shader_compile(GL_VERTEX_SHADER, shader_src_matrix_vertex, &vertex_shader))) {
        glfwTerminate();
        return result;
    }

    l_printf("Compiling fragment shader...\n");
    shader_t fragment_shader;
    if ((result = shader_compile(GL_FRAGMENT_SHADER, shader_src_uniform_color_fragment, &fragment_shader))) {
        glfwTerminate();
        return result;
    }

    shader_program_t shader_program = shader_program_create();

    l_printf("Attaching vertex shader...\n");
    shader_program_attach_shader(shader_program, vertex_shader);

    l_printf("Attaching fragment shader...\n");
    shader_program_attach_shader(shader_program, fragment_shader);

    l_printf("Linking program...\n");
    if ((result = shader_program_link(shader_program)) != 0) {
        return result;
    }

    shader_uniform_t u_model = shader_uniform_get_location(shader_program, "model");
    shader_uniform_t u_view = shader_uniform_get_location(shader_program, "view");
    shader_uniform_t u_projection = shader_uniform_get_location(shader_program, "projection");
    shader_uniform_t u_color = shader_uniform_get_location(shader_program, "color");

    shader_delete(vertex_shader);
    shader_delete(fragment_shader);

    l_printf("Shaders successfully built!\n");

    model_t box1_model = model_from_indices(box1_vertices, 3, BBOX_VERTEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, box1_indices, BBOX_INDEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, MODEL_DRAW_TRIANGLES);
    model_t box2_model = model_from_indices(box2_vertices, 3, BBOX_VERTEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, box2_indices, BBOX_INDEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, MODEL_DRAW_TRIANGLES);

    color_t color1 = COLOR_GREEN;
    color_t color2 = COLOR_BLUE;

    mat4 model1, model2; // generated each frame

    camera_t camera;
    camera_make(&camera, (vec3){ 0, 0, 25 }, (vec3){ 0, 0, -1 }, (vec3){ 0, 1, 0 }, GLM_PI_4f, 1.0f, 1.0f, 0.1f, 100.0f);

    mat4 view;

    mat4 projection;

#if PHY_FRAMES_PER_STEP >= 0
    uint32_t frames_since_last_phy_step = 0;
#endif

    while (!window_should_close(*window))
    {
        window_clear(WINDOW_BACKGROUND_COLOR);

#if PHY_FRAMES_PER_STEP >= 0
        if (++frames_since_last_phy_step >= PHY_FRAMES_PER_STEP) {
            frames_since_last_phy_step = 0;
#else
        if (window_is_key_pressed(window, GLFW_KEY_SPACE) || window_is_key_down(window, GLFW_KEY_LEFT_CONTROL)) {
#endif
            phy_body_add_gravity_force(&body1, &body2);

#ifndef NOCOLLISION
            if (bbox_is_bbox_inside(box1, box2)) {
                // a and b are colliding; find normal
                // and do collision forces
                vec3_t a_closest_b = box1.position;
                bbox_clamp_point_within_bounds(box2, &a_closest_b);
                vec3_t b_closest_a = box2.position;
                bbox_clamp_point_within_bounds(box1, &b_closest_a);
                vec3_t normal_a_b = b_closest_a;
                vec3_add_to(&normal_a_b, a_closest_b, -1);
                vec3_unit(&normal_a_b);
                phy_calculate_normal_force(&normal_a_b, body1, normal_a_b);
                phy_body_add_collision_forces(&body1, &body2, normal_a_b);
            }
#endif

            phy_body_step(&body1);
            phy_body_step(&body2);
        }

        box1.position = body1.position;
        box2.position = body2.position;

        // move camera up and down
        if (window_is_key_down(window, GLFW_KEY_E)) {
            glm_vec3_muladds(camera.up, CAMERA_MOVE_SPEED, camera.position);
        }
        if (window_is_key_down(window, GLFW_KEY_Q)) {
            glm_vec3_muladds(camera.up, -CAMERA_MOVE_SPEED, camera.position);
        }

        // move camera left and right
        if (window_is_key_down(window, GLFW_KEY_D)) {
            vec3 right;
            glm_vec3_cross(camera.forward, camera.up, right);
            glm_vec3_muladds(right, CAMERA_MOVE_SPEED, camera.position);
        }
        if (window_is_key_down(window, GLFW_KEY_A)) {
            vec3 right;
            glm_vec3_cross(camera.forward, camera.up, right);
            glm_vec3_muladds(right, -CAMERA_MOVE_SPEED, camera.position);
        }

        // move camera forward and backward
        if (window_is_key_down(window, GLFW_KEY_S)) {
            glm_vec3_muladds(camera.forward, -CAMERA_MOVE_SPEED, camera.position);
        }
        if (window_is_key_down(window, GLFW_KEY_W)) {
            glm_vec3_muladds(camera.forward, CAMERA_MOVE_SPEED, camera.position);
        }

        // Rotations must be applied to both the camera's forward and
        // up to maintain the invariant that the two vectors must be
        // purpendicular.  Use global directions so rotation doesn't
        // stack

        // rotate camera left and right
        if (window_is_key_down(window, GLFW_KEY_LEFT)) {
            glm_vec3_rotate(camera.forward, CAMERA_ROTATE_SPEED, GLOBAL_UP);
            glm_vec3_rotate(camera.up, CAMERA_ROTATE_SPEED, GLOBAL_UP);
        }
        if (window_is_key_down(window, GLFW_KEY_RIGHT)) {
            glm_vec3_rotate(camera.forward, -CAMERA_ROTATE_SPEED, GLOBAL_UP);
            glm_vec3_rotate(camera.up, -CAMERA_ROTATE_SPEED, GLOBAL_UP);
        }

        // rotate camera up and down.
        if (window_is_key_down(window, GLFW_KEY_UP)) {
            vec3 right;
            glm_vec3_cross(camera.forward, camera.up, right);
            glm_vec3_rotate(camera.forward, 0.001, right);
            glm_vec3_rotate(camera.up, 0.001, right);
        }
        if (window_is_key_down(window, GLFW_KEY_DOWN)) {
            vec3 right;
            glm_vec3_cross(camera.forward, camera.up, right);
            glm_vec3_rotate(camera.forward, -0.001, right);
            glm_vec3_rotate(camera.up, -0.001, right);
        }

        // close the window if asked to
        if (window_is_key_released(window, GLFW_KEY_ESCAPE)) {
            l_printf("window closing...\n");
            window_close(*window);
        }

        // generate transformation matrices
        body_gen_transform(&body1, model1);
        body_gen_transform(&body2, model2);

        camera_gen_view_matrix(camera, view);
        camera_gen_projection_matrix(camera, projection);

        // set VAO and shader, then draw
        shader_program_use(shader_program);
        // cast to float* because we know that a matrix works here
        glUniformMatrix4fv(u_model, 1, GL_FALSE, (float*)model1);
        glUniformMatrix4fv(u_view, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(u_projection, 1, GL_FALSE, (float*)projection);
        glUniform4f(u_color, color1.r, color1.g, color1.b, color1.a);
        model_draw(box1_model);

        // ...and do it again for the second box
        shader_program_use(shader_program);
        // cast to float* because we know that a matrix works here
        glUniformMatrix4fv(u_model, 1, GL_FALSE, (float*)model2);
        glUniformMatrix4fv(u_view, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(u_projection, 1, GL_FALSE, (float*)projection);
        glUniform4f(u_color, color2.r, color2.g, color2.b, color2.a);
        model_draw(box2_model);

        window_end_drawing(window);
    }

    free(window);
    window_cleanup();
    return 0;
}
