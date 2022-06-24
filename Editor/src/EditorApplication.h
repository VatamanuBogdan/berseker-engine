#pragma once
#include "UI/Implementation/UIRendererImpl_GL_GLFW.h"

#include <Rendering/GraphicsAPI/FrameBuffer.h>
#include <Application.h>
#include <Core/Memory.h>

#include <imgui.h>
#include <memory>

class EditorApplication : public Application {
public:
	EditorApplication();

	void Init();
	void Deinit() override;

	void UpdateStage(double deltaTime) override;
	void RenderStage() override;

private:
	void LoadAssets();
	void RenderSceneHierarchyPanel();
	void RenderEntityPropertiesPanel();
	void RenderEditor();

private:
	ImFont	*defaultEditorFont;
	Ecs::Entity	*selectedEntity = nullptr;
	int		 materialId = -1;

private:
	Lateinit<FrameBuffer>			fbo;
	std::unique_ptr<UIRendererBackend>	uiRendererBackend;
};

