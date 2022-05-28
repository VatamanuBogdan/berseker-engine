#version 330 core

in vec3 f_Position;
in vec3 f_Normal;

uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

layout (location = 0) out vec4 color;

const float ambientStrength = 0.1;
const float specularStrength = 0.6f;

const vec3 ambientColor = vec3(1, 1, 1);


vec3 AmbientLight() {
    return ambientStrength * ambientColor;
}

vec3 DiffuseLight(vec3 lightColor, vec3 normal, vec3 ligthDirection) {
    return max(dot(normal, ligthDirection), 0.0) * lightColor;
}

vec3 SpecularLight(vec3 lightColor, vec3 normal, vec3 ligthDirection) {
    vec3 viewDirection = normalize(u_CameraPosition - f_Position);
    vec3 reflectionDirection = reflect(-ligthDirection, normal);

    return specularStrength * pow(max(dot(viewDirection, reflectionDirection), 0.0), 32) * lightColor;
}

void main() {
    vec3 normal = normalize(f_Normal);
    vec3 lightDirection = normalize(u_LightPosition - f_Position);

    vec3 ambientLight = AmbientLight();
    vec3 diffuseLight = DiffuseLight(ambientColor, normal, lightDirection);
    vec3 specularLight = SpecularLight(ambientColor, normal, lightDirection);


    color = vec4((ambientLight + diffuseLight + specularLight) * vec3(0.8, 0.8, 0.8), 1.0);
}