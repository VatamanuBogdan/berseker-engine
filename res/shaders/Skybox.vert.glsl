#version 330 core
layout (location = 0) in vec3 v_Position;

uniform mat4 Projection;
uniform mat4 View;

out vec3 f_TextureCoords;

void main() {
    gl_Position = Projection * View * vec4(v_Position, 1.0);
    f_TextureCoords = v_Position;
}