#pragma once
#include "Core/Window/Window.h"
#include "Core/Input/InputManager.h"

struct GLFWwindow;

class GLFWWindow;
class GLFWWindowConcreteProvider;
struct WindowData;

class GLFWInput final : public InputManager {
public:
	[[nodiscard]] KeyState GetStateForKey(KeyCodeType keyCode) const override;
	[[nodiscard]] MousePosition GetMousePosition() const override;
	[[nodiscard]] KeyState GetMouseButtonState(KeyCodeType buttonCode) const override;

private:
	WindowData		*windowData = nullptr;
	friend class 	GLFWWindow;
};

class GLFWWindow final : public Window {
public:
	~GLFWWindow() override;

	void BindAsContext() override;
	void SwapBuffers() override;
	void PoolForEvents() override;

	InputManager& GetInput() override;

	[[nodiscard]] uint32_t GetWidth() const override ;
	[[nodiscard]] uint32_t GetHeight() const override;

	GLFWwindow* GetUnderlyingWindow();

	bool ShouldClose() override;

private:
	explicit GLFWWindow(const WindowProps &props);
	void SetupWindowData(const WindowProps &props);

	WindowData		*data;
	GLFWInput		 input;
	friend class 	 GLFWWindowConcreteProvider;
};

class GLFWWindowConcreteProvider final {
public:
	static void Init();
	static void Deinit();

	static void* GetOpenGLProcAddress();
	static std::shared_ptr<Window> ProvideWindow(const WindowProps &props);

private:
	GLFWWindowConcreteProvider() = default;
};

