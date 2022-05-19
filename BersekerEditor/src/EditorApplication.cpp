#include "EditorApplication.h"

#include <Rendering/Renderer.h>

void EditorApplication::UpdateStage(double deltaTime) {
	SafeNullableCall(scene, OnPreUpdate())
	SafeNullableCall(scene, OnUpdate())
	SafeNullableCall(scene, OnPostUpdate())
}

void EditorApplication::RenderStage() {
	SafeNullableCall(scene, OnPreRendering())
	Renderer::Render();
	SafeNullableCall(scene, OnPostRendering())
}
