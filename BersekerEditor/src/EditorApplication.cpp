#include "EditorApplication.h"
#include "EditorCameraController.h"

#include <Rendering/Renderer.h>
#include <Scenes/BVolumesScene.h>
#include <Rendering/GraphicsAPI/Texture2D.hpp>

#include <imgui.h>

void EditorApplication::UpdateStage(double deltaTime) {
	EditorCameraController(GetMainWindow()->GetInput(), scene->GetCamera());
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
	RenderEditor();
	uiRendererBackend->PostUIRendering();
}

void EditorApplication::Init() {
	std::shared_ptr<Scene> scene = std::make_shared<BVolumesScene>(this);
	Application::Init(scene);
	auto glfwWindow = std::static_pointer_cast<GLFWWindow>(window)->GetUnderlyingWindow();
	uiRendererBackend = std::make_unique<UIRendererImpl_GL_GLFW>(glfwWindow, UIRendererImpl_GL_GLFW::GLSLVersion(4, 6));

	uiRendererBackend->Init();

	fbo.Init(1280, 720);

	LoadAssets();
}

void EditorApplication::Deinit() {
	uiRendererBackend->Deinit();
	Application::Deinit();
}

void EditorApplication::LoadAssets() {
	auto io = ImGui::GetIO();
	defaultEditorFont = io.Fonts->AddFontFromFileTTF("res/fonts/SourceCodePro.ttf", 14.4);
}

void EditorApplication::RenderSceneHierarchyPanel() {
	// TODO: Find a better approach to identify entities. Temporary workaround
	static int entityId = -1;
	selectedEntity = nullptr;

	ImGui::Begin("Scene Hierarchy"); {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (ImGui::TreeNode("Entities")) {
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
			auto &entities = scene->GetEntities();
			for (int i = 0; i <  entities.size(); i++) {
				auto &entity = entities[i];
				auto *identifier = scene->GetRegistry().GetComponentFrom<Identifier>(entity);
				if (!identifier) {
					continue;
				}

				ImGuiTreeNodeFlags nodeFlags = flags;
				if (i == entityId) {
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
					selectedEntity = &entity;
				}

				ImGui::TreeNodeEx(identifier->Tag.c_str(), nodeFlags);
				if (ImGui::IsItemClicked()) {
					selectedEntity = &entity;
					entityId = i;
				}
			}
			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void EditorApplication::RenderEntityPropertiesPanel() {
	static auto renderVectorProperty = [](const char *label, glm::vec3 &vector) {
		constexpr float width = 55;

		ImGui::Text(label);

		ImGui::PushID(label);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::Button("X:");
		ImGui::SetNextItemWidth(width);
		ImGui::SameLine(); ImGui::DragFloat("##VALUE_X", &vector.x, 0.01f, -100.0f, 100.0f, "%.2f");

		ImGui::SameLine(); ImGui::Button("Y:");
		ImGui::SetNextItemWidth(width);
		ImGui::SameLine(); ImGui::DragFloat("##VALUE_Y", &vector.y, 0.01f, -100.0f, 100.0f, "%.2f");

		ImGui::SameLine(); ImGui::Button("Z:");
		ImGui::SetNextItemWidth(width);
		ImGui::SameLine(); ImGui::DragFloat("##VALUE_Z", &vector.z, 0.01f, -100.0f, 100.0f, "%.2f");

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopID();
	};

	ImGui::Begin("Entity Properties"); {
		if (selectedEntity) {
			if (auto *transform = scene->GetRegistry().GetComponentFrom<Transform>(*selectedEntity)) {
				ImGui::SetNextItemOpen(true);
				if (ImGui::TreeNode("Transform")) {
					renderVectorProperty("Position", transform->Position);
					renderVectorProperty("Rotation", transform->Rotation);
					renderVectorProperty("Scale", transform->Scale);
					ImGui::TreePop();
				}
				ImGui::Separator();
			}

			if (auto *lightSource = scene->GetRegistry().GetComponentFrom<LightSource>(*selectedEntity)) {
				ImGui::SetNextItemOpen(true);
				if (ImGui::TreeNode("Light Proprieties")) {
					ImGuiColorEditFlags flags = ImGuiColorEditFlags_None;

					ImGui::Columns(2, "locations");
					ImGui::SetColumnWidth(0, 75);

					ImGui::Text("Ambient");
					ImGui::Text("Diffuse");
					ImGui::Text("Specular");

					ImGui::NextColumn();
					ImGui::SetColumnWidth(1, 450);

					ImGui::ColorEdit3("##Ambient", &(lightSource->Ambient.x), flags);
					ImGui::ColorEdit3("##Diffuse", &lightSource->Diffuse.x, flags);
					ImGui::ColorEdit3("##Specular", &lightSource->Specular.x, flags);

					ImGui::Columns();

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
	}
	ImGui::End();
}

void EditorApplication::RenderEditor() {
	ImGui::PushFont(defaultEditorFont);

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
	ImGui::Begin("Scene");
	ImGui::Image(
		  (void*) fbo->GetTexture(), ImVec2(fbo->GetWidth(), fbo->GetHeight()),
		  ImVec2(0, 1), ImVec2(1, 0)

	);
	ImGui::PopStyleVar();
	ImGui::End();

	RenderSceneHierarchyPanel();
	RenderEntityPropertiesPanel();

	ImGui::End();


	ImGui::PopFont();
}
