#pragma once
/**
 * A helper header that includes all headers necessary for OpenGL in
 * the right order
 */

#include <glad/glad.h> // must be included before glfw3
#include <GLFW/glfw3.h>

/* Vertex segment offsets in a vertex array */
#define VIEWER_VERTEX_OFFSET_X 0
#define VIEWER_VERTEX_OFFSET_Y 1
#define VIEWER_VERTEX_OFFSET_Z 2
