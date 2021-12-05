#version 420 core
uniform mat4 Projection;
uniform mat4 View;

out vec3 f_NearPoint;
out vec3 f_FarPoint;

vec3 vertices[6] = {
    vec3(-1.0f, 1.0f, 0.0f),
    vec3(1.0f, 1.0f, 0.0f),
    vec3(1.0f,-1.0f, 0.0f),
    vec3(1.0f,-1.0f, 0.0f),
    vec3(-1.0f,-1.0f, 0.0f),
    vec3(-1.0f, 1.0f, 0.0f)
};

vec3 Unproject(vec3 vertex) {
    vec4 unprojVertex = inverse(View) * inverse(Projection) * vec4(vertex, 1.0f);
    return unprojVertex.xyz / unprojVertex.w;
}

void main() {
    gl_Position = vec4(vertices[gl_VertexID], 1.0f);
    f_NearPoint = Unproject(vec3(gl_Position.xy, 0));
    f_FarPoint = Unproject(vec3(gl_Position.xy, 1.0f));
}