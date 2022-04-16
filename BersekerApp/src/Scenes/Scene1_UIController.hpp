#pragma once
#include <UI/UIRenderer.h>

#include <imgui.h>

class Scene1_UIController final : public UIController {
public:
	void OnUpdate() final {
	}

	void OnRendering() final {
		ImGui::ShowDemoWindow();
	}
};