#pragma once
#include "Rendering/GraphicsAPI/OpenGL.h"
#include "Core/Window/Window.h"
#include "Scene.h"

#include <memory>

class Application {
public:
	Application(const char *name);

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

private:
	// TODO This should be set on Renderer
	static void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
					 GLsizei length, const GLchar *message, const void *userParam);

protected:
	std::shared_ptr<Scene>		scene;
	std::shared_ptr<Window>		window;

private:
	std::string name;
	bool 		shouldClose	= false;
	bool 		initialised	= false;
};
