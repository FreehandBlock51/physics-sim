#ifndef SHADER_BASIC_VERTEX
#define SHADER_BASIC_VERTEX

static const char* shader_src_basic_vertex = ""
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 in_position;\n"
"\n"
"void main() {\n"
"gl_Position = vec4(in_position, 1.0f);\n"
"}\n"
"";
#endif
