/**
 * The entry point for the simulation -- graphical mode
 */

#include <stdio.h>
#include <malloc.h>
#include <gl_includes.h>
#include <cglm/cglm.h>
#include "sim/cube.h"
#include "sim/body.h"
#include "sim/constraints.h"
#include "common/defines.h"
#include "viewer/window.h"
#include "viewer/shader.h"
#include "viewer/color.h"
#include "viewer/camera.h"
#include "viewer/cube.h"
#include "viewer/body.h"
#include "viewer/model.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Physics Sim"
const color_t WINDOW_BACKGROUND_COLOR = COLOR_PURPLE;

#include "shaders/matrix_color_passthrough_vertex.h"
#include "shaders/uniform_color_modified_fragment.h"

static GLfloat cube1_vertices[CCUBE_VERTEX_ARRAY_SIZE];
static GLuint  cube1_indices[CCUBE_INDEX_ARRAY_SIZE];
static GLfloat cube2_vertices[CCUBE_VERTEX_ARRAY_SIZE];
static GLuint  cube2_indices[CCUBE_INDEX_ARRAY_SIZE];

#define CAMERA_MOVE_SPEED 0.05
#define CAMERA_ROTATE_SPEED 0.0125

#define PHY_FRAMES_PER_STEP 0

int graphic_main(void) {
    vec3 GLOBAL_UP = { 0, 1, 0 };


    int result;
    window_t *window = calloc(1, sizeof(window_t));
    if ((result = window_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, window))) {
        return result;
    }

    ccube_t cube1 = ccube_make(VEC3_ZERO, QUATERNION_NOROTATION, 3, 3, 3);
    ccube_t cube2 = ccube_make(VEC3_ZERO, QUATERNION_NOROTATION, 2, 2, 2);
    body_t body1, body2;
    body_make(&body1, vec3_make(1, -5, 0), VEC3_ZERO, vec3_make(0, 1, 0), VEC3_ZERO, 1, 0, 0);
    body_make(&body2, vec3_make(0, 5, 0), VEC3_ZERO, vec3_make(0, -1, 0), VEC3_ZERO, 1, 0, 0);
    ccube_gen_vertices(cube1, cube1_vertices, cube1_indices);
    ccube_gen_vertices(cube2, cube2_vertices, cube2_indices);

    spring_t spring;
    spring_create(&spring, &body1, VEC3_ZERO, &body2, VEC3_ZERO, 0.1, 5);

    l_printf("Building shaders...\n");

    l_printf("Compiling vertex shader...\n");
    shader_t vertex_shader;
    if ((result = shader_compile(GL_VERTEX_SHADER, shader_src_matrix_color_passthrough_vertex, &vertex_shader))) {
        window_cleanup();
        return result;
    }

    l_printf("Compiling fragment shader...\n");
    shader_t fragment_shader;
    if ((result = shader_compile(GL_FRAGMENT_SHADER, shader_src_uniform_color_modified_fragment, &fragment_shader))) {
        window_cleanup();
        return result;
    }

    shader_program_t shader_program = shader_program_create();

    l_printf("Attaching vertex shader...\n");
    shader_program_attach_shader(shader_program, vertex_shader);

    l_printf("Attaching fragment shader...\n");
    shader_program_attach_shader(shader_program, fragment_shader);

    l_printf("Linking program...\n");
    if ((result = shader_program_link(shader_program)) != 0) {
        window_cleanup();
        return result;
    }

    shader_uniform_t u_model = shader_uniform_get_location(shader_program, "model");
    shader_uniform_t u_view = shader_uniform_get_location(shader_program, "view");
    shader_uniform_t u_projection = shader_uniform_get_location(shader_program, "projection");
    shader_uniform_t u_color = shader_uniform_get_location(shader_program, "color");

    shader_delete(vertex_shader);
    shader_delete(fragment_shader);

    l_printf("Shaders successfully built!\n");

    const mvblueprint_t blueprints[] = { mvblueprint_make(MVT_FLOAT, 3) };
    model_t box1_model = model_from_indices(cube1_vertices, CCUBE_VERTEX_ARRAY_SIZE, blueprints, 1, MODEL_BUFFER_STATIC, cube1_indices, CCUBE_INDEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, MODEL_DRAW_TRIANGLES);
    model_t box2_model = model_from_indices(cube2_vertices, CCUBE_VERTEX_ARRAY_SIZE, blueprints, 1, MODEL_BUFFER_STATIC, cube2_indices, CCUBE_INDEX_ARRAY_SIZE, MODEL_BUFFER_STATIC, MODEL_DRAW_TRIANGLES);

    color_t color1 = COLOR_GREEN;
    color_t color2 = COLOR_BLUE;

    mat4 model1, model2; // generated each frame

    camera_t camera;
    camera_make(&camera, (vec3){ 0, 0, 25 }, (vec3){ 0, 0, -1 }, (vec3){ 0, 1, 0 }, GLM_PI_4f, 1.0f, 1.0f, 0.1f, 100.0f);

    mat4 view;

    mat4 projection;

#if PHY_FRAMES_PER_STEP > 0
    uint32_t frames_since_last_phy_step = 0;
#endif

    bool draw_wireframes = false;

    while (!window_should_close(*window))
    {
        window_clear(WINDOW_BACKGROUND_COLOR);

        {
            // set aspect ratio to current window width:height ratio
            // so that viewport doesn't look wonky
            int width, height;
            window_get_size(*window, &width, &height);
            camera.aspect_width = width;
            camera.aspect_height = height;
        }

#if PHY_FRAMES_PER_STEP > 0
        if (++frames_since_last_phy_step >= PHY_FRAMES_PER_STEP) {
            frames_since_last_phy_step = 0;
#else
        if (window_is_key_pressed(window, GLFW_KEY_SPACE) || window_is_key_down(window, GLFW_KEY_LEFT_CONTROL)) {
#endif
            phy_body_add_gravity_force(&body1, &body2);
            spring_apply_constraint(spring);

#ifndef NOCOLLISION
            if (ccube_is_ccube_inside(cube1, cube2)) {
                // a and b are colliding; find normal
                // and do collision forces
                vec3_t b_closest_a = cube2.position;
                ccube_clamp_point_within_cube(cube1, &b_closest_a);
                vec3_t normal_a_b = ccube_get_surface_normal(cube1, b_closest_a);
                phy_calculate_normal_force(&normal_a_b, body1, normal_a_b);
                phy_body_add_collision_forces(&body1, &body2, normal_a_b, b_closest_a);
            }
#endif

            phy_body_step(&body1);
            phy_body_step(&body2);
        }

        cube1.position = body1.position;
        glm_euler_xyz_quat(vec3_to_cglm(body1.rotation), vec4_to_cglm(cube1.rotation));
        cube2.position = body2.position;
        glm_euler_xyz_quat(vec3_to_cglm(body2.rotation), vec4_to_cglm(cube2.rotation));

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
            glm_vec3_rotate(camera.forward, CAMERA_ROTATE_SPEED, right);
            glm_vec3_rotate(camera.up, CAMERA_ROTATE_SPEED, right);
        }
        if (window_is_key_down(window, GLFW_KEY_DOWN)) {
            vec3 right;
            glm_vec3_cross(camera.forward, camera.up, right);
            glm_vec3_rotate(camera.forward, -CAMERA_ROTATE_SPEED, right);
            glm_vec3_rotate(camera.up, -CAMERA_ROTATE_SPEED, right);
        }

        // toggle wireframe drawing
        if (window_is_key_pressed(window, GLFW_KEY_F4)) {
            draw_wireframes = !draw_wireframes;
            if (draw_wireframes) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
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

    window_cleanup();
    free(window);
    return 0;
}
