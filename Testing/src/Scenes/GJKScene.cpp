#include "GJKScene.h"
#include "Rendering/Renderer.h"
#include "Application.h"
#include "Rendering/GridLine.hpp"
#include "Scene/Components/Components.h"
#include "Physics/Boundings/BVolumes.h"
#include "Timer.h"
#include "Physics/Partitioning/BVH.h"

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
	std::vector<BVH::BoundedEntity> boundedEntities;

	for (auto &entity : entities) {
		auto *transform = registry.GetComponentFrom<Transform>(entity);
		auto *modelComponent = registry.GetComponentFrom<ModelComponent>(entity);
		if (transform && modelComponent) {
			AABB transformedAABB = (*modelComponent)->GetAABB();
			transformedAABB.Apply(*transform);
			boundedEntities.emplace_back(std::make_pair(entity, transformedAABB));
		}
	}
	bvh.BuildFrom(std::move(boundedEntities));


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
	static bool upFlag = true;
	static bool downFlag = true;
	static bool displayEntireBvhFlag = true;
	static bool displayDecompositionFlag = true;

	auto &input = application->GetMainWindow()->GetInput();
	if (input.GetStateForKey(KeyboardKey::DOWN) == KeyState::Released) {
		if (upFlag) {
			bvhDepthToDisplay = std::max(bvhDepthToDisplay - 1, (int) 0);
			std::cout << bvhDepthToDisplay << std::endl;
			upFlag = false;
		}
	} else {
		upFlag = true;
	}

	if (input.GetStateForKey(KeyboardKey::UP) == KeyState::Pressed) {
		if (downFlag) {
			bvhDepthToDisplay = std::min(bvhDepthToDisplay + 1, (int) bvh.GetDepth());
			std::cout << bvhDepthToDisplay << std::endl;
			downFlag = false;
		}
	} else {
		downFlag = true;
	}

	if (input.GetStateForKey(KeyboardKey::L) == KeyState::Pressed) {
		if (displayEntireBvhFlag) {
			displayEntireBvh = !displayEntireBvh;
			displayEntireBvhFlag = false;
		}
	} else {
		displayEntireBvhFlag = true;
	}


	if (input.GetStateForKey(KeyboardKey::P) == KeyState::Pressed) {
		if (displayDecompositionFlag) {
			displayDecomposition = !displayDecomposition;
			displayDecompositionFlag = false;
		}
	} else {
		displayDecompositionFlag = true;
	}

	if (input.GetStateForKey(KeyboardKey::K) == KeyState::Pressed) {
		displayDecompositionOnlyCollision = !displayDecompositionOnlyCollision;
	}
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
		auto *renderableCollider = registry.GetComponentFrom<RenderableCollider>(entity);;

		if (transform && renderableCollider && displayDecomposition) {
			Renderer::RenderCollider(*renderableCollider, *transform, collidedComponents[entity.GetId()], !displayDecompositionOnlyCollision);
		}
	}

	int depth = 0;
	RenderBVH(bvh.GetRoot(), depth);

	gridLine->Draw(camera->GetView(), camera->GetProjection());
}

void GJKScene::InitEntities() {
	std::shared_ptr<Model> dragon = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/StanfordDragon.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	std::shared_ptr<Model> armadillo = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/StanfordArmadillo.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	std::shared_ptr<Model> bunny = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/StanfordBunny.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	std::shared_ptr<Model> cube = std::make_shared<Model>(
		  std::move(
			    ModelLoader().LoadModel(
					"res/models/Cube.obj",
					ShaderRegistry::Get().GetShader(ShaderResource::LightedModel)
			    )
		  )
	);

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/StanfordDragon.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Dragon");
		registry.AddComponentTo<ModelComponent>(entity, dragon);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0, 2, 0), glm::vec3(0), glm::vec3(1.5)));
		entities.push_back(std::move(entity));
	}

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/StanfordArmadillo.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Armadillo");
		registry.AddComponentTo<ModelComponent>(entity, armadillo);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(-5, 3, -5), glm::vec3(0, 3.14, 0), glm::vec3(2)));
		entities.push_back(std::move(entity));
	}

	{
		auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/StanfordBunny.collider");
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Bunny");
		registry.AddComponentTo<ModelComponent>(entity, bunny);
		registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));
		registry.AddComponentTo<ColliderComponent>(entity, std::make_shared<Collider>(std::move(collider)));
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(5, 5, 5)));
		entities.push_back(std::move(entity));
	}

	{
		auto entity = registry.CreateEntity();

		registry.AddComponentTo<Identifier>(entity, "Plane");
		registry.AddComponentTo<ModelComponent>(entity, cube);
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(0), glm::vec3(0), glm::vec3(10, 0.1, 10)));
		entities.push_back(std::move(entity));
	}


	/* Light Source2 */ {
		auto entity = registry.CreateEntity();
		registry.AddComponentTo<Identifier>(entity, "LightSource2");
		registry.AddComponentTo<Transform>(entity, Transform(glm::vec3(4, 4, 0), glm::vec3(1), glm::vec3(0.5f)));
		registry.AddComponentTo<LightSource>(entity, LightSource(glm::vec3(0.1), glm::vec3(1), glm::vec3(0.4)));

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

static Color colors[] = {
	  Color(0.16470588235294117, 0.8509803921568627, 0.10196078431372549),
	  Color(0.5254901960784314, 0.10196078431372549, 0.8509803921568627),
	  Color(0.8509803921568627, 0.30196078431372547, 0.10196078431372549),
	  Color(0.10196078431372549, 0.5137254901960784, 0.8509803921568627),
	  Color(0.8392156862745098, 0.10196078431372549, 0.8509803921568627),
	  Color(0.8392156862745098, 0.8509803921568627, 0.15294117647058825),
	  Color(0.15294117647058825, 0.3058823529411765, 0.8509803921568627)
};


void GJKScene::RenderBVH(BVH::BVHNode *node, int depth) {
	if (!node) {
		return;
	}

	depth++;

	Color &color = colors[depth % (sizeof(colors) / sizeof(color))];
	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			if (depth == bvhDepthToDisplay || displayEntireBvh) {
				Renderer::RenderBVolume(bvh.GetBoundingUsingId(i), color);
			}
		}
		return;
	}
	if (depth == bvhDepthToDisplay || displayEntireBvh) {
		Renderer::RenderBVolume(node->Bounding, color);
	}
	RenderBVH(node->Internal.LeftChildren, depth + 1);
	RenderBVH(node->Internal.RightChildren, depth + 1);
}




