#version 420 core
in vec3 f_NearPoint;
in vec3 f_FarPoint;

uniform mat4 Projection;
uniform mat4 View;

float computeLinearDepth(vec3 pos) {
    float near = 0.1f;
    float far = 100.0f;

    vec4 clip_space_pos = Projection * View * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

layout (location = 0) out vec4 color;

void main() {
    float t = f_NearPoint.y / (f_NearPoint.y - f_FarPoint.y);
    vec3 f_Position = f_NearPoint + t * (f_FarPoint - f_NearPoint);
    vec2 coord = f_Position.xz;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);

    color = vec4(0.2, 0.2, 0.2, (1.0 - line)) * float(t > 0);

    vec4 clip_coord = Projection * View * vec4(f_Position.xyz, 1.0f);
    gl_FragDepth = clip_coord.z / clip_coord.w;

    float linearDepth = computeLinearDepth(f_Position);
    float fading = max(0, (0.5 - linearDepth));

    color.a *= fading;
}