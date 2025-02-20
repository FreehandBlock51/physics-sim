#include "sim/sphere.h"

#include "common/math.h"

bool csphere_is_point_inside(csphere_t sphere, vec3_t point) {
    vec3_add_to(&point, sphere.center, -1);
    return
        vec3_magnitude_sqr(point) <= sphere.radius * sphere.radius;
}

bool csphere_is_csphere_inside(csphere_t a, csphere_t b) {
    phy_real_t radii_sum = a.radius + b.radius;
    return
        vec3_distance_sqr(a.center, b.center) <= radii_sum * radii_sum;
}

bool csphere_is_bbox_inside(csphere_t sphere, bbox_t box) {
    vec3_t closest_box_point = sphere.center;
    bbox_clamp_point_within_bounds(box, &closest_box_point);
    return csphere_is_point_inside(sphere, closest_box_point);
}
