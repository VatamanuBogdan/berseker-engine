#version 330 core
in vec2 f_TextCoords;

uniform sampler2D u_Texture;

layout (location = 0) out vec4 color;

void main() {
    color = texture(u_Texture, f_TextCoords);
    if (color.a < 1.0f)
        discard;
}