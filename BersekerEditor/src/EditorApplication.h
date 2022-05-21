#pragma once
#include "UI/Implementation/UIRendererImpl_GL_GLFW.h"

#include <Rendering/GraphicsAPI/FrameBuffer.h>
#include <Application.h>
#include <Utils.h>

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
	Lateinit<FrameBuffer>			fbo;
	std::unique_ptr<UIRendererBackend>	uiRendererBackend;
};

