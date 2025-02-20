#include "viewer/shader.h"

#include <stdio.h>

int shader_compile(shader_kind_t kind, const char *source, shader_t *dest) {
    int compile_result;

#ifndef NO_LOGGING
    // static so it isn't allocated every call
        static char info_log[SHADER_MAX_LOG_SIZE];
#endif

    shader_t shader = glCreateShader(kind);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

    if(!compile_result)
    {
#ifndef NO_LOGGING
        glGetShaderInfoLog(shader, SHADER_MAX_LOG_SIZE, NULL, info_log);
        l_printf("Fatal: couldn't compile shader (gl returned %d): %s\n", compile_result, info_log);
#endif
        return -1;
    }

    *dest = shader;

    return 0;
}

/**
 * Creates a shader program.  This will connect the seperate shaders
 * into a single pipline
 */
shader_program_t shader_program_create() {
    return glCreateProgram();
}

/**
 * Links a shader to a given program
 */
void shader_program_attach_shader(shader_program_t program, shader_t shader) {
    glAttachShader(program, shader);
}

/**
 * Finishes the linking process for a shader program
 */
int shader_program_link(shader_program_t program) {
    int link_result;

    // static so it isn't allocated every call
#ifndef NO_LOGGING
    static char info_log[SHADER_MAX_LOG_SIZE];
#endif

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if(!link_result) {
#ifndef NO_LOGGING
        glGetProgramInfoLog(program, 512, NULL, info_log);
        l_printf("Fatal: couldn't link shader program: %s\n", info_log);
#endif
        return -1;
    }

    return 0;
}

void shader_program_use(shader_program_t program) {
    glUseProgram(program);
}

shader_uniform_t shader_uniform_get_location(shader_program_t program, const char *uniform_name) {
    return glGetUniformLocation(program, uniform_name);
}

void shader_delete(shader_t shader) {
    glDeleteShader(shader);
}
