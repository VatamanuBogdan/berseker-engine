#version 330 core

in vec3 f_TextureCoords;

uniform samplerCube u_CubeMapTexture;

layout (location = 0) out vec4 color;

void main() {
    color = texture(u_CubeMapTexture, f_TextureCoords);
}