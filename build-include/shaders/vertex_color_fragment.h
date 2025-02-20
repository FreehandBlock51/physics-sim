#ifndef SHADER_VERTEX_COLOR_FRAGMENT
#define SHADER_VERTEX_COLOR_FRAGMENT

static const char* shader_src_vertex_color_fragment = ""
"#version 330 core\n"
"\n"
"in vec4 frag_color;\n"
"\n"
"out vec4 out_color;\n"
"\n"
"void main() {\n"
"out_color = frag_color;\n"
"}\n"
"";
#endif
