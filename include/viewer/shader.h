#pragma once
/**
 * Contains utilities for creating and managing shaders
 */

#include <cglm/call.h>
#include <gl_includes.h>
#include "common/defines.h"
#include "common/vec3.h"
#include "common/vec4.h"
#include "viewer/color.h"

/**
 * The max number of characters that will be collected from a shader or
 * program error.
 */
#define SHADER_MAX_LOG_SIZE 512

/**
 * A shader
 */
typedef GLuint shader_t;
/**
 * A shader type (vertex, fragment, geometry, etc.)
 */
typedef GLenum shader_kind_t;
/**
 * A shader program.  Links together the individual shaders and is used
 * to render shapes.
 */
typedef GLuint shader_program_t;
/**
 * The location of a uniform shader parameter.  Uniforms are set once
 * before rendering, as opposed to vertex values that can change between
 * vertices.
 */
typedef GLint shader_uniform_t;

/**
 * Creates and compiles a shader of the specified type from the given
 * source code.
 */
int shader_compile(shader_kind_t kind, const char *source, shader_t *dest);

/**
 * Creates a shader program.  This will connect the seperate shaders
 * into a single pipline
 */
shader_program_t shader_program_create();

/**
 * Links a shader to a given program
 */
void shader_program_attach_shader(shader_program_t program, shader_t shader);

/**
 * Finishes the linking process for a shader program
 */
int shader_program_link(shader_program_t program);

/**
 * Marks this program to be used when drawing
 */
void shader_program_use(shader_program_t program);

/**
 * Gets the location of a shader's uniform
 */
shader_uniform_t shader_uniform_get_location(shader_program_t program, const char *uniform_name);                   

/**
 * Deletes a shader.  Once a shader has been linked, it can be safely
 * deleted.
 */
void shader_delete(shader_t shader);
