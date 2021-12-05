#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Camera {
public:
	struct Direction {
		glm::vec3 Upward;
		glm::vec3 Forward;
		glm::vec3 Rightward;
	};

public:
	void SetPositioning(const glm::vec3 &position, const glm::vec3 &rotations) {
		this->position = position;
		this->rotations = rotations;
		ComputeDirection();
	}

	[[nodiscard]] const glm::vec3& GetPosition() const { return position; }
	[[nodiscard]] const glm::vec3& GetRotations() const { return rotations; }
	[[nodiscard]] const glm::mat4& GetProjection() const { return projection; }
	[[nodiscard]] const Direction& GetDirection() const { return direction; }

	[[nodiscard]] glm::mat4 GetView() const {
		return glm::lookAt(position, position + direction.Forward, direction.Upward);
	}

private:
	void ComputeDirection() {
		glm::mat4 transform(1);
		transform = glm::rotate(transform, rotations.z, glm::vec3(0, 0, 1.0));
		transform = glm::rotate(transform, rotations.y, glm::vec3(0, 1.0, 0));
		transform = glm::rotate(transform, rotations.x, glm::vec3(1.0, 0, 0));

		direction.Upward = glm::normalize(glm::vec3(transform * glm::vec4(0, 1, 0, 1)));
		direction.Forward = glm::normalize(glm::vec3(transform * glm::vec4(0, 0, 1, 1)));
		direction.Rightward = glm::normalize(glm::cross(direction.Forward, direction.Upward));
	}

protected:
	explicit Camera(const glm::vec3 &position, const glm::vec3 &rotation, const glm::mat4 &projection)
		: position(position), rotations(rotation), projection(projection) {
		ComputeDirection();
	}

protected:
	glm::vec3 position;
	glm::vec3 rotations;
	glm::mat4 projection;
	Direction direction{};
};

class PerspectiveCamera : public Camera {
public:
	struct Properties {
		float FovRadians;
		float AspectRation;
		float NearPlane;
		float FarPlane;

		Properties(float fovRadians, float aspectRation, float nearPlane, float farPlane)
			: FovRadians(fovRadians), AspectRation(aspectRation), NearPlane(nearPlane), FarPlane(farPlane) {
		}
	};

public:
	PerspectiveCamera(const glm::vec3 &position, const glm::vec3 &rotations, const Properties &properties)
		: Camera(position, rotations, glm::mat4(1.0f)), properties(properties)  {
		ComputePerspective();
	}

	[[nodiscard]] const Properties& GetProperties() const { return properties; }
	void SetProperties(const Properties &properties) {
		this->properties = properties;
		ComputePerspective();
	}

private:
	void ComputePerspective() {
		this->projection = glm::perspective(properties.FovRadians,
								 properties.AspectRation,
								 properties.NearPlane,
								 properties.FarPlane
								 );
	}

private:
	Properties properties;
};