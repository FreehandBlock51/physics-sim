#pragma once

/**
 * Defines utilities for rendering objects
 * controlled by the physics engine
 */

#include <cglm/cglm.h>
#include "sim/body.h"

void body_gen_transform(body_t *body, mat4 transform_matrix);
