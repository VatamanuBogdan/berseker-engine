#include "UITransform3D.h"

#include <imgui.h>

void UITransform3D::Draw(Adapter &adapter) {

	glm::vec3 position(0);
	glm::vec3 rotation(0);
	glm::vec3 scale(0);

	if (adapterIndex >= 0) {
		position = adapter.GetPosition(adapterIndex);
		rotation = adapter.GetRotation(adapterIndex);
		scale = adapter.GetScale(adapterIndex);
	}

	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &position.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Rotation");
	ImGui::DragFloat3("##Rotation", &rotation.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &scale.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Items");
	if (adapterIndex >= 0) {
		adapter.SetPosition(position, adapterIndex);
		adapter.SetRotation(rotation, adapterIndex);
		adapter.SetScale(scale, adapterIndex);
	}

	ImGui::ListBox("##Items", &adapterIndex, IDGetterWrapper, static_cast<void *>(&adapter), adapter.GetSize());
}

bool UITransform3D::IDGetterWrapper(void* data, int index, const char** outText) {
	auto *adapter = static_cast<Adapter*>(data);
	*outText = adapter->GetID(index);
	return true;
}

