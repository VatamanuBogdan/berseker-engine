#version 330 core
layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 f_Position;
out vec3 f_Normal;

void main() {
    gl_Position = Projection * View * Model * vec4(v_Position, 1.0f);

    f_Position = vec3(Model * vec4(v_Position, 1.0f));
    f_Normal = mat3(transpose(inverse(Model))) * v_Normal;
}