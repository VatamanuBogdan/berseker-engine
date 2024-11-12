#include "BVHScene.h"

#include "Rendering/Renderer.h"
#include "Scene/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"
#include "ResourceLoaders/ShaderRegistry.h"
#include "Scene/Components/Components.h"
#include "../Utils/Random.h"
#include "../Utils/Timer.h"

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
	Timer timer;

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

	// timer.Start();
	// CollisionComputingComputingUsingBruteForce();
	// CollisionComputingUsingBVH(bvh);
	// std::cout << timer.Stop() << std::endl;
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

#define ENTITES_NUM 16384

void BVHScene::OnPostUpdate() {
	Timer timer;
	timer.Start();
	CollisionComputingComputingUsingBruteForce();
	// CollisionComputingUsingBVH(bvh);
	std::cout << "Collision Time:" << timer.Stop() << std::endl;
}

void BVHScene::OnPreRendering() {
	Timer timer;
	{
		timer.Start();
		NormalRendering();
		// std::cout << "Normal Rendering:" << timer.Stop() << std::endl;
	}
	/*
	{
		timer.Start();
		CullingRendering();
		std::cout << "Culling Rendering:" << timer.Stop() << std::endl;
	}
	*/
	/*
	{
		timer.Start();
		BVHCullingRendering();
		std::cout << "BVHCulling Rendering:" << timer.Stop() << std::endl;
	}
	*/
}

void BVHScene::OnPostRendering() {
	gridLine->Draw(camera->GetView(), camera->GetProjection());
}


void BVHScene::NormalRendering() {
	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *model = registry.GetComponentFrom<ModelComponent>(entity);

		if (transform && model) {
			Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
		}
	}
}

void BVHScene::CullingRendering() {
	Camera camera = GetCamera();
	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	glm::mat4 modelMatrix;

	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *aabb = registry.GetComponentFrom<AABB>(entity);

		if (transform && aabb) {
			modelMatrix = transform->ComputeTransformMatrix();

			if (AABBvsFrustum(vp * modelMatrix, *aabb) != CullingResult::Outside) {
				auto *model = registry.GetComponentFrom<ModelComponent>(entity);
				if (model) {
					Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
				}
			}
		}
	}
}

void BVHScene::BVHCullingRendering() {
	Camera camera = GetCamera();
	frustumVP = camera.GetProjection() * camera.GetView();
	BVHCullingRenderingHelper1(bvh.GetRoot());
}

void BVHScene::BVHCullingRenderingHelper1(BVH::BVHNode *node) {
	if (!node) {
		return;
	}

	auto cullingResult = AABBvsFrustum(frustumVP, node->Bounding);
	if (cullingResult == CullingResult::Outside) {
		return;
	}

	if (cullingResult == CullingResult::Inside) {
		BVHCullingRenderingHelper2(node);
		return;
	}

	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			auto entity = bvh.GetEntityUsingId(i);
			auto *transform = registry.GetComponentFrom<Transform>(entity);
			auto *model = registry.GetComponentFrom<ModelComponent>(entity);

			if (transform && model) {
				Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
			}
		}
		return;
	}

	BVHCullingRenderingHelper1(node->Internal.LeftChildren);
	BVHCullingRenderingHelper1(node->Internal.RightChildren);
}

void BVHScene::BVHCullingRenderingHelper2(BVH::BVHNode *node) {
	if (!node) {
		return;
	}

	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			auto entity = bvh.GetEntityUsingId(i);
			auto *transform = registry.GetComponentFrom<Transform>(entity);
			auto *model = registry.GetComponentFrom<ModelComponent>(entity);

			if (transform && model) {
				Renderer::SubmitModelForRendering(model->get(), transform->ComputeTransformMatrix());
			}
		}
		return;
	}
	BVHCullingRenderingHelper2(node->Internal.LeftChildren);
	BVHCullingRenderingHelper2(node->Internal.RightChildren);
}


CullingResult BVHScene::AABBvsFrustum(const glm::mat4& mvp, const AABB& aabb) {
	glm::vec3 min = aabb.GetPosition() - aabb.GetWidths() / 2.0f;
	glm::vec3 max = aabb.GetPosition() + aabb.GetWidths() / 2.0f;


	glm::vec4 corners[8] = {
		  {min.x, min.y, min.z, 1.0},
		  {max.x, min.y, min.z, 1.0},
		  {min.x, max.y, min.z, 1.0},
		  {max.x, max.y, min.z, 1.0},

		  {min.x, min.y, max.z, 1.0},
		  {max.x, min.y, max.z, 1.0},
		  {min.x, max.y, max.z, 1.0},
		  {max.x, max.y, max.z, 1.0},
	};

	bool intersecting = false;
	bool inside = true;

	for (auto & i : corners) {
		glm::vec4 corner = mvp * i;

		bool isCornerInside = (-corner.w <= corner.x && corner.x <= corner.w) &&
					    (-corner.w <= corner.y && corner.y <= corner.w) &&
					    (0.0f <= corner.z && corner.z <= corner.w);

		intersecting = intersecting || isCornerInside;
		inside = inside && isCornerInside;
	}

	if (inside) {
		return CullingResult::Inside;
	}

	return intersecting ? CullingResult::Intersecting : CullingResult::Outside;
}

void BVHScene::InitEntities() {
	std::shared_ptr<Model> model = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"resources/models/Monkey.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::Model3D)
					)
			)
	);
	float cubeSize = 100;
	Random random(-cubeSize, cubeSize);
	size_t entitiesNum = ENTITES_NUM;

	for (int i = 0; i < entitiesNum; i++) {
		auto entity = registry.CreateEntity();
		glm::vec3 position = {random.Generate(), random.Generate(), random.Generate()};

		registry.AddComponentTo<Identifier>(entity, ("Monkey" + std::to_string(i)).c_str());
		registry.AddComponentTo<ModelComponent>(entity, model);
		registry.AddComponentTo<Transform>(entity, Transform(position));
		registry.AddComponentTo<AABB>(entity, model->GetAABB());
		entities.push_back(std::move(entity));
	}


}

static Color colors[] = {
	Color(0.16470588235294117, 0.8509803921568627, 0.10196078431372549),
	Color(0.5254901960784314, 0.10196078431372549, 0.8509803921568627),
	Color(0.8509803921568627, 0.30196078431372547, 0.10196078431372549),
	Color(0.10196078431372549, 0.5137254901960784, 0.8509803921568627),
	Color(0.8392156862745098, 0.10196078431372549, 0.8509803921568627),
	Color(0.8392156862745098, 0.8509803921568627, 0.15294117647058825),
	Color(0.15294117647058825, 0.3058823529411765, 0.8509803921568627)
};

void BVHScene::RenderBVH(BVH::BVHNode *node, int &depth) {
	if (!node) {
		return;
	}

	depth++;

	Color &color = colors[depth % (sizeof(colors) / sizeof(Color))];
	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			// if (depth == bvhDepthToDisplay) {
				Renderer::RenderBVolume(bvh.GetBoundingUsingId(i), color);
			// }
		}
		return;
	}
	// if (depth == bvhDepthToDisplay) {
		Renderer::RenderBVolume(node->Bounding, color);
	// }
	RenderBVH(node->Internal.LeftChildren, depth);
	RenderBVH(node->Internal.RightChildren, depth);
}
