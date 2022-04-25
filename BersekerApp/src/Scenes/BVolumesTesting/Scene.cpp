#include "Scene.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.hpp"
#include "Rendering/Primitives/Renderer.h"

#include <glm/glm.hpp>

#include <iostream>
#include <optional>


namespace BVolumesTesting {

	void BoundedPrimitivesStorage::Add(const char *id, PrimitivePropsPtr &&props, BoundedVolumePtr &&bVolume) {
		auto &primitive = primitives.emplace_back();
		primitive.ID = id;
		primitive.Props = std::move(props);
		primitive.BVolume = std::move(bVolume);
	}

	BoundedPrimitivesAdapter::BoundedPrimitivesAdapter(BoundedPrimitivesStorage *storage)
		: storage(storage) {
	}

	void BoundedPrimitivesAdapter::SetPosition(const glm::vec3 &position, int index) {
		auto &primitive = storage->primitives[index];
		primitive.Props->Position = position;

		switch (primitive.BVolume->Type) {
			case BVolumes::BoundingVolume::TypeTag::AABB:
				reinterpret_cast<BVolumes::AABB&>(*primitive.BVolume).Position = position;
				break;
			case BVolumes::BoundingVolume::TypeTag::Sphere:
				reinterpret_cast<BVolumes::Sphere&>(*primitive.BVolume).Position = position;
				break;
			case BVolumes::BoundingVolume::TypeTag::OBB:
				reinterpret_cast<BVolumes::OBB&>(*primitive.BVolume).Position = position;
				break;
		}
	}

	void BoundedPrimitivesAdapter::SetRotation(const glm::vec3 &rotation, int index) {
		auto &primitive = storage->primitives[index];

		switch (primitive.BVolume->Type) {
			case BVolumes::BoundingVolume::TypeTag::Sphere:
				primitive.Props->Rotation = rotation;
				break;
			case BVolumes::BoundingVolume::TypeTag::OBB:
				primitive.Props->Rotation = rotation;
				reinterpret_cast<BVolumes::OBB&>(*primitive.BVolume).Directions.SetRotation(rotation);
				break;
			default:
				break;
		}
	}

	void BoundedPrimitivesAdapter::SetScale(const glm::vec3 &scale, int index) {
		auto &primitive = storage->primitives[index];

		switch (primitive.BVolume->Type) {
			case BVolumes::BoundingVolume::TypeTag::AABB:
				primitive.Props->Scale = scale;
				reinterpret_cast<BVolumes::AABB&>(*primitive.BVolume).HalfWidths = scale / glm::vec3(2);
				break;
			case BVolumes::BoundingVolume::TypeTag::Sphere:
				primitive.Props->Scale = glm::vec3(scale.x);
				reinterpret_cast<BVolumes::Sphere&>(*primitive.BVolume).Radius = scale.x;
				break;
			case BVolumes::BoundingVolume::TypeTag::OBB:
				primitive.Props->Scale = scale;
				reinterpret_cast<BVolumes::OBB&>(*primitive.BVolume).HalfWidths = scale / glm::vec3(2);
				break;
		}
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetPosition(int index) {
		return storage->primitives[index].Props->Position;
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetRotation(int index) {
		return storage->primitives[index].Props->Rotation;
	}

	const glm::vec3 &BoundedPrimitivesAdapter::GetScale(int index) {
		return storage->primitives[index].Props->Scale;
	}


	int BoundedPrimitivesAdapter::GetSize() {
		return storage->primitives.size();
	}

	const char *BoundedPrimitivesAdapter::GetID(int index) {
		return storage->primitives[index].ID.c_str();
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

		model = std::make_shared<Model>(ModelLoader().LoadModel("res/models/Model.obj"));
		gridLine = std::make_shared<GridLine>();


		storage.Add(
			  "AABB1",
			  std::make_unique<CubeProps>(1, Primitive3DProps(glm::vec3(0), glm::vec3(0), glm::vec3(1),
							     	PrimitiveBody(PrimitiveBody::SHAPED, Color(0, 1.0, 0), 20.0f))),
			  std::make_unique<BVolumes::AABB>(glm::vec3(0, 0, 0), glm::vec3(1))
		);

		storage.Add(
			  "AABB2",
			  std::make_unique<CubeProps>(1, Primitive3DProps(glm::vec3(2, 1, 0), glm::vec3(0), glm::vec3(1),
								PrimitiveBody(PrimitiveBody::SHAPED, Color(0.5, 0.3, 0), 20.0f))),
			  std::make_unique<BVolumes::AABB>(glm::vec3(2, 1, 0), glm::vec3(1))
		);

		storage.Add(
			  "Sphere1",
			  std::make_unique<SphereProps>(1, Primitive3DProps(glm::vec3(2, 1, 2), glm::vec3(0), glm::vec3(2),
								PrimitiveBody(PrimitiveBody::SHAPED, Color(0.2, 0.6, 0.5), 20.0f))),
			  std::make_unique<BVolumes::Sphere>(glm::vec3(2, 1, 0), 2)
		);

		/*
		renderablePrimitives.push_back(
			  std::make_shared<RenderableOBB>(
				    "OBB1", glm::vec3(1, 1, 1), glm::vec3(1),
				    PrimitiveBody(PrimitiveBody::SHAPED, Color(0, 0.2f, 0.9f), 20.0f)
			  )
		);


		renderablePrimitives.push_back(
			  std::make_shared<RenderableOBB>(
				    "OBB2", glm::vec3(1, 1, 1), glm::vec3(1),
				    PrimitiveBody(PrimitiveBody::SHAPED, Color(0.7f, 0.2f, 0.9f), 20.0f)
			  )
		);
		 */


		adapter.Init(&this->storage);

		Renderer::SetCamera(camera);
		uiController = std::make_shared<SceneUIController>(this);
		Application::GetUIRenderer().BindController(uiController);
	}

	void Scene::Deinit() {
		camera.reset();
	}

	void Scene::OnPreUpdate() {
		CollisionUpdate();
	}

	void Scene::OnUpdate() {
		CameraPositionController(Application::GetMainWindow()->GetInput(), *camera);
	}

	void Scene::OnPostUpdate() {

	}

	void Scene::OnPreRendering() {
		for (auto &mesh : model->GetMeshes()) {
			Renderer::SubmitForRendering(
				  &mesh.GetVertexArray(),
				  &mesh.GetIndexBuffer(),
				  shader.get(),
				  glm::scale(glm::mat4(1), glm::vec3(0.5f))
			);
		}
	}

	void Scene::OnPostRendering() {
		for (auto & primitive : storage.primitives) {
			PrimitiveProps *props = primitive.Props.get();
			auto tempColor = props->BodyType.BodyColor;
			if (primitive.CollisionFlag) {
				primitive.Props->BodyType.BodyColor = Color(1, 0, 0);
			}
			if (auto cubeProps = dynamic_cast<CubeProps*>(props)) {
				Renderer::RenderCube(*cubeProps);
			} else if (auto sphereProps = dynamic_cast<SphereProps*>(props)) {
				Renderer::RenderSphere(*sphereProps);
			}
			primitive.Props->BodyType.BodyColor = tempColor;
		}

		gridLine->Draw(camera->GetView(), camera->GetProjection());
	}

	void Scene::CollisionUpdate() {
		for (auto &primitive : storage.primitives) {
			primitive.CollisionFlag = false;
		}

		for (auto i = 0; i < storage.primitives.size(); i++) {
			auto &bVolume1 = *storage.primitives[i].BVolume;

			for (auto j = i + 1; j < storage.primitives.size(); j++) {
				auto &bVolume2 = *storage.primitives[j].BVolume;

				if (BVolumes::CollisionDetector::AreCollidingDispatch(bVolume1, bVolume2)) {
					storage.primitives[i].CollisionFlag = storage.primitives[j].CollisionFlag = true;
				}
			}
		}
	}


}