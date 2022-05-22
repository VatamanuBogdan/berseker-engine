#include "BVolumesScene.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"

#include <glm/glm.hpp>

#include <iostream>


void BVolumesScene::Init() {
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
	shader = std::make_shared<ShaderProgram>(
		  ShaderProgram::LoadFrom(
			    "res/shaders/3DModel.vert.glsl",
			    "res/shaders/3DModel.frag.glsl"
		  )
	);

	model = std::make_shared<Model>(ModelLoader().LoadModel("res/models/Monkey.obj"));
	gridLine = std::make_shared<GridLine>();

	Renderer::SetCamera(camera);
	InitEntities();
}

void BVolumesScene::Deinit() {
	camera.reset();
}

void BVolumesScene::OnPreUpdate() {
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

void BVolumesScene::OnUpdate() {
}

void BVolumesScene::OnPostUpdate() {
}

void BVolumesScene::OnPreRendering() {
	for (auto &mesh : model->GetMeshes()) {
		Renderer::SubmitForRendering(
			  &mesh.GetVertexArray(),
			  &mesh.GetIndexBuffer(),
			  shader.get(),
			  glm::scale(glm::mat4(1), glm::vec3(0.5f))
		);
	}
}

void BVolumesScene::OnPostRendering() {
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

	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void BVolumesScene::CreateEntity(const char *identifier, const Transform &transform, BVolumes::BVolume::Type type) {
	auto entity = ECS::Registry::CreateEntity();

	registry.AddComponentTo<Identifier>(entity).Identifier = identifier;
	registry.AddComponentTo<Transform>(entity, transform);
	registry.AddComponentTo<ColliderComponent>(entity, transform, type);
	registry.AddComponentTo<CollisionInfo>(entity);

	entities.push_back(std::move(entity));
}

void BVolumesScene::InitEntities() {
	CreateEntity("Entity1 AABB", Transform(glm::vec3(0, 0, 0)), BVolumes::BVolume::AABB);
	CreateEntity("Entity2 AABB", Transform(glm::vec3(1, 2, 0)), BVolumes::BVolume::AABB);
	CreateEntity("Entity3 Sphere", Transform(glm::vec3(3, -1, 0)), BVolumes::BVolume::Sphere);
	CreateEntity("Entity4 Sphere", Transform(glm::vec3(-2, 3, 1)), BVolumes::BVolume::Sphere);
	CreateEntity("Entity5 OBB", Transform(glm::vec3(5, 5, 2)), BVolumes::BVolume::OBB);
	CreateEntity("Entity6 OBB", Transform(glm::vec3(0, 5, 2)), BVolumes::BVolume::OBB);
}

void BVolumesScene::CollisionTest() {
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

			if (BVolumes::CollisionDetector::AreColliding(
				  bvolume1->GetBoundingVolume(),
				  bvolume2->GetBoundingVolume())) {
				info1->CollisionFlag = info2->CollisionFlag = true;
			}
		}
	}
}