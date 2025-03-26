#pragma once
/**
 * Definitions and utility functions for a spherical collider
 */

#include <stddef.h>
#include <stdbool.h>
#include "common/vec3.h"
#include "sim/aabb.h"

struct SphereCollider {
    vec3_t center;
    phy_real_t radius;
};
typedef struct SphereCollider csphere_t;

/**
 * Creates a new sphere with the specified radius
 * centered at a given point
 */
#define csphere_make(center, radius) ((csphere_t){ .center = center, .radius = radius })

/**
 * Checks if a point is inside the given sphere
 */
bool csphere_is_point_inside(csphere_t sphere, vec3_t point);

/**
 * Checks if two spheres are overlapping
 */
bool csphere_is_csphere_inside(csphere_t a, csphere_t b);

/**
 * Checks if a sphere and an Axis-Aligned Bounding Box (AABB) are
 * overlapping
 */
bool csphere_is_bbox_inside(csphere_t sphere, bbox_t box);

/**
 * Given a contact point on the surface of a sphere, gets the normal
 * of that point
 */
vec3_t csphere_get_surface_normal(csphere_t sphere, vec3_t point_on_surface);
