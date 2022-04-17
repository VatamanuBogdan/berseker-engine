#pragma once
#include <Window/Window.h>
#include <Rendering/GraphicsAPI/ShaderProgram.h>
#include <Rendering/Primitives/Primitives.h>
#include <Rendering/GridLine.hpp>
#include <Rendering/Model.h>
#include <UI/UIRenderer.h>
#include <UI/Controls/UITransform3D.h>
#include <Scene.h>
#include <Utils.h>

#include <memory>
#include <imgui.h>

class Scene1Controller;


class Scene1 final : public Scene {
public:
	void Init() override;
	void Deinit() override;

	void OnPreUpdate() override;
	void OnUpdate() override;
	void OnPostUpdate() override;

	void OnPreRendering() override;
	void OnPostRendering() override;

private:
	std::shared_ptr<Camera> 		camera = nullptr;
	std::shared_ptr<GridLine> 		gridLine;
	std::shared_ptr<ShaderProgram>	shader;
	std::shared_ptr<Model>			model;
	std::shared_ptr<UIController>		uiController;
	Lateinit<CubeProps>			cubeProps;

private:
	friend class Scene1Controller;
};


class CubeTransformAdapter final : public UITransform3D::Adapter {
public:
	explicit CubeTransformAdapter(CubeProps *props);

	void SetPosition(const glm::vec3 &position) override;
	void SetRotation(const glm::vec3 &rotation) override;
	void SetScale(const glm::vec3 &scale) override;

	const glm::vec3 &GetPosition() override;
	const glm::vec3 &GetRotation() override;
	const glm::vec3 &GetScale() override;

private:
	CubeProps *props = nullptr;
};


class Scene1Controller final : public UIController {
public:
	explicit Scene1Controller(Scene1 *scene1);

	void OnUpdate() final;
	void OnRendering() final;

private:
	CubeTransformAdapter 	 adapter;
	ImFont 			*jetbrainsMono;
	Scene1 			*scene1;
};
