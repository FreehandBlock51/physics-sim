#ifndef SHADER_MATRIX_VERTEX
#define SHADER_MATRIX_VERTEX

static const char* shader_src_matrix_vertex = ""
"#version 330 core\n"
"\n"
"layout(location=0) in vec3 in_position;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main() {\n"
"gl_Position = projection * view * model * vec4(in_position, 1.0f);\n"
"}\n"
"";
#endif
