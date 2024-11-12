#include "GJKScene.h"
#include "Rendering/Renderer.h"
#include "Application.h"
#include "Rendering/GridLine.hpp"
#include "Scene/Components/Components.h"
#include "../Utils/Timer.h"

#include <unordered_map>
#include <set>

GJKScene::GJKScene(Application *application)
	: Scene(application) {
}

static std::unordered_map<Ecs::Entity::IdType, std::set<size_t>> collidedComponents;

void GJKScene::Init() {
	auto &window = application->GetMainWindow();
	uint32_t width = window->GetWidth();
	uint32_t height = window->GetHeight();

	PerspectiveCamera::Properties properties(
		  glm::radians(45.0f),
		  ((float) width) / ((float) height),
		  0.1f,
		  100.0f
	);
	camera.reset(new PerspectiveCamera(glm::vec3(0), glm::vec3(0), properties));
	gridLine = std::make_shared<GridLine>();

	Renderer::SetCamera(camera);
	InitEntities();
	collisionStrategy.Init();
}

void GJKScene::Deinit() {
	collisionStrategy.Deinit();
}

void GJKScene::OnPreUpdate() {
	for (auto &entity1 : entities) {
		collidedComponents[entity1.GetId()].clear();
	}

	for (auto &entity1 : entities) {
		auto *transform1 = registry.GetComponentFrom<Transform>(entity1);
		auto *collider1 = registry.GetComponentFrom<ColliderComponent>(entity1);

		if (!transform1 || !collider1) {
			continue;
		}

		(*collider1)->SetTransform(*transform1);

		for (auto &entity2 : entities) {
			if (entity1 == entity2) {
				continue;
			}

			auto *transform2 = registry.GetComponentFrom<Transform>(entity2);
			auto *collider2 = registry.GetComponentFrom<ColliderComponent>(entity2);

			if (!transform2 || !collider2) {
				continue;
			}

			(*collider2)->SetTransform(*transform2);

			auto collisionData = collisionStrategy.AreColliding(**collider1, **collider2);

			if (collisionData.valid) {
				collidedComponents[entity1.GetId()].emplace(collisionData.component1Index);
				collidedComponents[entity2.GetId()].emplace(collisionData.component2Index);
			}
		}
	}
}

void GJKScene::OnUpdate() {

}

void GJKScene::OnPostUpdate() {

}

void GJKScene::OnPreRendering() {
	Renderer::ClearLights();

	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<ModelComponent>(entity);
		auto *lightSource = registry.GetComponentFrom<LightSource>(entity);

		if (transform && model) {
			Transform modifiedTransform = *transform;
			modifiedTransform.Scale -= 0.01;
			Renderer::SubmitModelForRendering(model->get(), modifiedTransform.ComputeTransformMatrix());
		}

		if (transform && lightSource) {
			Renderer::AddLight(*lightSource, transform->Position);
		}
	}
}

void GJKScene::OnPostRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *renderableCollider = registry.GetComponentFrom<RenderableCollider>(entity);

		if (transform && renderableCollider) {
			Renderer::RenderCollider(*renderableCollider, *transform, collidedComponents[entity.GetId()]);
		}
	}

	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void GJKScene::InitEntities() {
	std::shared_ptr<Model> dragon = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/Monkey.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	std::shared_ptr<Model> armadillo = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/Monkey.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Model1");
		registry.AddComponentTo<ModelComponent>(entity, dragon);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform());
		entities.push_back(std::move(entity));
	}

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Model2");
		registry.AddComponentTo<ModelComponent>(entity, armadillo);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform());
		entities.push_back(std::move(entity));
	}

	/* Light Source2 */ {
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "LightSource2");
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(4, 4, 0), glm::vec3(1), glm::vec3(0.5f)));
		registry.AddComponentTo<LightSource>(entity, LightSource(glm::vec3(0.1), glm::vec3(0, 1, 0), glm::vec3(0.4)));

		entities.push_back(std::move(entity));
	}

	/* Light Source1 */ {
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "LightSource1");
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 4, 2), glm::vec3(1), glm::vec3(0.5f)));
		registry.AddComponentTo<LightSource>(entity, LightSource(glm::vec3(0.1), glm::vec3(1), glm::vec3(0.4)));

		entities.push_back(std::move(entity));
	}


}




