#include "GJKScene.h"
#include "Rendering/Renderer.h"
#include "Application.h"
#include "Rendering/GridLine.hpp"
#include "Scene/Components/Components.h"


GJKScene::GJKScene(Application *application)
	: Scene(application) {
}

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

			if (collisionStrategy.AreColliding(**collider1, **collider2)) {
				std::cout << "Collision" << std::endl;
			}
		}
	}
}

void GJKScene::OnUpdate() {

}

void GJKScene::OnPostUpdate() {

}

void GJKScene::OnPreRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<ModelComponent>(entity);

		if (transform && model) {
			Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
		}
	}
}

void GJKScene::OnPostRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *renderableCollider = registry.GetComponentFrom<RenderableCollider>(entity);

		if (transform && renderableCollider) {
			Renderer::RenderCollider(*renderableCollider, transform->ComputeTransformMatrix());
		}
	}

	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void GJKScene::InitEntities() {
	std::shared_ptr<Model> model = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/Monkey.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::Model3D)
			    )
		  )
	);

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Cube1");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform());
		entities.push_back(std::move(entity));
	}

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Cube2");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform());
		entities.push_back(std::move(entity));
	}
}




