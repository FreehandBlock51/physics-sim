#ifndef SHADER_VERTEX_COLOR_VERTEX
#define SHADER_VERTEX_COLOR_VERTEX

static const char* shader_src_vertex_color_vertex = ""
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 in_position;\n"
"layout(location = 1) in vec3 in_color;\n"
"\n"
"out vec3 frag_color;\n"
"\n"
"void main() {\n"
"gl_Position = vec4(in_position, 1.0f);\n"
"frag_color = in_color;\n"
"}\n"
"";
#endif
