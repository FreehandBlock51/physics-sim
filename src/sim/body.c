#include "sim/body.h"

#include "common/defines.h"

#define PHY_GRAVITATIONAL_CONSTANT 1 /* 6.67430E-11 */

void body_make(body_t *body,
 vec3_t position, vec3_t rotation,
 vec3_t velocity, vec3_t angular_velocity,
 phy_real_t mass,
 phy_real_t static_friction, phy_real_t kinetic_friction)
{
    safe_assert(body != NULL,);

    body->position = position;
    body->rotation = rotation;
    body->velocity = velocity;
    body->angular_velocity = angular_velocity;
    body->mass = mass;
    body->static_friction = static_friction;
    body->kinetic_friction = kinetic_friction;
    vec3_clear(&body->net_force);
    vec3_clear(&body->net_torque);
}

void phy_body_add_force(body_t *body, vec3_t force) {
    safe_assert(body != NULL,);

    vec3_add_to(&body->net_force, force, 1.0);
}

void phy_body_add_torque(body_t *body, vec3_t torque) {
    safe_assert(body != NULL,);

    vec3_add_to(&body->net_torque, torque, 1.0);
}

void phy_body_add_force_and_torque(body_t *body, vec3_t force, vec3_t applied_at) {
    safe_assert(body != NULL,);

    // apply force; nothing extra needs to be done here
    phy_body_add_force(body, force);

    // apply torque; torque = radius (to center of mass) x force
    vec3_t torque;
    vec3_cross_product(&torque, applied_at, force);
    phy_body_add_torque(body, torque);
}

void phy_body_add_gravity_force(body_t *a, body_t *b) {
    safe_assert(a != NULL && b != NULL,);

    phy_real_t gravity =
        (PHY_GRAVITATIONAL_CONSTANT * a->mass * b->mass) /
        vec3_distance_sqr(a->position, b->position);

    vec3_t force_a_b = b->position;
    vec3_add_to(&force_a_b, a->position, -1.0);
    vec3_unit(&force_a_b);
    vec3_multiply_by(&force_a_b, gravity);

    vec3_t force_b_a = force_a_b;
    vec3_multiply_by(&force_b_a, -1.0);

    phy_body_add_force(a, force_a_b);
    phy_body_add_force(b, force_b_a);
}

/**
 * Given a normal force by a on b,
 * adds collision-based forces to just a
 * (normal, friction, etc.)
 * This function DOES NOT affect b
 */
PRIVATE_FUNC void phy_body_add_collision_forces_to_a(body_t *a, const body_t *b, vec3_t normal_force, vec3_t contact_point) {
    safe_assert(a != NULL && b != NULL,);

    // add normal force
    phy_body_add_force_and_torque(a, normal_force, contact_point);

    // add friction
    phy_real_t normal_magnitude = vec3_magnitude(normal_force);
    if (vec3_magnitude(a->velocity) < PHYSICS_EPSILON) {
        // object is essentially at rest; use static friction

        // get portion of net force that will oppose friction
        // since friction is perpendicular to the normal,
        // we can remove the normal to get what we want
        vec3_t friction_opposed = a->net_force;
        vec3_t normal_opposed;
        vec3_get_portion_in_direction(&normal_opposed, a->net_force, normal_force);
        vec3_add_to(&friction_opposed, normal_opposed, -1);

        // calculate friction magnitude
        phy_real_t friction_coeff = a->static_friction * b->static_friction;
        phy_real_t friction_magnitude = friction_coeff * normal_magnitude;

        // static friction is all-or-nothing;
        // it either negates all acceleration or does nothing
        if (vec3_magnitude(friction_opposed) <= friction_magnitude) {
            vec3_t static_friction = friction_opposed;
            vec3_multiply_by(&friction_opposed, -1);
            phy_body_add_force_and_torque(a, static_friction, contact_point);
        }
    }
    else {
        // object is moving; use kinetic friction

        // kinetic friction opposes motion
        vec3_t friction = a->velocity;
        vec3_unit(&friction);
        vec3_multiply_by(&friction, -1);

        // calculate friction magnitude
        phy_real_t friction_coeff = a->kinetic_friction * b->kinetic_friction;
        phy_real_t friction_magnitude = friction_coeff * normal_magnitude;
        vec3_multiply_by(&friction, friction_magnitude);

        phy_body_add_force_and_torque(a, friction, contact_point);
    }
}

/**
 * Given a normal force by a on b,
 * adds collision-based forces on both a and b
 * (normal, friction, etc.)
 */
void phy_body_add_collision_forces(body_t *a, body_t *b, vec3_t normal_force, vec3_t contact_point) {
    safe_assert(a != NULL && b != NULL,);

    phy_body_add_collision_forces_to_a(a, b, normal_force, contact_point);

    // Newton says each force on an object has an
    // equal and opposite partner on the other object
    // so we just invert the normal force and do everything
    // on b this time
    vec3_multiply_by(&normal_force, -1);
    phy_body_add_collision_forces_to_a(b, a, normal_force, contact_point);
}

/**
 * Given bodies a and b, as well as the
 * direction of the normal force on a by b,
 * calculates the normal force on a by b
 */
void phy_calculate_normal_force(vec3_t *normal_a_b, body_t a, vec3_t normal_a_b_dir) {
    safe_assert(normal_a_b != NULL,);

    // the velocity and force opposed by the normal
    vec3_t opposed_velocity;
    vec3_get_portion_in_direction(&opposed_velocity, a.velocity, normal_a_b_dir);
    vec3_t opposed_force;
    vec3_get_portion_in_direction(&opposed_force, a.net_force, normal_a_b_dir);

    // only oppose velocity & force bringing
    // the colliding bodies together
    if (vec3_dot_product(opposed_velocity, normal_a_b_dir) < 0) {
        // normal and velocity are in opposite directions, which means
        // this body is not moving towards the collision; apply no force
        vec3_clear(&opposed_velocity);
    }
    if (vec3_dot_product(opposed_force, normal_a_b_dir) < 0) {
        // normal and force are in opposite directions, which means
        // this body will not accelerate towards the collision; apply no force
        vec3_clear(&opposed_force);
    }

    // calculate the normal force.
    // this should be enough to both negate
    // all velocity and force bring the
    // bodies together in a single step
    *normal_a_b = opposed_velocity;
    vec3_multiply_by(normal_a_b, -a.mass);
    vec3_add_to(normal_a_b, opposed_force, -1);
}

void phy_body_add_drag_force(body_t *body, phy_real_t drag_coefficient) {
    vec3_t drag_force = body->velocity;
    vec3_multiply_by(&drag_force, -drag_coefficient);
    phy_body_add_force(body, drag_force);

    vec3_t drag_torque = body->angular_velocity;
    vec3_multiply_by(&drag_torque, -drag_coefficient);
    phy_body_add_torque(body, drag_torque);
}

/**
 * Applies all forces and torques on a body
 * over a single 'tick.'  This resets force
 * and torque; both should be applied every
 * 'tick' they are active
 */
void phy_body_step(body_t *body) {
    safe_assert(body != NULL,);

    vec3_add_to(&body->velocity, body->net_force, 1.0/body->mass);
    vec3_clear(&body->net_force);
    vec3_add_to(&body->position, body->velocity, 1.0);
    vec3_add_to(&body->angular_velocity, body->net_torque, 1.0/body->mass);
    vec3_clear(&body->net_torque);
    vec3_add_to(&body->rotation, body->angular_velocity, 1.0);
}
