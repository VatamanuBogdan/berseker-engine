#include "Views.h"

#include <imgui.h>

LinearLayout::LinearLayout(const char *id)
	: id(id) {
}


void LinearLayout::Draw() {
	ImGui::Begin(id.c_str());
	for (auto &view : views) {
		view->Draw();
	}
	ImGui::End();
}

void LinearLayout::AddChildren(View *view) {
	views.emplace_back(view);
}

