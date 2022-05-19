#pragma once
#include "Window/Window.h"
#include "Scene.h"
#include "Utils.h"
#include "UI/UIRenderer.h"

#include <memory>

class Application final {
public:
	void Init(std::shared_ptr<Scene> &initialScene);
	void Deinit();

	void StartRunning();
	void SwitchSceneTo(std::shared_ptr<Scene> &scene);

	std::shared_ptr<Window>& GetMainWindow();
	UIRenderer& GetUIRenderer();

private:
	void InitWindow();
	void InitRenderingAPI();

private:
	void MainLoop();

private:
	std::shared_ptr<Scene>		scene;
	std::shared_ptr<Window>		window;
	std::unique_ptr<UIRenderer>	uiRenderer;
	bool 					initialised = false;
};
