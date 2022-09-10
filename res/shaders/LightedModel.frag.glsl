#version 330 core

struct LightSource {
    vec3    Position;
    vec3    Ambient;
    vec3    Diffuse;
    vec3    Specular;
};

struct Material {
    vec3    Ambient;
    vec3    Diffuse;
    vec3    Specular;
    float   Shininess;
};

uniform Material    u_Material;

#define MAX_LIGHT_SOURCE 16

uniform LightSource u_LightSources[MAX_LIGHT_SOURCE];
uniform int u_LightsNum;

in vec3 f_Position;
in vec3 f_Normal;

uniform vec3 u_CameraPosition;

layout (location = 0) out vec4 color;

struct PhongModel {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

PhongModel ComputePhongModelFor(LightSource lightSource, Material material) {
    vec3 normal = normalize(f_Normal);
    vec3 viewDirection = normalize(u_CameraPosition - f_Position);
    vec3 lightDirection = normalize(lightSource.Position - f_Position);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    vec3 ambientLightComponent = lightSource.Ambient * material.Ambient;
    vec3 diffuseLightComponent = max(dot(normal, lightDirection), 0.0) * lightSource.Diffuse * material.Diffuse;
    vec3 specularLightComponent = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.Shininess) * lightSource.Specular * material.Specular;

    return PhongModel(ambientLightComponent, diffuseLightComponent, specularLightComponent);
}

void main() {
    vec3 temp_color = vec3(0, 0, 0);
    for (int i = 0; i < 2; i++) {
        PhongModel phongModel = ComputePhongModelFor(u_LightSources[i], u_Material);
        temp_color = temp_color + vec3(phongModel.Ambient + phongModel.Diffuse + phongModel.Specular);
    }

    color = vec4(temp_color, 1.0f);
}