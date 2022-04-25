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
#include <Scene.h>
#include <Utils.h>

#include <vector>
#include <memory>

namespace BVolumesTesting {

	struct BoundedPrimitivesStorage {
		using PrimitivePropsPtr = std::unique_ptr<Primitive3DProps>;
		using BoundedVolumePtr = std::unique_ptr<BVolumes::BoundingVolume>;

		struct PrimitiveEntity {
			std::string 	ID;
			BoundedVolumePtr 	BVolume;
			PrimitivePropsPtr	Props;
			bool 			CollisionFlag;
		};

		std::vector<PrimitiveEntity>	primitives;

		void Add(const char *id, PrimitivePropsPtr &&props, BoundedVolumePtr &&bVolume);
	};


	class BoundedPrimitivesAdapter final : public UITransform3D::Adapter {
	public:
		explicit BoundedPrimitivesAdapter(BoundedPrimitivesStorage *storage);

		void SetPosition(const glm::vec3 &position, int index) override;
		void SetRotation(const glm::vec3 &rotation, int index) override;
		void SetScale(const glm::vec3 &scale, int index) override;

		const glm::vec3 &GetPosition(int index) override;
		const glm::vec3 &GetRotation(int index) override;
		const glm::vec3 &GetScale(int index) override;

		int GetSize() override;

		const char *GetID(int index) override;

	private:
		BoundedPrimitivesStorage *storage;
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
		void CollisionUpdate();

	private:
		std::shared_ptr<Camera> 			camera = nullptr;
		std::shared_ptr<GridLine> 			gridLine;
		std::shared_ptr<ShaderProgram>		shader;
		std::shared_ptr<Model>				model;
		std::shared_ptr<UIController>			uiController;
		Lateinit<BoundedPrimitivesAdapter>		adapter;
		BoundedPrimitivesStorage			storage;

	private:
		friend class SceneUIController;
	};

}