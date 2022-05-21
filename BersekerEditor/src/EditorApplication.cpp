#include "EditorApplication.h"

#include <Rendering/Renderer.h>
#include <Scenes/BVolumes/Scene.h>
#include <Rendering/GraphicsAPI/Texture2D.hpp>

#include <imgui.h>

void EditorApplication::UpdateStage(double deltaTime) {
	SafeNullableCall(scene, OnPreUpdate())
	SafeNullableCall(scene, OnUpdate())
	SafeNullableCall(scene, OnPostUpdate())
}

void EditorApplication::RenderStage() {
	fbo->Bind();
	SafeNullableCall(scene, OnPreRendering())
	Renderer::Render();
	SafeNullableCall(scene, OnPostRendering())
	fbo->Unbind();

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

	fbo.Init(1280, 720);
}

void EditorApplication::Deinit() {
	uiRendererBackend->Deinit();
	Application::Deinit();
}

void EditorApplication::RenderUI() {
	static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
	static bool opened = true;

	ImGuiWindowFlags dockSpaceWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	dockSpaceWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	dockSpaceWindowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	dockSpaceWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpaceWindow", &opened, dockSpaceWindowFlags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspaceId = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Close", nullptr, false))
				opened = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Window1");
	ImGui::Image(
		  (void*) fbo->GetTexture(), ImVec2(fbo->GetWidth(), fbo->GetHeight()),
		  ImVec2(0, 1), ImVec2(1, 0)

	);
	ImGui::PopStyleVar();
	ImGui::End();

	ImGui::Begin("Window2");
	ImGui::End();

	ImGui::Begin("Window3");
	ImGui::End();

	ImGui::End();
}