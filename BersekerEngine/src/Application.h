#pragma once
#include "Window/Window.h"
#include "Scene.h"
#include "Utils.h"

#include <memory>

class Application {
public:
	virtual void Init(std::shared_ptr<Scene> &initialScene);
	virtual void Deinit();

	void StartRunning();

	std::shared_ptr<Window>& GetMainWindow();

	void Close();

	virtual void UpdateStage(double deltaTime) = 0;
	virtual void RenderStage() = 0;

private:
	void InitWindow();
	void InitRenderingAPI();

	void MainLoop();

protected:
	std::shared_ptr<Scene>		scene;
	std::shared_ptr<Window>		window;

private:
	bool 					shouldClose	= false;
	bool 					initialised	= false;
};
