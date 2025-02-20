#ifndef SHADER_UNIFORM_COLOR_FRAGMENT
#define SHADER_UNIFORM_COLOR_FRAGMENT

static const char* shader_src_uniform_color_fragment = ""
"#version 330 core\n"
"\n"
"out vec4 out_color;\n"
"\n"
"uniform vec4 color;\n"
"\n"
"void main() {\n"
"out_color = color;\n"
"}\n"
"";
#endif
