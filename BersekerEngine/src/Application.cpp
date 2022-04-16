#include "Application.h"
#include "Rendering/Renderer.h"
#include "Window/Window.h"
#include "Window/GLFW/GLFWWindow.h"
#include "UI/Implementation/UIRendererImpl_GL_GLFW.h"

#include <spdlog/spdlog.h>
#include <iostream>


void Application::Init(std::shared_ptr<Scene> &initialScene) {
	if (Application::initialised) {
		return;
	}

	GLFWWindowConcreteProvider::Init();

	WindowProps windowProps{};
	windowProps.Title = "Window";
	windowProps.Width = 1280;
	windowProps.Height = 720;

	Application::window = GLFWWindowConcreteProvider::ProvideWindow(windowProps);
	Application::scene = initialScene;

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

	// TODO Fix this workaround
	auto glfwWindow = std::static_pointer_cast<GLFWWindow>(window)->GetUnderlyingWindow();
	uiRenderer = std::make_unique<UIRendererImpl_GL_GLFW>(glfwWindow, UIRendererImpl_GL_GLFW::GLSLVersion(4, 6));
	uiRenderer->Init();

	initialScene->Init();
	Renderer::Init();

	Application::initialised = true;
}

void Application::Deinit() {
	if (!initialised) {
		return;
	}
	GLFWWindowConcreteProvider::Deinit();
	Application::initialised = false;

	uiRenderer->Deinit();
}

void Application::StartRunning() {
	MainLoop();
}

void Application::SwitchSceneTo(std::shared_ptr<Scene> &scene) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

std::shared_ptr<Window>& Application::GetMainWindow() {
	return window;
}

UIRenderer &Application::GetUIRenderer() {
	return *uiRenderer;
}

void Application::MainLoop() {
	while (true) {
		SafeNullableCall(scene, OnPreUpdate())
		SafeNullableCall(scene, OnUpdate())
		SafeNullableCall(scene, OnPostUpdate())
		uiRenderer->Update();

		SafeNullableCall(scene, OnPreRendering())
		Renderer::Render();
		SafeNullableCall(scene, OnPostRendering())
		uiRenderer->Render();
		Application::window->SwapBuffers();
		Application::window->PoolForEvents();
	}
}

std::shared_ptr<Scene>		Application::scene	 = nullptr;
std::shared_ptr<Window>		Application::window 	 = nullptr;
std::unique_ptr<UIRenderer>	Application::uiRenderer;
bool 					Application::initialised = false;
