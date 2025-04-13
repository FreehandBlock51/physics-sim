#pragma once
/**
 * Definitions and utility functions for a cubic collider
 */

#include <stddef.h>
#include <stdbool.h>
#include "common/vec3.h"
#include "common/vec4.h"
#include "sim/aabb.h"
#include "sim/sphere.h"


struct CubeCollider {
    vec3_t position; // the position of the cube's center
    quaternion_t rotation;
    phy_real_t length;
    phy_real_t width;
    phy_real_t height;
};
typedef struct CubeCollider ccube_t;

/**
 * Creates a cube with the given length, width, height, and rotation,
 * centered at the provided position
 */
#define ccube_make(_position, _rotation, _length, _width, _height) \
    ((ccube_t){ .position = _position, .rotation = _rotation, .length = _length, .width = _width, .height = _height })

/**
 * Checks if a point is inside the given cube
 */
bool ccube_is_point_inside(ccube_t cube, vec3_t point);

/**
 * Clamps a point within the bounds of the given cube
 */
void ccube_clamp_point_within_cube(ccube_t cube, vec3_t *point);

/**
 * Checks if an Axis-Aligned Bounding Box (AABB) and a cube are
 * overlapping
 */
bool ccube_is_bbox_inside(ccube_t cube, bbox_t box);

/**
 * Checks if a sphere and a cube are overlapping
 */
bool ccube_is_sphere_inside(ccube_t cube, csphere_t sphere);

/**
 * Checks if two cubes are overlapping
 */
bool ccube_is_ccube_inside(ccube_t a, ccube_t b);
