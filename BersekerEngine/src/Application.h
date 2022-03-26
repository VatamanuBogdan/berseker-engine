#pragma once
#include "Window/Window.h"
#include "Scene.h"
#include "Utils.h"

#include <memory>

class Application final {
public:
	Application() = delete;

	static void Init(std::shared_ptr<Scene> &initialScene);
	static void Deinit();

	static void StartRunning();
	static void SwitchSceneTo(std::shared_ptr<Scene> &scene);

	static std::shared_ptr<Window>& GetMainWindow();

private:
	static void MainLoop();

private:
	static std::shared_ptr<Scene>		scene;
	static std::shared_ptr<Window>	window;
	static bool 				initialised;
};
