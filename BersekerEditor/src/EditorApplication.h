#pragma once
#include <Application.h>

#include <UI/Implementation/UIRendererImpl_GL_GLFW.h>

#include <memory>

class EditorApplication : public Application {
public:
	void Init();
	void Deinit() override;

	void UpdateStage(double deltaTime) override;
	void RenderStage() override;

private:
	void RenderUI();

private:
	std::unique_ptr<UIRendererBackend> uiRendererBackend;
};

