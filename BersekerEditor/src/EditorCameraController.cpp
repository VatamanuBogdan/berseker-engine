#include "EditorCameraController.h"

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
	constexpr float velocity = 3.0f;
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
		std::cout << "Camera Properties: " << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "[Position]: " << position << std::endl;
		std::cout << "[Rotations]: " << rotations << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "[Upward]: " << direction.Upward << std::endl;
		std::cout << "[Forward]: " << direction.Forward << std::endl;
		std::cout << "[Rightward]: " << direction.Rightward << std::endl;
	}

	auto mousePosition = input.GetMousePosition();
	if (input.GetStateForKey(KeyboardKey::F1) == KeyState::Pressed) {
		std::cout << "Mouse position: " << mousePosition.xPosition << " " << mousePosition.yPosition
			    << std::endl;
	}

	camera.SetPositioning(position, rotations);
}