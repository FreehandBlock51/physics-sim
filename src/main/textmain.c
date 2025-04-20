/**
 * The entry point for the simulation -- text mode
 */

#include <stdio.h>
#include "common/defines.h"
#include "common/vec3.h"
#include "sim/body.h"
#include "sim/aabb.h"


 /**
  * [textmode] How many steps should the simulation run for?
  */
 #ifndef STEPS
 #define STEPS 255
 #endif

 int text_main(void) {
     body_t a, b;
     bbox_t a_box, b_box;
     body_make(&a,
         VEC3_ZERO, VEC3_ZERO,
         VEC3_ZERO, VEC3_ZERO,
         350.0,
         0.0, 0.0);
     bbox_make(&a_box, 0, 0, 0, 2, 3, 5);
     body_make(&b,
         vec3_make(10.0,50.0,30.0), VEC3_ZERO,
         vec3_make(1.0,3.0,-6.0), VEC3_ZERO,
         5.0,
         0.0, 0.0);
     bbox_make(&b_box, 0, 0, 0, 1, 1, 1);

     for (int i = 0; i < STEPS; i++) {
         a_box.position = a.position;
         b_box.position = b.position;
         phy_body_add_gravity_force(&a, &b);
 #ifndef NOCOLLISION
         if (bbox_is_bbox_inside(a_box, b_box)) {
             // a and b are colliding; find normal
             // and do collision forces
             vec3_t b_closest_a = b_box.position;
             bbox_clamp_point_within_bounds(a_box, &b_closest_a);
             vec3_t normal_a_b = bbox_get_surface_normal(a_box, b_closest_a);
             phy_calculate_normal_force(&normal_a_b, a, normal_a_b);
             phy_body_add_collision_forces(&a, &b, normal_a_b, b_closest_a);
         }
 #endif

         phy_body_step(&a, 1.0);
         phy_body_step(&b, 1.0);
         printf("a: (%f, %f, %f); b: (%f, %f, %f)\n",
             a.position.x, a.position.y, a.position.z,
             b.position.x, b.position.y, b.position.z);
     }
     printf("da: (%f, %f, %f); db: (%f, %f, %f)\n",
         a.position.x, a.position.y, a.position.z,
         b.position.x - 10.0, b.position.y - 50.0, b.position.z - 30.0);
     return 0;
 }
