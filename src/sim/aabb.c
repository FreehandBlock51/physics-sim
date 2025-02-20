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
