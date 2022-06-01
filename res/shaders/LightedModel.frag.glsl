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

uniform LightSource u_LightSource;
uniform Material    u_Material;

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
    PhongModel phongModel = ComputePhongModelFor(u_LightSource, u_Material);

    color = vec4(phongModel.Ambient + phongModel.Diffuse + phongModel.Specular, 1.0);
}