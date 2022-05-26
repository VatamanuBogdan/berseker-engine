#pragma once
#include "Rendering/GraphicsAPI/ShaderProgram.h"


class Material {
public:
	glm::vec3					Ambient;
	glm::vec3					Diffuse;
	glm::vec3 					Specular;
	std::shared_ptr<ShaderProgram> 	Shader = nullptr;


	Material(const glm::vec3 &ambient,
		   const glm::vec3 &diffuse,
		   const glm::vec3 &specular,
		   const std::shared_ptr<ShaderProgram> &shaderProgram)
		   : Ambient(ambient), Diffuse(diffuse), Specular(specular), Shader(shaderProgram) {
	}

};