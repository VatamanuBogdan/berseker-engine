#include "Application.h"
#include "Rendering/Renderer.h"
#include "Window/Window.h"
#include "Window/GLFW/GLFWWindow.h"

#include <spdlog/spdlog.h>
#include <chrono>


void Application::Init(std::shared_ptr<Scene> &initialScene) {
	if (initialised) {
		return;
	}

	scene = initialScene;
	InitWindow();
	InitRenderingAPI();

	// TODO Fix this workaround
	/*
	auto glfwWindow = std::static_pointer_cast<GLFWWindow>(window)->GetUnderlyingWindow();
	uiRenderer = std::make_unique<UIRendererImpl_GL_GLFW>(glfwWindow, UIRendererImpl_GL_GLFW::GLSLVersion(4, 6));
	uiRenderer->Init();
	*/

	initialScene->Init();
	Renderer::Init();

	initialised = true;
}

void Application::Deinit() {
	if (!initialised) {
		return;
	}
	GLFWWindowConcreteProvider::Deinit();
	initialised = false;
}

void Application::StartRunning() {
	MainLoop();
}

std::shared_ptr<Window>& Application::GetMainWindow() {
	return window;
}

void Application::Close() {
	shouldClose = true;
}

void Application::InitWindow() {
	GLFWWindowConcreteProvider::Init();

	WindowProps windowProps{};
	windowProps.Title = "Window";
	windowProps.Width = 1280;
	windowProps.Height = 720;

	window = GLFWWindowConcreteProvider::ProvideWindow(windowProps);
}

void Application::InitRenderingAPI() {
	window->BindAsContext();
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(GLFWWindowConcreteProvider::GetOpenGLProcAddress()))) {
		spdlog::error("Failed to load OpenGL functions");
		return;
	}

	std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL shading language " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	int textureUnitsNum;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnitsNum);
	std::cout << "Texture slots: " <<  textureUnitsNum << std::endl;
}

void Application::MainLoop() {
	auto frameStartTime = std::chrono::steady_clock::now();
	auto frameEndTime = frameStartTime;
	while (!shouldClose) {
		auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);
		double deltaTime = static_cast<double>(timeDifference.count()) / 1000;
		UpdateStage(deltaTime);
		RenderStage();
		window->SwapBuffers();
		window->PoolForEvents();
		frameStartTime = frameEndTime;
		frameEndTime = std::chrono::steady_clock::now();
	}
}


