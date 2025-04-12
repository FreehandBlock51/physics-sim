#include "sim/aabb.h"

#include "common/math.h"


/** makes a bounding box centered at (x, y, z)
 * with size width along the x-axis,
 *      size height along the y-axis,
 *  and size length along the z-axis
 */
void bbox_make(bbox_t *box, phy_real_t x, phy_real_t y, phy_real_t z, phy_real_t length, phy_real_t width, phy_real_t height) {
    box->position.x = x;
    box->position.y = y;
    box->position.z = z;
    box->front = z + length / 2;
    box->back = z - length / 2;
    box->left = x - width / 2;
    box->right = x + width / 2;
    box->top = y + height / 2;
    box->bottom = y - height / 2;
}

bool bbox_is_point_inside(bbox_t box, vec3_t point) {
    vec3_add_to(&point, box.position, -1);
    return
        point.x >= box.left && point.x <= box.right &&
        point.y >= box.bottom && point.y <= box.top &&
        point.z >= box.back && point.z <= box.front;
}

bool bbox_is_bbox_inside(bbox_t boxA, bbox_t boxB) {
    return
        (boxA.left + boxA.position.x) <= (boxB.right + boxB.position.x) &&
        (boxA.right + boxA.position.x) >= (boxB.left + boxB.position.x) &&
        (boxA.bottom + boxA.position.y) <= (boxB.top + boxB.position.y) &&
        (boxA.top + boxA.position.y) >= (boxB.bottom + boxB.position.y) &&
        (boxA.back + boxA.position.z) <= (boxB.front + boxB.position.z) &&
        (boxA.front + boxA.position.z) >= (boxB.back + boxB.position.z);
}

void bbox_clamp_point_within_bounds(bbox_t box, vec3_t *point) {
    point->x = clamp(point->x, box.position.x + box.left, box.position.x + box.right);
    point->y = clamp(point->y, box.position.y + box.bottom, box.position.y + box.top);
    point->z = clamp(point->z, box.position.z + box.back, box.position.z + box.front);
}

phy_real_t bbox_get_volume(bbox_t box) {
    phy_real_t length = box.right - box.left;
    phy_real_t width = box.front - box.back;
    phy_real_t height = box.top - box.bottom;
    phy_real_t volume = length * width * height;
    if (volume < 0) {
        return -volume;
    }
    return volume;
}

/**
 * Gets the surface normal, given a vector
 * clamped within a surface of the bounding box
 * and relative to its center
 */
PRIVATE_FUNC vec3_t bbox_get_surface_normal_clamped_relative(bbox_t box, vec3_t point_on_surface) {
    // normalize all distances to make comparison possible
    phy_real_t dist_to_top = (box.top - point_on_surface.y) / (box.top - box.bottom);
    phy_real_t dist_to_bottom = (point_on_surface.y - box.bottom) / (box.top - box.bottom);
    phy_real_t dist_to_left = (point_on_surface.x - box.left) / (box.right - box.left);
    phy_real_t dist_to_right = (box.right - point_on_surface.x) / (box.right - box.left);
    phy_real_t dist_to_front = (box.front - point_on_surface.z) / (box.front - box.back);
    phy_real_t dist_to_back = (point_on_surface.z - box.back) / (box.front - box.back);

    phy_real_t min_x_distance = min(dist_to_left, dist_to_right);
    phy_real_t min_y_distance = min(dist_to_top, dist_to_bottom);
    phy_real_t min_z_distance = min(dist_to_front, dist_to_back);

    phy_real_t min_distance = min(min(min_x_distance, min_y_distance), min_z_distance);

    vec3_t normal = VEC3_ZERO;

    // use ifs instead of switches b/c we're not checking constants
    if (min_distance == dist_to_top) {
        normal.y += 1;
    }
    if (min_distance == dist_to_bottom) {
        normal.y -= 1;
    }
    if (min_distance == dist_to_left) {
        normal.x -= 1;
    }
    if (min_distance == dist_to_right) {
        normal.x += 1;
    }
    if (min_distance == dist_to_front) {
        normal.z += 1;
    }
    if (min_distance == dist_to_back) {
        normal.z -= 1;
    }

    return normal;
}

vec3_t bbox_get_surface_normal(bbox_t box, vec3_t point_on_surface) {
    vec3_t clamped_point = point_on_surface;
    bbox_clamp_point_within_bounds(box, &clamped_point);
    vec3_add_to(&clamped_point, box.position, -1);

    return bbox_get_surface_normal_clamped_relative(box, clamped_point);
}
