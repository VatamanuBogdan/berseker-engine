#pragma once

#include <Window/Window.h>
#include <Rendering/GraphicsAPI/ShaderProgram.h>
#include <Rendering/Primitives/Primitives.h>
#include <Rendering/GridLine.hpp>
#include <Rendering/Model.h>
#include <Collision/BVolumes.h>
#include <ECS/ECS.h>
#include <ECS/Components/Components.h>
#include <Scene.h>
#include <Utils.h>

#include <vector>
#include <memory>

namespace BVolumesTesting {

	class Scene final : public ::Scene {
	public:
		explicit Scene(Application *application)
			: ::Scene(application) {
		}

		void Init() override;
		void Deinit() override;

		void OnPreUpdate() override;
		void OnUpdate() override;
		void OnPostUpdate() override;

		void OnPreRendering() override;
		void OnPostRendering() override;

	private:
		void CreateEntity(const char *identifier, const Transform& transform, BVolumes::BVolume::Type type);

		void InitEntities();
		void CollisionTest();

	private:
		std::shared_ptr<Camera> 			camera = nullptr;
		std::shared_ptr<GridLine> 			gridLine;
		std::shared_ptr<ShaderProgram>		shader;
		std::shared_ptr<Model>				model;

		ECS::Registry					registry;
		std::vector<ECS::Entity> 			entities;

	private:
		friend class SceneUIController;
	};

}