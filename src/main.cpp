#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>
#include <csignal>
#include <optional>

#include "Graphics/OpenGL/OpenGL.hpp"
#include "Graphics/OpenGL/VertexBuffer.hpp"
#include "Graphics/OpenGL/VertexArray.hpp"
#include "Graphics/OpenGL/IndexBuffer.hpp"
#include "Graphics/OpenGL/ShaderProgram.h"
#include "Graphics/OpenGL/Texture2D.hpp"

#include "Graphics/GridLine.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.h"

#include "Window/Window.h"
#include "Window/GLFW//GLFWWindow.h"

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
					 GLsizei length, const GLchar *message, const void *userParam) {
	std::cout << "Message: " << message << std::endl;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3 &vec) {
	os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4 &mat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			os << mat[j][i] << " ";
		os << std::endl;
	}
	return os;
}

void CameraPositionController(InputManager &input, Camera &camera) {
	constexpr float velocity = 3.0f;
	constexpr float sensitivity = 0.5f;
	constexpr float constant = 0.01f;

	static std::optional<MousePosition> previousPosition;

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
	if (previousPosition) {
		auto mouseOffset = mousePosition;
		mouseOffset.xPosition -= previousPosition->xPosition;
		mouseOffset.yPosition -= previousPosition->yPosition;
		rotations.x -= mouseOffset.yPosition * sensitivity;
		rotations.y -= mouseOffset.xPosition * sensitivity;
	}
	previousPosition = mousePosition;

	if (input.GetStateForKey(KeyboardKey::F1) == KeyState::Pressed) {
		std::cout << "Mouse position: " << mousePosition.xPosition << " " << mousePosition.yPosition << std::endl;
	}

	camera.SetPositioning(position, rotations);
}

int main(int argc, char *argv[]) {
	/* Initialize the library */
	GLFWWindowConcreteProvider::Init();

	WindowProps windowProps;
	windowProps.Title = "Window";
	windowProps.Width = 1280;
	windowProps.Height = 720;


	std::shared_ptr<Window> window = GLFWWindowConcreteProvider::ProvideWindow(windowProps);

	/* Make the window's context current */
	window->BindAsContext();
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(GLFWWindowConcreteProvider::GetOpenGLProcAddress()))) {
		spdlog::error("Failed to load OpenGL functions");
		return -1;
	}

	std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL shading language " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	int textureUnitsNum;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnitsNum);
	std::cout << "Texture slots: " <<  textureUnitsNum << std::endl;

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, nullptr);

	try {

		ShaderProgram modelProgram = ShaderProgram::LoadFrom("res/shaders/3DModel.vert.glsl", "res/shaders/3DModel.frag.glsl");

		Model model = ModelLoader().LoadModel("res/models/Model.obj");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto width = window->GetWidth();
		auto height = window->GetHeight();


		PerspectiveCamera::Properties properties(
			  glm::radians(45.0f),
			  ((float) width) / ((float) height),
			  0.1f,
			  100.0f
			  );
		PerspectiveCamera camera(glm::vec3(0), glm::vec3(0), properties);

		GridLine gridLine;

		glEnable(GL_DEPTH_TEST);

		// double deltaTime = 0;
		// double lastTime = glfwGetTime();
		/* Loop until the user closes the window */
		while (true)
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			CameraPositionController(window->GetInput(), camera);

			glm::mat4 projection = camera.GetProjection();
			glm::mat4 view = camera.GetView();

			auto &meshes = model.GetMeshes();
			for (auto &mesh : meshes) {
				modelProgram.Bind();
				modelProgram.SetUniform("Projection", camera.GetProjection());
				modelProgram.SetUniform("Model", glm::mat4(1.0f));
				modelProgram.SetUniform("View", view);

				mesh.GetVertexArray().Bind();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				OpenGL::DrawElementsWith(mesh.GetIndexBuffer());
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			gridLine.Draw(camera.GetView(), camera.GetProjection());

			/* Swap front and back buffers */
			window->SwapBuffers();

			/* Poll for and process events */
			window->PoolForEvents();
			/*
			double newTime = glfwGetTime();
			deltaTime = newTime - lastTime;
			lastTime = newTime;
			 */
		}

	} catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
	GLFWWindowConcreteProvider::Deinit();
	return 0;
}