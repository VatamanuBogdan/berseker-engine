#pragma once
#include "SceneUIController.h"

#include <Window/Window.h>
#include <Rendering/GraphicsAPI/ShaderProgram.h>
#include <Rendering/Primitives/Primitives.h>
#include <Rendering/GridLine.hpp>
#include <Rendering/Model.h>
#include <UI/UIRenderer.h>
#include <UI/Controls/UITransform3D.h>
#include <Collision/BVolumes.h>
#include <ECS/ECS.h>
#include <ECS/Components/Components.h>
#include <Scene.h>
#include <Utils.h>

#include <vector>
#include <memory>

namespace BVolumesTesting {

	class BoundedPrimitivesAdapter final : public UITransform3D::Adapter {
	public:
		Transform& GetTransformFor(int index);

		void SetPosition(const glm::vec3 &position, int index) override;
		void SetRotation(const glm::vec3 &rotation, int index) override;
		void SetScale(const glm::vec3 &scale, int index) override;

		const glm::vec3 &GetPosition(int index) override;
		const glm::vec3 &GetRotation(int index) override;
		const glm::vec3 &GetScale(int index) override;

		void Bind(ECS::Registry *registry, std::vector<ECS::Entity> *entities);

		int GetSize() override;

		const char *GetID(int index) override;

	private:
		ECS::Registry			*registry = nullptr;
		std::vector<ECS::Entity> 	*entities = nullptr;
	};


	class Scene final : public ::Scene {
	public:
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
		std::shared_ptr<UIController>			uiController;
		BoundedPrimitivesAdapter			adapter;

		ECS::Registry					registry;
		std::vector<ECS::Entity> 			entities;

	private:
		friend class SceneUIController;
	};

}