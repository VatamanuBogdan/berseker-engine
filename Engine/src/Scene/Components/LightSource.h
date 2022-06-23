#pragma once
#include <glm/glm.hpp>

struct LightSource {
	glm::vec3	Ambient;
	glm::vec3	Diffuse;
	glm::vec3	Specular;

	LightSource(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular) {
	}
};