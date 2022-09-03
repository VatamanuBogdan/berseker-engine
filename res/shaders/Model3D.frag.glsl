#version 330 core

in vec3 f_Normal;

layout (location = 0) out vec4 color;

void main() {



    color = vec4(abs(f_Normal), 1.0);
}