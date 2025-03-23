#pragma once
/**
 * Definitions and utilities for the Axis-Aligned Bounding Box
 */

#include <stddef.h>
#include <stdbool.h>
#include "common/vec3.h"

/**
 * An Axis-Aligned Bounding Box (AABB).
 * Represents a rectanguluar prism with all faces parallel
 * to an axis.  Used for fast collision detection
 */
struct BoundingBox {
    /**
     * The box's position.  Not necessarily its center.
     * All of the box's bounds (front, back, etc.) are
     * relative to this value.
     */
    vec3_t position;
    /**
     * The largest z value still within the box.
     * Relative to the box's position.
     */
    phy_real_t front;
    /**
     * The smallest z value still within the box.
     * Relative to the box's position.
     */
    phy_real_t back;
    /**
     * The smallest x value still within the box.
     * Relative to the box's position.
     */
    phy_real_t left;
    /**
     * The largest x value still within the box.
     * Relative to the box's position.
     */
    phy_real_t right;
    /**
     * The largest y value still within the box.
     * Relative to the box's position.
     */
    phy_real_t top;
    /**
     * The smallest y value still within the box.
     * Relative to the box's position.
     */
    phy_real_t bottom;
};
typedef struct BoundingBox bbox_t;

/** makes a bounding box centered at (x, y, z)
 * with size width along the x-axis,
 *      size height along the y-axis,
 *  and size length along the z-axis
 */
void bbox_make(bbox_t *box, phy_real_t x, phy_real_t y, phy_real_t z, phy_real_t length, phy_real_t width, phy_real_t height);

/**
 * Checks if a point is inside the given AABB
 */
bool bbox_is_point_inside(bbox_t box, vec3_t point);

/**
 * Checks if two AABBs are overlapping
 */
bool bbox_is_bbox_inside(bbox_t boxA, bbox_t boxB);

/**
 * 'Clamps' a point to the closest point within the bounds of the AABB
 */
void bbox_clamp_point_within_bounds(bbox_t box, vec3_t *point);

/**
 * @brief Calculates the volume of an AABB
 * @param box The AABB to use
 * @return The AABB's volume
 */
phy_real_t bbox_get_volume(bbox_t box);

/**
 * @brief Gets the normal of one of a bounding box's surfaces
 * @param point_on_surface A point on the surface to get the normal of
 * @return The normal vector, which is purpendicular to the given surface
 */
vec3_t bbox_get_surface_normal(bbox_t box, vec3_t point_on_surface);
