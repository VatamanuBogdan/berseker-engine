#pragma once
#include <UI/UIRenderer.h>
#include <UI/Controls/UITransform3D.h>

#include <imgui.h>

namespace BVolumesTesting {

	class Scene;

	class SceneUIController : public UIController {
	public:
		explicit SceneUIController(Scene *scene);

		void OnUpdate() final;
		void OnRendering() final;

	private:
		Scene			*scene;
		ImFont 		*jetbrainsMono;
		UITransform3D 	 uiTransform3D;
	};
}