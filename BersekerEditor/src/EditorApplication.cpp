#include "EditorApplication.h"

#include <Rendering/Renderer.h>
#include <Scenes/BVolumes/Scene.h>

#include <imgui.h>

void EditorApplication::UpdateStage(double deltaTime) {
	SafeNullableCall(scene, OnPreUpdate())
	SafeNullableCall(scene, OnUpdate())
	SafeNullableCall(scene, OnPostUpdate())
}

void EditorApplication::RenderStage() {
	SafeNullableCall(scene, OnPreRendering())
	Renderer::Render();
	SafeNullableCall(scene, OnPostRendering())

	uiRendererBackend->PreUIRendering();
	RenderUI();
	uiRendererBackend->PostUIRendering();
}

void EditorApplication::Init() {
	std::shared_ptr<Scene> scene = std::make_shared<BVolumesTesting::Scene>(this);
	Application::Init(scene);
	auto glfwWindow = std::static_pointer_cast<GLFWWindow>(window)->GetUnderlyingWindow();
	uiRendererBackend = std::make_unique<UIRendererImpl_GL_GLFW>(glfwWindow, UIRendererImpl_GL_GLFW::GLSLVersion(4, 6));
	uiRendererBackend->Init();
}

void EditorApplication::Deinit() {
	uiRendererBackend->Deinit();
	Application::Deinit();
}

void EditorApplication::RenderUI() {
	ImGui::ShowDemoWindow();
}