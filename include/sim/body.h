#pragma once
/**
 * Definitions and utility functions for a rigidbody/particle
 */

 #include "common/vec3.h"

/**
 * Represents a dynamic element of the physics engine--something that
 * can move and interact with forces
 */
struct Body {
    vec3_t position;
    vec3_t rotation;
    vec3_t velocity;
    vec3_t angular_velocity;
    phy_real_t mass;
    phy_real_t static_friction;
    phy_real_t kinetic_friction;
    vec3_t net_force;
    vec3_t net_torque;
};
typedef struct Body body_t;

/**
 * Constructs a body from its component parts
 */
void body_make(body_t *body,
 vec3_t position, vec3_t rotation,
 vec3_t velocity, vec3_t angular_velocity,
 phy_real_t mass,
 phy_real_t static_friction, phy_real_t kinetic_friction);

/**
 * Adds a force to the body.
 * Forces must be added every physics step they are affecting the body
 */
void phy_body_add_force(body_t *body, vec3_t force);

/**
 * Adds a torque to the body.
 * Torques must be added every physics step they are affecting the body
 */
void phy_body_add_torque(body_t *body, vec3_t torque);

/**
 * Applies a force and a torque, given the force and where it is applied
 * (relative to the body's position).
 * Both forces and torques must be added every physics step they are
 * affecting the body.
 */
void phy_body_add_force_and_torque(body_t *body, vec3_t force, vec3_t applied_at);

/**
 * Calculates the force of gravity between two bodies, then adds that
 * force to both of them
 */
void phy_body_add_gravity_force(body_t *a, body_t *b);

/**
 * Given a normal force by a on b,
 * adds collision-based forces on both a and b
 * (normal, friction, etc.)
 */
void phy_body_add_collision_forces(body_t *a, body_t *b, vec3_t normal_force, vec3_t contact_point);

/**
 * Given bodies a and b, as well as the
 * direction of the normal force on a by b,
 * calculates the normal force on a by b
 */
void phy_calculate_normal_force(vec3_t *normal_a_b, body_t a, vec3_t normal_a_b_dir);

/**
 * Applies all forces and torques on a body
 * over a single 'step.'  This resets force
 * and torque; both should be applied every
 * step they are active
 */
void phy_body_step(body_t *body);
