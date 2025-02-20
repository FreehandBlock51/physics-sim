#pragma once

/**
 * Controls how an Axis-Aligned Bounding Box (AABB) is rendered
 */

#include <gl_includes.h>
#include <cglm/call.h>
#include "common/defines.h"
#include "sim/aabb.h"

/**
 * The minimum size a vertex array has to be to contain all
 * the vertices in a bounding box
 */
#define BBOX_VERTEX_ARRAY_SIZE 24

/**
 * The minimum size an index array has to be to contain all
 * the indices in a bounding box
 */
#define BBOX_INDEX_ARRAY_SIZE 36

/**
 * Creates a transformation matrix for a bounding box
 */
void bbox_make_transform(bbox_t box, mat4 transform);

/**
 * Generates both the vertex array and index array for
 * a bounding box.  The vertex array must have as much
 * space as required by BBOX_VERTEX_ARRAY_SIZE, and the
 * index array must have as much space as required by
 * BBOX_INDEX_ARRAY_SIZE
 */
void bbox_gen_vertices(bbox_t box, GLfloat *vertices, GLint *indices);
