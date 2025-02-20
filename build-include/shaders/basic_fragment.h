#ifndef SHADER_BASIC_FRAGMENT
#define SHADER_BASIC_FRAGMENT

static const char* shader_src_basic_fragment = ""
"#version 330 core\n"
"\n"
"out vec4 color;\n"
"\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n"
"";
#endif
