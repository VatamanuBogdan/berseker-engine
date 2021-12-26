#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>
#include <csignal>

#include "Graphics/OpenGL/OpenGL.hpp"
#include "Graphics/OpenGL/VertexBuffer.hpp"
#include "Graphics/OpenGL/VertexArray.hpp"
#include "Graphics/OpenGL/IndexBuffer.hpp"
#include "Graphics/OpenGL/ShaderProgram.h"
#include "Graphics/OpenGL/Texture2D.hpp"

#include "Graphics/GridLine.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.h"

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

void CameraControllerTest(Camera &camera, GLFWwindow *window, float deltaTime) {
	constexpr float velocity = 3.0f;
	constexpr float sensitivity = 0.5f;

	auto position = camera.GetPosition();
	auto rotations = camera.GetRotations();
	auto &direction = camera.GetDirection();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += velocity * direction.Forward * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= velocity * direction.Forward * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= velocity * direction.Rightward * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += velocity * direction.Rightward * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position -= velocity * direction.Upward * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += velocity * direction.Upward * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		rotations.x -= sensitivity * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		rotations.x += sensitivity * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		rotations.y -= sensitivity * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		rotations.y += sensitivity * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		std::cout << "Camera Properties: " << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "[Position]: " << position << std::endl;
		std::cout << "[Rotations]: " << rotations << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "[Upward]: " << direction.Upward << std::endl;
		std::cout << "[Forward]: " << direction.Forward << std::endl;
		std::cout << "[Rightward]: " << direction.Rightward << std::endl;
	}

	camera.SetPositioning(position, rotations);
}

int main(int argc, char *argv[]) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 800, "Hello World", nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		spdlog::error("Failed to init glfw");
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
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

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		PerspectiveCamera::Properties properties(
			  glm::radians(45.0f),
			  ((float) width) / ((float) height),
			  0.1f,
			  100.0f
			  );
		PerspectiveCamera camera(glm::vec3(0), glm::vec3(0), properties);

		GridLine gridLine;

		glEnable(GL_DEPTH_TEST);

		double deltaTime = 0;
		double lastTime = glfwGetTime();
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			CameraControllerTest(camera, window, deltaTime);


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
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			double newTime = glfwGetTime();
			deltaTime = newTime - lastTime;
			lastTime = newTime;
		}

	} catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
	glfwTerminate();
	return 0;
}