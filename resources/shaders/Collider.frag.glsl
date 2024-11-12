#version 330 core

layout (location = 0) out vec4 color;

uniform vec3 u_Color;

in vec4 f_Color;

void main() {
    color = vec4(u_Color, 1);
}