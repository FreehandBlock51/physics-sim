#ifndef SHADER_DEBUG_FRAGMENT
#define SHADER_DEBUG_FRAGMENT

static const char* shader_src_debug_fragment = ""
"#version 330 core\n"
"\n"
"out vec4 out_color;\n"
"\n"
"in vec4 debug;\n"
"\n"
"void main() {\n"
"out_color = debug;\n"
"}\n"
"";
#endif
