#pragma once
#include "Core/Input/InputManager.h"

#include <memory>
#include <functional>


struct WindowProps {
	const char	*Title;
	uint32_t	 Width;
	uint32_t	 Height;
};

class Window {
public:
	virtual ~Window() = default;

	virtual void BindAsContext() = 0;
	virtual void SwapBuffers() = 0;
	virtual InputManager& GetInput() = 0;
	virtual void PoolForEvents() = 0;
  	virtual bool ShouldClose() = 0;

	[[nodiscard]] virtual uint32_t GetWidth() const = 0;
	[[nodiscard]] virtual uint32_t GetHeight() const = 0;

protected:
	Window() = default;
};


template <class ConcreteProvider>
class WindowProvider final {
public:
	static void Init() { ConcreteProvider::Init(); }
	static void Deinit() { ConcreteProvider::Deinit(); }

	static std::shared_ptr<Window> ProvideWindow(const WindowProps &props) {
		return ConcreteProvider::ProvideWindow(props);
	}

private:
	WindowProvider() = default;
};


