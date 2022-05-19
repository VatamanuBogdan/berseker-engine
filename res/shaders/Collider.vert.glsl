#version 330 core
layout (location = 0) in vec3 v_Position;
layout (location = 1) in int v_ComponentIndex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 f_Color;

const vec3 Colors[3] = vec3[3](
    vec3(1, 0, 0),
    vec3(0, 1, 0),
    vec3(0, 0, 1)
);

vec3 rand(int index){
    return vec3(
            fract(sin(index * 12.9898) * 43758.5453),
            fract(sin(index * 18.435498) * 143548.653),
            fract(sin(index * 43.95498) * 65424432.542342)
    );
}

void main() {
    gl_Position = Projection * View * Model * vec4(v_Position, 1.0f);
    f_Color = vec4(Colors[v_ComponentIndex % 3], 1.0f);
}