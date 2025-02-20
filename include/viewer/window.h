#pragma once
/**
 * Handles the window lifecycle and events
 */

#include <stdint.h>
#include <stdbool.h>
#include "gl_includes.h"
#include "viewer/color.h"

/**
 * The state of a keyboard key
 */
typedef int32_t window_key_state_t;

/**
 * Represents a key that has just been pressed
 */
#define WINDOW_KEY_FIRSTPRESS 1

/**
 * Represents a key that has just been released
 */
#define WINDOW_KEY_FIRSTRELEASE 0

/**
 * Represents a key that has not changed state for multiple frames
 */
#define WINDOW_KEY_SAMESTATE 2

/**
 * Represents a key that is being held down
 */
#define WINDOW_KEY_DOWN (WINDOW_KEY_FIRSTPRESS | WINDOW_KEY_SAMESTATE)

/**
 * Represents a key that is not pressed
 */
#define WINDOW_KEY_UP (WINDOW_KEY_FIRSTRELEASE | WINDOW_KEY_SAMESTATE)

/**
 * If this value is returned, an error has occured with GLFW
 */
#define WINDOW_ERROR_GLFW 128;

/**
 * If this value is returned, the parameters to the called function were incorrect
 */
#define WINDOW_ERROR_PARAMS 81;

/**
 * The size of the window keymap
 */
#define WINDOW_KEYMAP_SIZE GLFW_KEY_LAST

/**
 * A graphical window
 */
struct Window {
    GLFWwindow *glfw_handle;
    window_key_state_t keymap[WINDOW_KEYMAP_SIZE];
};

typedef struct Window window_t;

/**
 * Initializes all systems that work with the window (glfw, glad, etc.),
 * then creates a window, assigns it as the current context, and scales
 * the GL viewport to match its dimensions;
 * If any initialization fails, returns the error code;
 *  otherwise, return 0 on success
 */
int window_init(int width, int height, const char *title, window_t *window);

/**
 * @brief Gets the size of the window
 * @param window The window to check
 * @param width Populated with the width of the window (can be null)
 * @param height Populated with the height of the window (can be null)
 */
void window_get_size(window_t window, int *width, int *height);

/**
 * Checks if the window should be closed
 */
bool window_should_close(window_t window);

/**
 * Notifies the application that the window should be closed
 */
void window_close(window_t window);

/**
 * Clears the screen with the given background color
 */
void window_clear(color_t bgcolor);

/**
 * Swaps the window buffers to display the newly drawn frame
 * Also processes window events
 */
void window_end_drawing(window_t *window);

/**
 * Gets the current state of a given key --
 * whether it's pressed or not, and if that
 * state changed this frame
 */
window_key_state_t window_get_key(window_t *window, int key);

/**
 * Checks if a given key is being held down
 */
bool window_is_key_down(window_t *window, int key);

/**
 * Checks if a given key is not being held down
 */
bool window_is_key_up(window_t *window, int key);

/**
 * Checks if a given key has been pressed for the first time
 */
bool window_is_key_pressed(window_t *window, int key);

/**
 * Checks if a given key has just been released
 */
bool window_is_key_released(window_t *window, int key);

/**
 * Cleans up all of the window systems (eg: glfw)
 */
void window_cleanup();
