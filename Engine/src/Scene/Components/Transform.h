#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	explicit Transform(const glm::vec3 &position = glm::vec3(0),
		    const glm::vec3 &rotation = glm::vec3(0),
		    const glm::vec3 &scale = glm::vec3(1))
		    : Position(position), Rotation(rotation), Scale(scale) {
	}

	[[nodiscard]] glm::mat4 ComputeTransformMatrix() const {
		glm::mat4 transform = glm::translate(glm::mat4(1), Position);
		transform = glm::rotate(transform, Rotation.z, glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, Rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, Rotation.x, glm::vec3(1, 0, 0));
		transform = glm::scale(transform, Scale);
		return transform;
	}
};