#pragma once

/**
 * Utilities for drawing cubes
 */

#include <gl_includes.h>
#include <cglm/cglm.h>
#include "sim/cube.h"

#define CCUBE_VERTEX_ARRAY_SIZE 24

#define CCUBE_INDEX_ARRAY_SIZE 36

/**
 * Creates a transformation matrix for a cube
 */
void ccube_make_transform(ccube_t cube, mat4 transform);

/**
 * Generates both the vertex array and index array for
 * a cube.  The vertex array must have as much
 * space as required by CCUBE_VERTEX_ARRAY_SIZE, and the
 * index array must have as much space as required by
 * CCUBE_INDEX_ARRAY_SIZE
 */
void ccube_gen_vertices(ccube_t cube, GLfloat vertices[CCUBE_VERTEX_ARRAY_SIZE], GLuint indices[CCUBE_INDEX_ARRAY_SIZE]);
