#include "BVHScene.h"

#include "Rendering/Renderer.h"
#include "Scene/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"
#include "ResourceLoaders/ShaderRegistry.h"
#include "Scene/Components/Components.h"

#include <glm/glm.hpp>


void BVHScene::Init() {
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
}

void BVHScene::Deinit() {
	camera.reset();
}

void BVHScene::OnPreUpdate() {
	std::vector<BVH::BoundedEntity> boundedEntities;

	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *aabb = registry.GetComponentFrom<AABB>(entity);
		if (transform && aabb) {
			AABB transformedAABB = *aabb;
			transformedAABB.Apply(*transform);
			boundedEntities.emplace_back(std::make_pair(entity, transformedAABB));
		}
	}
	bvh.BuildFrom(std::move(boundedEntities));
}

void BVHScene::OnUpdate() {
	static bool upFlag = true;
	static bool downFlag = true;

	auto &input = application->GetMainWindow()->GetInput();
	if (input.GetStateForKey(KeyboardKey::DOWN) == KeyState::Released) {
		if (upFlag) {
			bvhDepthToDisplay = std::max(bvhDepthToDisplay - 1, (int) 0);
			std::cout << bvhDepthToDisplay << std::endl;
		}
		upFlag = false;
	} else {
		upFlag = true;
	}

	if (input.GetStateForKey(KeyboardKey::UP) == KeyState::Pressed) {
		if (downFlag) {
			bvhDepthToDisplay = std::min(bvhDepthToDisplay + 1, (int) bvh.GetDepth() - 1);
			std::cout << bvhDepthToDisplay << std::endl;
		}
		downFlag = false;
	} else {
		downFlag = true;
	}
}

void BVHScene::OnPostUpdate() {
}

void BVHScene::OnPreRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<ModelComponent>(entity);

		if (transform && model) {
			Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
		}

	}
}

void BVHScene::OnPostRendering() {
	RenderBVH(bvh.GetRoot());
	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void BVHScene::InitEntities() {
	std::shared_ptr<Model> model = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/Monkey.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::Model3D)
					)
			)
	);

	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey0");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 0, 0)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}


	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey1");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 4, 2)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}

	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey2");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(8, 0, -4)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}

	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey3");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 4, -10)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}

	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey4");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(-8, 0, 2)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}

	{
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "Monkey5");
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(2, 2, 0)));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}
}

void BVHScene::RenderBVH(BVH::BVHNode *node, int depth) {
	if (!node) {
		return;
	}

	Color color(0, 1, 1);
	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			if (depth == bvhDepthToDisplay) {
				Renderer::RenderBVolume(bvh.GetBoundingUsingId(i), color);
			}
		}
		return;
	}
	if (depth == bvhDepthToDisplay) {
		Renderer::RenderBVolume(node->Bounding, color);
	}
	RenderBVH(node->Internal.LeftChildren, depth + 1);
	RenderBVH(node->Internal.RightChildren, depth + 1);
}
