#include "UITransform3D.h"

#include <imgui.h>

void UITransform3D::Draw() {
	Adapter *adapter = nullptr;
	if (adapterIndex >= 0) {
		adapter = adaptersContainer[adapterIndex].get();
	}

	glm::vec3 position(0);
	glm::vec3 rotation(0);
	glm::vec3 scale(0);

	if (adapter) {
		position = adapter->GetPosition();
		rotation = adapter->GetRotation();
		scale = adapter->GetScale();
	}

	ImGui::Text("Position");
	ImGui::DragFloat3("##Position", &position.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Rotation");
	ImGui::DragFloat3("##Rotation", &rotation.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", &scale.x, 0.01f, -100.0f, 100.0f, "%.2f");

	ImGui::Text("Items");
	if (adapter) {
		adapter->SetPosition(position);
		adapter->SetRotation(rotation);
		adapter->SetScale(scale);
	}

	ImGui::ListBox("##Items", &adapterIndex, ItemGetterWrapper, static_cast<void*>(this), adaptersContainer.size());
}

bool UITransform3D::ItemGetterWrapper(void* data, int index, const char** outText) {
	auto *uiTransform3D = static_cast<UITransform3D*>(data);
	*outText = uiTransform3D->ItemGetter(index);
	return true;
}

const char *UITransform3D::ItemGetter(int index) {
	return adaptersContainer[index]->GetID();
}

