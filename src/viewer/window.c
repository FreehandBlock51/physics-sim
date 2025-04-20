#include "viewer/window.h"

#include <stdio.h>
#include "common/defines.h"

/**
 * An internal flag singaling that a keystate should not be rechecked this frame
 */
#define WINDOW_KEY_ALREADY_CHECKED 64

/**
 * If iserror is true, a GLFW action has failed.  Print GLFW's error
 * message (prefixed by msg_prefix) and pass the error code to
 * error_code_handler via `error_code_handler (code);`
 *  NOTE: error_code_handler could be a function that takes a single
 *   integer parameter (like exit()) or the return keyword
 */
#define GLFW_FAIL_ON_ERROR(iserror, msg_prefix, error_code_handler) \
if (iserror) { \
    const char *__err_msg; \
    int __err_code = glfwGetError(&__err_msg); \
    l_printf(msg_prefix "\n\t%s\n", __err_msg); \
    glfwTerminate(); \
    error_code_handler (__err_code); \
}


/* Callbacks */

/**
 * Called whenever the window is resized
 */
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window; // this param is needed as part of the callback signature
    glViewport(0, 0, width, height);
}

/* End Callbacks */

int window_init(int width, int height, const char *title, window_t *window) {
    GLFW_FAIL_ON_ERROR(glfwInit() == GLFW_FALSE, "Fatal: Could not initialize GLFW!", return)

    // Using OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // required on macos
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the window
    window->glfw_handle = glfwCreateWindow(width, height, title, NULL, NULL);
    GLFW_FAIL_ON_ERROR(window->glfw_handle == NULL, "Fatal: Could not create window!", return)
    glfwMakeContextCurrent(window->glfw_handle);

    // use GLAD to retrieve the locations of the OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        l_printf("Fatal: Could not initialize GLAD!\n");
        glfwTerminate();
        return -1;
    }

    // fit the viewport to the window dimensions (starts at origin)
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window->glfw_handle, (GLFWframebuffersizefun)framebuffer_size_callback);

    // set all keys in the keymap to up
    for (int i = 0; i < WINDOW_KEYMAP_SIZE; i++) {
        window->keymap[i] = GLFW_RELEASE;
    }

    glEnable(GL_DEPTH_TEST);

    return 0;
}

void window_get_size(window_t window, int *width, int *height) {
    static int unused_width, unused_height;
    if (width == NULL) {
        width = &unused_width;
    }
    if (height == NULL) {
        height = &unused_height;
    }
    glfwGetFramebufferSize(window.glfw_handle, width, height);
}

bool window_should_close(window_t window) {
    return glfwWindowShouldClose(window.glfw_handle) == GLFW_TRUE;
}

void window_close(window_t window) {
    glfwSetWindowShouldClose(window.glfw_handle, GLFW_TRUE);
}

void window_clear(color_t bgcolor) {
    glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_end_drawing(window_t *window) {
    glfwSwapBuffers(window->glfw_handle);
    glfwPollEvents();
    for (int key = 0; key < WINDOW_KEYMAP_SIZE; key++) {
        window->keymap[key] &= ~WINDOW_KEY_ALREADY_CHECKED;
    }
}

window_key_state_t window_get_key(window_t *window, int key) {
    if (key < 0 || key >= WINDOW_KEYMAP_SIZE) {
        return WINDOW_ERROR_PARAMS;
    }
    window_key_state_t oldstate = window->keymap[key];
    if (oldstate & WINDOW_KEY_ALREADY_CHECKED) {
        return (oldstate & ~WINDOW_KEY_ALREADY_CHECKED);
    }
    window_key_state_t newstate;
    switch (glfwGetKey(window->glfw_handle, key)) {
        case GLFW_PRESS:
            newstate = WINDOW_KEY_FIRSTPRESS;
            break;
        case GLFW_RELEASE:
            newstate = WINDOW_KEY_FIRSTRELEASE;
            break;
        default: // If we can't parse the result, assume error
        case GLFW_INVALID_ENUM:
            return WINDOW_ERROR_GLFW;
    }
    if (newstate == (oldstate & ~WINDOW_KEY_SAMESTATE)) {
        // encode whether or not the state has changed
        newstate |= WINDOW_KEY_SAMESTATE;
    }
    window->keymap[key] = newstate | WINDOW_KEY_ALREADY_CHECKED;
    return newstate;
}

bool window_is_key_down(window_t *window, int key) {
    window_key_state_t state = window_get_key(window, key);
    return state == WINDOW_KEY_DOWN || state == WINDOW_KEY_FIRSTPRESS;
}

bool window_is_key_up(window_t *window, int key) {
    window_key_state_t state = window_get_key(window, key);
    return state == WINDOW_KEY_UP || state == WINDOW_KEY_FIRSTRELEASE;
}

bool window_is_key_pressed(window_t *window, int key) {
    return window_get_key(window, key) == WINDOW_KEY_FIRSTPRESS;
}

bool window_is_key_released(window_t *window, int key) {
    return window_get_key(window, key) == WINDOW_KEY_FIRSTRELEASE;
}

void window_cleanup() {
    glfwTerminate();
}
