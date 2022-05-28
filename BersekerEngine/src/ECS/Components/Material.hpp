#pragma once
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include <bitset>

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

	bool IsLighted() { return mask[0]; }
	void SetLighted(bool value) { mask[0] = value; }

private:
	constexpr static size_t	MaskSize = 1;
	std::bitset<MaskSize> 	mask{};
};