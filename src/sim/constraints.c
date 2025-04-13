#include "sim/constraints.h"

#include "common/defines.h"

void spring_create(spring_t *spring, body_t *a, vec3_t a_endpoint, body_t *b, vec3_t b_endpoint, phy_real_t spring_constant, phy_real_t equilibrium_distance) {
    safe_assert(spring != NULL,);

    spring->a = a;
    spring->a_endpoint = a_endpoint;
    spring->b = b;
    spring->b_endpoint = b_endpoint;
    spring->spring_constant = spring_constant;
    spring->equilibrium_distance = equilibrium_distance;
}

void spring_apply_constraint(spring_t spring) {
    safe_assert(spring.a != NULL && spring.b != NULL,);

    // get the spring force on a, then negate to apply to b
    vec3_t spring_force_on_a = spring.b->position;
    vec3_add_to(&spring_force_on_a, spring.b_endpoint, 1);
    vec3_add_to(&spring_force_on_a, spring.a->position, -1);
    vec3_add_to(&spring_force_on_a, spring.a_endpoint, -1);

    // right now, spring_force_on_a stores the distance between the two
    // endpoints, so we can use its current magnitude to calculate the
    // spring force before normalizing
    phy_real_t current_distance = vec3_magnitude(spring_force_on_a);
    phy_real_t spring_force_magnitude = (current_distance - spring.equilibrium_distance) * spring.spring_constant;

    // now we can use the direction of the displacement as the direction
    // of the force
    vec3_unit(&spring_force_on_a);
    vec3_multiply_by(&spring_force_on_a, spring_force_magnitude);
    phy_body_add_force_and_torque(spring.a, spring_force_on_a, spring.a_endpoint);

    // Newton's 3rd law lets us reverse F_ab to get F_ba
    vec3_t spring_force_on_b = spring_force_on_a;
    vec3_multiply_by(&spring_force_on_b, -1);
    phy_body_add_force_and_torque(spring.b, spring_force_on_b, spring.b_endpoint);
}
