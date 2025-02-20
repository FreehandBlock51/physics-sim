#include "sim/cube.h"

#include "common/defines.h"
#include "common/math.h"

/**
 * Given a vector in world space, translates and rotates it
 * so that it is in the cube's model space.
 * Allows the point to treat the cube like an AABB centered at
 * the origin.
 */
PRIVATE_FUNC void ccube_apply_cube_transformations(ccube_t cube, vec3_t *point) {
    // transform point so that it's relative to cube position
    vec3_add_to(point, cube.position, -1);
    // now that it's relative to the cube, we can rotate it
    vec3_rotate_by_quaternion(point, *point, cube.rotation);
}

/**
 * Given a vector in the cube's model space, translates and rotates it
 * so that it is in world space.
 */
PRIVATE_FUNC void ccube_undo_cube_transformations(ccube_t cube, vec3_t *point) {
    // undo rotation first
    quaternion_t inverse = cube.rotation;
    quaternion_conjugate(&inverse); // take the inverse to cancel out the cube's rotation
    vec3_rotate_by_quaternion(point, *point, inverse);

    // next, undo transformation
    vec3_add_to(point, cube.position, 1);
}

bool ccube_is_point_inside(ccube_t cube, vec3_t point) {
    ccube_apply_cube_transformations(cube, &point);
    // at this point, we should be able run the check as if
    // our cube is axis-aligned and centered at the origin
    phy_real_t halflength = cube.length / 2;
    phy_real_t halfwidth = cube.width / 2;
    phy_real_t halfheight = cube.height / 2;
    return
        fabs(point.x) <= halfwidth &&
        fabs(point.y) <= halfheight &&
        fabs(point.z) <= halflength;
}

void ccube_clamp_point_within_cube(ccube_t cube, vec3_t *point) {
    ccube_apply_cube_transformations(cube, point);
    phy_real_t halflength = cube.length / 2;
    phy_real_t halfwidth = cube.width / 2;
    phy_real_t halfheight = cube.height / 2;
    point->x = clamp(point->x, -halfwidth, halfwidth);
    point->y = clamp(point->y, -halfheight, halfheight);
    point->z = clamp(point->z, -halflength, halflength);
    ccube_undo_cube_transformations(cube, point);
}

bool ccube_is_bbox_inside(ccube_t cube, bbox_t box) {
    // check if the closest point on the AABB is inside the cube
    vec3_t closest_to_cube_center = cube.position;
    bbox_clamp_point_within_bounds(box, &closest_to_cube_center);
    return ccube_is_point_inside(cube, closest_to_cube_center);
}


bool ccube_is_sphere_inside(ccube_t cube, csphere_t sphere) {
    // check if the point on the sphere closest to the cube's
    // center is inside the cube
    vec3_t closest_to_cube_center = cube.position;
    ccube_clamp_point_within_cube(cube, &closest_to_cube_center);
    return csphere_is_point_inside(sphere, closest_to_cube_center);
}
