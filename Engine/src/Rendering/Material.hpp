#pragma once
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include <bitset>

class Material {
public:
	const std::string				Name;
	glm::vec3					Ambient;
	glm::vec3					Diffuse;
	glm::vec3 					Specular;
	float						Shininess;
	std::shared_ptr<ShaderProgram> 	Shader = nullptr;

	Material(const char *name,
		   const glm::vec3 &ambient,
		   const glm::vec3 &diffuse,
		   const glm::vec3 &specular,
		   float shininess,
		   const std::shared_ptr<ShaderProgram> &shaderProgram)
		   : Name(name),
		     Ambient(ambient), Diffuse(diffuse), Specular(specular),
		     Shininess(shininess),Shader(shaderProgram) {
	}

	[[nodiscard]] bool IsLighted() const { return mask[0]; }
	void SetLighted(bool value) { mask[0] = value; }

private:
	constexpr static size_t	MaskSize = 1;
	std::bitset<MaskSize> 	mask{};
};