#pragma once
#include <glm/glm.hpp>

struct LightSource {
	glm::vec3 Position;

	explicit LightSource(const glm::vec3 &position)
		: Position(position) {
	}
};