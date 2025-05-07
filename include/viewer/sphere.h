#pragma once

/**
 * Utilities for rendering a sphere
 */


#include <gl_includes.h>
#include <cglm/cglm.h>
#include "sim/sphere.h"

/**
 * The minimum size required of a vertex array for a sphere
 */
#define CSPHERE_VERTEX_ARRAY_SIZE 49

/**
 * Creates a transformation matrix for a sphere
 */
void csphere_make_transform(csphere_t sphere, mat4 transform);

/**
 * Generates vertices to render a given sphere.
 * Intended to be drawn with the sphere_* shaders
 */
void csphere_gen_vertices(csphere_t sphere, GLfloat vertices[CSPHERE_VERTEX_ARRAY_SIZE]);
