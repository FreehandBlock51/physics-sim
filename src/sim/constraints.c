#include "sim/constraints.h"

#include "common/defines.h"

void spring_create(spring_t *spring, body_t *a, vec3_t a_endpoint, body_t *b, vec3_t b_endpoint, phy_real_t spring_constant) {
    safe_assert(spring != NULL,);

    spring->a = a;
    spring->a_endpoint = a_endpoint;
    spring->b = b;
    spring->b_endpoint = b_endpoint;
    spring->spring_constant = spring_constant;
}

void spring_apply_constraint(spring_t spring) {
    safe_assert(spring.a != NULL && spring.b != NULL,);

    // get the spring force on a, then negate to apply to b
    vec3_t spring_force_on_a = spring.b->position;
    vec3_add_to(&spring_force_on_a, spring.a->position, 1);
    vec3_multiply_by(&spring_force_on_a, spring.spring_constant);
    phy_body_add_force_and_torque(spring.a, spring_force_on_a, spring.a_endpoint);

    vec3_t spring_force_on_b = spring_force_on_a;
    vec3_multiply_by(&spring_force_on_b, -1);
    phy_body_add_force_and_torque(spring.b, spring_force_on_b, spring.b_endpoint);
}
