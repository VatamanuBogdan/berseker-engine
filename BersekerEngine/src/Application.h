#pragma once
#include "Window/Window.h"
#include "Scene.h"
#include "Utils.h"
#include "UI/UIRenderer.h"

#include <memory>

class Application {
public:
	virtual void Init(std::shared_ptr<Scene> &initialScene);
	virtual void Deinit();

	void StartRunning();

	std::shared_ptr<Window>& GetMainWindow();
	UIRenderer& GetUIRenderer();

	void Close();

	virtual void UpdateStage(double deltaTime) = 0;
	virtual void RenderStage() = 0;

private:
	void InitWindow();
	void InitRenderingAPI();

	void MainLoop();

protected:
	std::shared_ptr<Scene>		scene;

private:
	std::shared_ptr<Window>		window;
	std::unique_ptr<UIRenderer>	uiRenderer;
	bool 					shouldClose	= false;
	bool 					initialised	= false;
};
