#include "SceneUIController.h"
#include "Scene.h"

namespace BVolumesTesting {

	SceneUIController::SceneUIController(Scene *scene)
		: scene(scene) {
		auto &io = ImGui::GetIO();
		jetbrainsMono = io.Fonts->AddFontFromFileTTF("res/fonts/JetBrainsMono-Regular.ttf", 14);
	}

	void SceneUIController::OnUpdate() {
	}

	void SceneUIController::OnRendering() {
		ImGui::Begin("Transform");
		ImGui::PushFont(jetbrainsMono);

		uiTransform3D.Draw(*scene->adapter);

		ImGui::PopFont();
		ImGui::End();
	}

}