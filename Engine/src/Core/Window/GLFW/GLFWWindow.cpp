#include "GLFWWindow.h"
#include "GLFW/glfw3.h"

// TODO Must be replaced by logger
#include <iostream>

struct WindowData {
	GLFWwindow		*Window;
};

GLFWWindow::GLFWWindow(const WindowProps &props) {
	SetupWindowData(props);
}

void GLFWWindow::SetupWindowData(const WindowProps &props) {
	data = new WindowData();
	data->Window = glfwCreateWindow(
		  static_cast<int>(props.Width),
		  static_cast<int>(props.Height),
		  props.Title, nullptr, nullptr
	);
	if (!data->Window) {
		const char *errorDescription;
		glfwGetError(&errorDescription);
		throw std::runtime_error(std::string("Cannot initialized Linux Window") + errorDescription);
	}
	glfwSetWindowUserPointer(data->Window, data);

	input.windowData = data;
}

GLFWWindow::~GLFWWindow() {
	glfwDestroyWindow(data->Window);
	delete data;
}

void GLFWWindow::BindAsContext() {
	glfwMakeContextCurrent(data->Window);
	// TODO Remove
	glfwSwapInterval(0);
}

void GLFWWindow::SwapBuffers() {
	glfwSwapBuffers(data->Window);
}

void GLFWWindow::PoolForEvents() {
	glfwPollEvents();
}

uint32_t GLFWWindow::GetWidth() const {
	int width;
	glfwGetWindowSize(data->Window, &width, nullptr);
	return width;
}

uint32_t GLFWWindow::GetHeight() const {
	int height;
	glfwGetWindowSize(data->Window, nullptr, &height);
	return height;
}

InputManager &GLFWWindow::GetInput() {
	return input;
}

GLFWwindow *GLFWWindow::GetUnderlyingWindow() {
	return data->Window;
}

bool GLFWWindow::ShouldClose() {
	return glfwWindowShouldClose(data->Window);
}

void GLFWWindowConcreteProvider::Init() {
	if (!glfwInit()) {
		throw std::runtime_error("Failed to init Linux Window library");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GLFWWindowConcreteProvider::Deinit() {
	glfwTerminate();
}

void *GLFWWindowConcreteProvider::GetOpenGLProcAddress() {
	return (void*) glfwGetProcAddress;
}

std::shared_ptr<Window> GLFWWindowConcreteProvider::ProvideWindow(const WindowProps &props) {
	auto window = new GLFWWindow(props);
	return std::shared_ptr<Window>(window);
}

KeyState GLFWInput::GetStateForKey(KeyCodeType keyCode) const {
	auto state = glfwGetKey(windowData->Window, keyCode);
	return state == GLFW_PRESS ? KeyState::Pressed : KeyState::Released;
}

MousePosition GLFWInput::GetMousePosition() const {
	double xPosition, yPosition;
	int width, height;

	glfwGetWindowSize(windowData->Window, &width, &height);
	glfwGetCursorPos(windowData->Window, &xPosition, &yPosition);

	xPosition = 2 * xPosition / static_cast<double>(width) - 1;
	yPosition = - 2 * yPosition / static_cast<double>(height) + 1;

	return MousePosition(xPosition, yPosition);
}

KeyState GLFWInput::GetMouseButtonState(KeyCodeType buttonCode) const {
	auto state = glfwGetMouseButton(windowData->Window, buttonCode);
	return state == GLFW_PRESS ? KeyState::Pressed : KeyState::Released;
}
