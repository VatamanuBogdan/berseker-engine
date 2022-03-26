#pragma once
#include "Scene.h"
#include "Window/Window.h"
#include "Rendering/GridLine.hpp"
#include "Rendering/Model.h"
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include <memory>

class MainScene final : public Scene {
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
};
