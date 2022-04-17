#include "UITransform3D.h"

#include <imgui.h>

void UITransform3D::Draw(UITransform3D::Adapter &adapter) {
	glm::vec3 position = adapter.GetPosition();
	glm::vec3 rotation = adapter.GetRotation();
	glm::vec3 scale = adapter.GetScale();

	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &position.x, 0.01f, -100.0f, 100.0f, "%.2f");
	adapter.SetPosition(position);

	ImGui::Text("Rotation");
	ImGui::DragFloat3("##Rotation", &rotation.x, 0.01f, -100.0f, 100.0f, "%.2f");
	adapter.SetRotation(rotation);

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &scale.x, 0.01f, -100.0f, 100.0f, "%.2f");
	adapter.SetScale(scale);
}
