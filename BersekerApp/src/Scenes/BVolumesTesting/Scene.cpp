#include "Scene.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"
#include "Collision/Collider.h"

#include <glm/glm.hpp>

#include <iostream>
#include <optional>


namespace BVolumesTesting {

	Transform &BoundedPrimitivesAdapter::GetTransformFor(int index) {
		return *(registry->GetComponentFrom<Transform>(entities->at(index)));
	}

	void BoundedPrimitivesAdapter::SetPosition(const glm::vec3 &position, int index) {
		GetTransformFor(index).Position = position;
	}

	void BoundedPrimitivesAdapter::SetRotation(const glm::vec3 &rotation, int index) {
		GetTransformFor(index).Rotation = rotation;
	}

	void BoundedPrimitivesAdapter::SetScale(const glm::vec3 &scale, int index) {
		GetTransformFor(index).Scale = scale;
	}

	void BoundedPrimitivesAdapter::Bind(ECS::Registry* registry, std::vector<ECS::Entity> *entities) {
		this->registry = registry;
		this->entities = entities;
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetPosition(int index) {
		return GetTransformFor(index).Position;
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetRotation(int index) {
		return GetTransformFor(index).Rotation;
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetScale(int index) {
		return GetTransformFor(index).Scale;
	}


	int BoundedPrimitivesAdapter::GetSize() {
		if (entities) {
			return entities->size();
		}
		return 0;
	}

	const char *BoundedPrimitivesAdapter::GetID(int index) {
		return registry->GetComponentFrom<Identifier>(entities->at(index))->Identifier.c_str();
	}


	std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
		os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
		return os;
	}

	std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				os << mat[j][i] << " ";
			os << std::endl;
		}
		return os;
	}

	void CameraPositionController(InputManager &input, Camera &camera) {
		constexpr float velocity = 3.0f;
		constexpr float sensitivity = 1.25f;
		constexpr float constant = 0.01f;

		static std::optional<MousePosition> previousPosition;

		auto position = camera.GetPosition();
		auto rotations = camera.GetRotations();
		auto &direction = camera.GetDirection();

		if (input.GetStateForKey(KeyboardKey::W) == KeyState::Pressed) {
			position += velocity * direction.Forward * constant;
		}
		if (input.GetStateForKey(KeyboardKey::S) == KeyState::Pressed) {
			position -= velocity * direction.Forward * constant;
		}

		if (input.GetStateForKey(KeyboardKey::A) == KeyState::Pressed) {
			position -= velocity * direction.Rightward * constant;
		}
		if (input.GetStateForKey(KeyboardKey::D) == KeyState::Pressed) {
			position += velocity * direction.Rightward * constant;
		}

		if (input.GetStateForKey(KeyboardKey::LEFT_SHIFT) == KeyState::Pressed) {
			position -= velocity * direction.Upward * constant;
		}
		if (input.GetStateForKey(KeyboardKey::SPACE) == KeyState::Pressed) {
			position += velocity * direction.Upward * constant;
		}

		if (input.GetStateForKey(KeyboardKey::KP_8) == KeyState::Pressed) {
			rotations.x -= sensitivity * constant;
		}
		if (input.GetStateForKey(KeyboardKey::KP_2) == KeyState::Pressed) {
			rotations.x += sensitivity * constant;
		}

		if (input.GetStateForKey(KeyboardKey::KP_6) == KeyState::Pressed) {
			rotations.y -= sensitivity * constant;
		}
		if (input.GetStateForKey(KeyboardKey::KP_4) == KeyState::Pressed) {
			rotations.y += sensitivity * constant;
		}

		if (input.GetStateForKey(KeyboardKey::TAB) == KeyState::Pressed) {
			std::cout << "Camera Properties: " << std::endl;
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "[Position]: " << position << std::endl;
			std::cout << "[Rotations]: " << rotations << std::endl;
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "[Upward]: " << direction.Upward << std::endl;
			std::cout << "[Forward]: " << direction.Forward << std::endl;
			std::cout << "[Rightward]: " << direction.Rightward << std::endl;
		}

		auto mousePosition = input.GetMousePosition();
		if (input.GetStateForKey(KeyboardKey::F1) == KeyState::Pressed) {
			std::cout << "Mouse position: " << mousePosition.xPosition << " " << mousePosition.yPosition
				    << std::endl;
		}

		camera.SetPositioning(position, rotations);
	}


	ECS::Entity *cube1 = nullptr;
	ECS::Entity *cube2 = nullptr;

	void Scene::Init() {
		auto &window = Application::GetMainWindow();
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

		entities.reserve(1024);

		gridLine = std::make_shared<GridLine>();

		{
			auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
			auto entity = ECS::Registry::CreateEntity();



			registry.AddComponentTo<Identifier>(entity).Identifier = "Monkey1";
			registry.AddComponentTo<Transform>(entity, glm::vec3(0));
			registry.AddComponentTo<Model>(entity, ModelLoader().LoadModel("res/models/Monkey.obj"));
			registry.AddComponentTo<Collider>(entity, std::move(collider));
			registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));

			entities.push_back(std::move(entity));
			cube1 = &entities.back();
		}

		{
			auto[collider, renderableCollider] = ColliderLoader().LoadCollider("res/colliders/Monkey.collider");
			auto entity = ECS::Registry::CreateEntity();



			registry.AddComponentTo<Identifier>(entity).Identifier = "Monkey2";
			registry.AddComponentTo<Transform>(entity, glm::vec3(0, 1, 0));
			registry.AddComponentTo<Model>(entity, ModelLoader().LoadModel("res/models/Monkey.obj"));
			registry.AddComponentTo<Collider>(entity, std::move(collider));
			registry.AddComponentTo<RenderableCollider>(entity, std::move(renderableCollider));

			entities.push_back(std::move(entity));
			cube2 = &entities.back();
		}

		Renderer::SetCamera(camera);
		uiController = std::make_shared<SceneUIController>(this);
		Application::GetUIRenderer().BindController(uiController);

		adapter.Bind(&registry, &entities);
		// InitEntities();
	}

	void Scene::Deinit() {
		camera.reset();
	}

	void Scene::OnPreUpdate() {
		{
			auto *collider1 = registry.GetComponentFrom<Collider>(*cube1);
			auto *transform1 = registry.GetComponentFrom<Transform>(*cube1);

			auto *collider2 = registry.GetComponentFrom<Collider>(*cube2);
			auto *transform2 = registry.GetComponentFrom<Transform>(*cube2);

			collider1->SetModel(transform1->ComputeTransformMatrix());
			collider2->SetModel(transform2->ComputeTransformMatrix());

			if (GJKCollisionStrategy().AreColliding(*collider1, *collider2)) {
				std::cout << "Are colliding" << std::endl;
			}
		}




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

	void Scene::OnUpdate() {
		CameraPositionController(Application::GetMainWindow()->GetInput(), *camera);
	}

	void Scene::OnPostUpdate() {

	}

	void Scene::OnPreRendering() {
		for (auto &entity : entities) {
			auto *model = registry.GetComponentFrom<Model>(entity);
			auto *transform = registry.GetComponentFrom<Transform>(entity);
			if (!model || !transform) {
				continue;
			}
			auto modelMatrix = transform->ComputeTransformMatrix();
			for (auto &mesh : model->GetMeshes()) {
				Renderer::SubmitForRendering(
					  &mesh.GetVertexArray(), &mesh.GetIndexBuffer(), shader.get(), modelMatrix
				);
			}
		}
	}

	void Scene::OnPostRendering() {
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

			auto *renderableCollider = registry.GetComponentFrom<RenderableCollider>(entity);
			auto *transform = registry.GetComponentFrom<Transform>(entity);
			if (renderableCollider && transform) {
				// Renderer::RenderCollider(*renderableCollider, transform->ComputeTransformMatrix());
			}
		}

		gridLine->Draw(camera->GetView(), camera->GetProjection());
	}

	void Scene::CreateEntity(const char *identifier, const Transform &transform, BVolumes::BVolume::Type type) {
		auto entity = ECS::Registry::CreateEntity();

		registry.AddComponentTo<Identifier>(entity).Identifier = identifier;
		registry.AddComponentTo<Transform>(entity, transform);
		registry.AddComponentTo<ColliderComponent>(entity, transform, type);
		registry.AddComponentTo<CollisionInfo>(entity);

		entities.push_back(std::move(entity));
	}

	void Scene::InitEntities() {
		CreateEntity("Entity1 AABB", Transform(glm::vec3(0, 0, 0)), BVolumes::BVolume::AABB);
		CreateEntity("Entity2 AABB", Transform(glm::vec3(1, 2, 0)), BVolumes::BVolume::AABB);
		CreateEntity("Entity3 Sphere", Transform(glm::vec3(3, -1, 0)), BVolumes::BVolume::Sphere);
		CreateEntity("Entity4 Sphere", Transform(glm::vec3(-2, 3, 1)), BVolumes::BVolume::Sphere);
		CreateEntity("Entity5 OBB", Transform(glm::vec3(5, 5, 2)), BVolumes::BVolume::OBB);
		CreateEntity("Entity6 OBB", Transform(glm::vec3(0, 5, 2)), BVolumes::BVolume::OBB);
	}

	void Scene::CollisionTest() {
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

}