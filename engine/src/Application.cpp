#include "Application.h"
#include "Rendering/Renderer.h"
#include "Core/Window/Window.h"
#include "Core/Window/GLFW/GLFWWindow.h"
#include "Core/Logger.h"

#include <chrono>


Application::Application(const char *name)
	  : name(name) {
}

void Application::Init(std::shared_ptr<Scene> &initialScene) {
	if (initialised) {
		return;
	}

	scene = initialScene;
	InitWindow();
	InitRenderingAPI();
	Renderer::Init();
	initialScene->Init();
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
	windowProps.Title = name.c_str();
	windowProps.Width = 1600;
	windowProps.Height = 900;

	window = GLFWWindowConcreteProvider::ProvideWindow(windowProps);
}

void Application::InitRenderingAPI() {
	window->BindAsContext();
	bool result = OpenGL::SetupOpenGL(
		  reinterpret_cast<GLADloadproc>(GLFWWindowConcreteProvider::GetOpenGLProcAddress()),
		  Application::OpenGLDebugCallback
		  );

	if (!result) {
		Logger::Log<Logger::ERROR>("Failed to load OpenGL functions");
		return;
	}

	int textureUnitsNum;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnitsNum);

	Logger::Log<Logger::INFO>("OpenGL version {}", glGetString(GL_VERSION));
	Logger::Log<Logger::INFO>("OpenGL shading language {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Logger::Log<Logger::INFO>("Texture slots {}", textureUnitsNum);
}

void Application::MainLoop() {
	auto frameStartTime = std::chrono::steady_clock::now();
	auto frameEndTime = frameStartTime;
	while (!shouldClose && !window->ShouldClose()) {
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

void Application::OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
				 GLsizei length, const GLchar *message, const void *userParam) {
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			Logger::Log<Logger::ERROR>("OpenGL issue severity {}: {}", severity, message);
			break;
		default:
			Logger::Log<Logger::WARNING>("OpenGL issue severity {}: {}", severity, message);
	}
}


