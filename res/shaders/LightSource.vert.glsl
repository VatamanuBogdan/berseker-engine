#version 330 core
layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 f_TextCoords;

void main() {
    vec3 cameraRight = vec3(View[0][0], View[1][0], View[2][0]);
    vec3 cameraUp = vec3(View[0][1], View[1][1], View[2][1]);

    gl_Position = Projection * View * Model * vec4(cameraRight * v_Position.x + cameraUp * v_Position.y, 1.0f);
    f_TextCoords = v_TexCoords;
}