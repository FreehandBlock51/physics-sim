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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Physics Sim"
const color_t WINDOW_BACKGROUND_COLOR = COLOR_PURPLE;

#include "shaders/matrix_vertex.h"
#include "shaders/uniform_color_fragment.h"

static GLfloat vertices[BBOX_VERTEX_ARRAY_SIZE];
static GLuint  indices[BBOX_INDEX_ARRAY_SIZE];

int graphic_main(void) {
    int result;
    window_t *window = calloc(1, sizeof(window_t));
    if ((result = window_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, window))) {
        return result;
    }

    body_t body;
    body_make(&body, vec3_make(0, 0, 0), VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 1, 0, 0);
    bbox_t box;
    bbox_make(&box, 0, 0, 0, 10, 10, 10);
    bbox_gen_vertices(box, vertices, indices);

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

    // Create a Vertex Array Object (VAO) to store the environment we
    // need to draw (vertices, vertex attributes, etc.)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Create a Vertex Buffer Object (VBO) to store our vertices
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Create an Element Buffer Object (EBO) to store our vertices
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);

    // put vertices in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (sizeof vertices), vertices, GL_STATIC_DRAW);

    // put indices in the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof indices), indices, GL_STATIC_DRAW);

    // set vertex attributes (how shader expects vertex layout)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof *vertices), NULL);
    glEnableVertexAttribArray(0);

    color_t color = COLOR_GREEN;

    mat4 model; // generated each frame

    camera_t camera;
    camera_make(&camera, (vec3){ 0, 0, 25 }, (vec3){ 0, 0, -1 }, (vec3){ 0, 1, 0 }, GLM_PI_4f, 1.0f, 1.0f, 0.1f, 100.0f);

    mat4 view;

    mat4 projection;

    while (!window_should_close(*window))
    {
        window_clear(WINDOW_BACKGROUND_COLOR);

        // modify transformations
        if (window_is_key_down(window, GLFW_KEY_R)) {
            body.rotation.x += 0.01;
            body.rotation.z += 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_UP)) {
            body.position.y += 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_DOWN)) {
            body.position.y -= 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_RIGHT)) {
            body.position.x += 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_LEFT)) {
            body.position.x -= 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_RIGHT_BRACKET)) {
            body.position.z += 0.01;
        }
        if (window_is_key_down(window, GLFW_KEY_LEFT_BRACKET)) {
            body.position.z -= 0.01;
        }
        if (window_is_key_released(window, GLFW_KEY_ESCAPE)) {
            l_printf("window closing...\n");
            window_close(*window);
        }

        // generate transformation matrix
        body_gen_transform(&body, model);        

        camera_gen_view_matrix(camera, view);
        camera_gen_projection_matrix(camera, projection);

        // set VAO and shader, then draw
        shader_program_use(shader_program);
        // cast to float* because we know that a matrix works here
        glUniformMatrix4fv(u_model, 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(u_view, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(u_projection, 1, GL_FALSE, (float*)projection);
        glUniform4f(u_color, color.r, color.g, color.b, color.a);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, BBOX_INDEX_ARRAY_SIZE, GL_UNSIGNED_INT, 0);

        window_end_drawing(window);
    }

    free(window);
    window_cleanup();
    return 0;
}
