#include "EditorCameraController.h"
#include "Utils/Logger.h"

#include <iostream>


std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
	os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			os << mat[j][i] << " ";
		os << std::endl;
	}
	return os;
}

void EditorCameraController(InputManager &input, Camera &camera) {
	constexpr float velocity = 6.0f;
	constexpr float sensitivity = 1.25f;
	constexpr float constant = 0.01f;

	auto position = camera.GetPosition();
	auto rotations = camera.GetRotations();
	auto &direction = camera.GetDirection();

	if (input.GetStateForKey(KeyboardKey::W) == KeyState::Pressed) {
		position += velocity * direction.Forward * constant;
	}
	if (input.GetStateForKey(KeyboardKey::S) == KeyState::Pressed) {
		position -= velocity * direction.Forward * constant;
	}

	if (input.GetStateForKey(KeyboardKey::A) == KeyState::Pressed) {
		position -= velocity * direction.Rightward * constant;
	}
	if (input.GetStateForKey(KeyboardKey::D) == KeyState::Pressed) {
		position += velocity * direction.Rightward * constant;
	}

	if (input.GetStateForKey(KeyboardKey::LEFT_SHIFT) == KeyState::Pressed) {
		position -= velocity * direction.Upward * constant;
	}
	if (input.GetStateForKey(KeyboardKey::SPACE) == KeyState::Pressed) {
		position += velocity * direction.Upward * constant;
	}

	if (input.GetStateForKey(KeyboardKey::KP_8) == KeyState::Pressed) {
		rotations.x -= sensitivity * constant;
	}
	if (input.GetStateForKey(KeyboardKey::KP_2) == KeyState::Pressed) {
		rotations.x += sensitivity * constant;
	}

	if (input.GetStateForKey(KeyboardKey::KP_6) == KeyState::Pressed) {
		rotations.y -= sensitivity * constant;
	}
	if (input.GetStateForKey(KeyboardKey::KP_4) == KeyState::Pressed) {
		rotations.y += sensitivity * constant;
	}

	if (input.GetStateForKey(KeyboardKey::TAB) == KeyState::Pressed) {
		auto mousePosition = input.GetMousePosition();

		Logger::Log<Logger::INFO>(
			  "Camera Properties: \n"
			  "------------------------------------------\n"
			  "[Position]: x: {} y: {} z: {}\n"
			  "[Rotations]: x: {} y: {} z: {}\n"
			  "------------------------------------------\n"
			  "[Upward]: x: {} y: {} z: {}\n"
			  "[Forward]: x: {} y: {} z: {}\n"
			  "[Rightward]: x: {} y: {} z: {}\n"
			  "[Mouse position]: x: {} y: {}",
			  position.x, position.y, position.z,
			  rotations.x, rotations.y, rotations.z,
			  direction.Upward.x, direction.Upward.y, direction.Upward.z,
			  direction.Forward.x, direction.Forward.y, direction.Forward.z,
			  direction.Rightward.x, direction.Rightward.y, direction.Rightward.z,
			  mousePosition.xPosition, mousePosition.yPosition
	  	);
	}

	camera.SetPositioning(position, rotations);
}