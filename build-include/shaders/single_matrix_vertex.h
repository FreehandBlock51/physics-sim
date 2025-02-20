#ifndef SHADER_SINGLE_MATRIX_VERTEX
#define SHADER_SINGLE_MATRIX_VERTEX

static const char* shader_src_single_matrix_vertex = ""
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 in_position;\n"
"\n"
"out vec4 debug;\n"
"\n"
"uniform mat4 transform;\n"
"\n"
"void main() {\n"
"gl_Position = transform * vec4(in_position, 1.0f);\n"
"}\n"
"";
#endif
