#include "TestingScene.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"
#include "ResourceLoaders/ShaderRegistry.h"

#include <glm/glm.hpp>

#include <iostream>


void TestingScene::Init() {
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
	lightedModelShader = ShaderRegistry::Get().GetShader(ShaderResource::LightedModel);
	lightShader = ShaderRegistry::Get().GetShader(ShaderResource::LightSource);


	gridLine = std::make_shared<GridLine>();

	Renderer::SetCamera(camera);
	InitEntities();
}

void TestingScene::Deinit() {
	camera.reset();
}

void TestingScene::OnPreUpdate() {
	for (auto &entity : entities) {
		auto *collider = registry.GetComponentFrom<ColliderComponent>(entity);
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *collisionInfo = registry.GetComponentFrom<CollisionInfo>(entity);

		if (collider && transform) {
			collider->SetTransform(*transform);
		}
		if (collisionInfo) {
			collisionInfo->CollisionFlag = false;
		}
	}

	CollisionTest();
}

void TestingScene::OnUpdate() {
}

void TestingScene::OnPostUpdate() {
}

void TestingScene::OnPreRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<Model>(entity);
		auto *lightSource = registry.GetComponentFrom<LightSource>(entity);

		if (transform && model) {
			Renderer::SubmitModelForRendering(model, transform->ComputeTransformMatrix());

			AABB aabb = model->GetAABB();
			aabb.Apply(*transform);
			Renderer::RenderBVolume(aabb, Color(0, 1.0f, 1.0f));

		}

		if (transform && lightSource) {
			Renderer::SetLight(*lightSource, transform->Position);
		}

	}
}

void TestingScene::OnPostRendering() {
	for (auto &entity : entities) {
		auto *collider = registry.GetComponentFrom<ColliderComponent>(entity);
		auto *info = registry.GetComponentFrom<CollisionInfo>(entity);
		if (collider) {
			Color color(1, 1, 0);
			if (info && info->CollisionFlag) {
				color = Color(1, 0, 0);
			}

			Renderer::RenderBVolume(collider->GetBoundingVolume(), color);
		}
	}

	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<Model>(entity);

		if (transform && model) {
			AABB aabb = model->GetAABB();
			aabb.Apply(*transform);
			Renderer::RenderBVolume(aabb, Color(0, 1.0f, 1.0f));
		}
	}

	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void TestingScene::CreateEntity(const char *identifier, const Transform &transform, BVolume::Type type) {
	auto entity = ECS::Registry::CreateEntity();

	registry.AddComponentTo<Identifier>(entity, identifier);
	registry.AddComponentTo<Transform>(entity, transform);
	registry.AddComponentTo<ColliderComponent>(entity, transform, type);
	registry.AddComponentTo<CollisionInfo>(entity);

	entities.push_back(std::move(entity));
}

void TestingScene::InitEntities() {
	// Bounding Volumes Testing Entities
	CreateEntity("Entity1 AABB", Transform(glm::vec3(0, 0, 0)), BVolume::AABB);
	CreateEntity("Entity2 AABB", Transform(glm::vec3(1, 2, 0)), BVolume::AABB);
	CreateEntity("Entity3 Sphere", Transform(glm::vec3(3, -1, 0)), BVolume::Sphere);
	CreateEntity("Entity4 Sphere", Transform(glm::vec3(-2, 3, 1)), BVolume::Sphere);
	CreateEntity("Entity5 OBB", Transform(glm::vec3(5, 5, 2)), BVolume::OBB);
	CreateEntity("Entity6 OBB", Transform(glm::vec3(0, 5, 2)), BVolume::OBB);

	/* Monkey Model */ {
		auto entity = ECS::Registry::CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Model");
		registry.AddComponentTo<Model>(
			  entity,
			  ModelLoader().LoadModel("res/models/F1.obj", ShaderRegistry::Get().GetShader(ShaderResource::LightedModel))
			  );
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 0, 0)));

		entities.push_back(std::move(entity));
	}

	/* Light Source */ {
		auto entity = ECS::Registry::CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "LightSource");
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 4, 2), glm::vec3(1), glm::vec3(0.5f)));
		registry.AddComponentTo<LightSource>(entity, LightSource(glm::vec3(0.1), glm::vec3(1), glm::vec3(0.4)));

		entities.push_back(std::move(entity));
	}

}

void TestingScene::CollisionTest() {
	for (auto i = 0; i < entities.size(); i++) {
		auto *bvolume1 = registry.GetComponentFrom<ColliderComponent>(entities[i]);
		auto *info1 = registry.GetComponentFrom<CollisionInfo>(entities[i]);

		if (!bvolume1 || !info1) {
			continue;
		}

		for (auto j = i + 1; j < entities.size(); j++) {
			auto *bvolume2 = registry.GetComponentFrom<ColliderComponent>(entities[j]);
			auto *info2 = registry.GetComponentFrom<CollisionInfo>(entities[j]);

			if (!bvolume2 || !info2) {
				continue;
			}

			if (CollisionDetector::AreColliding(
				  bvolume1->GetBoundingVolume(),
				  bvolume2->GetBoundingVolume())) {
				info1->CollisionFlag = info2->CollisionFlag = true;
			}
		}
	}
}