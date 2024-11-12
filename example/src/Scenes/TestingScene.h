#pragma once

#include <Core/Window/Window.h>
#include <Rendering/GraphicsAPI/ShaderProgram.h>
#include <Rendering/Primitives/Primitives.h>
#include <Rendering/GridLine.hpp>
#include <Rendering/Model.h>
#include "Physics/Boundings/BVolumes.h"
#include <Ecs/Registry.h>
#include <Scene/Components/Components.h>
#include <Scene/Scene.h>

#include <vector>
#include <memory>

class TestingScene final : public Scene {
public:
	explicit TestingScene(Application *application)
		: Scene(application) {
	}

	void Init() override;
	void Deinit() override;

	void OnPreUpdate() override;
	void OnUpdate() override;
	void OnPostUpdate() override;

	void OnPreRendering() override;
	void OnPostRendering() override;

private:
	void CreateEntity(const char *identifier, const Transform& transform, BVolume::Type type);

	void InitEntities();
	void CollisionTest();

private:
	std::shared_ptr<GridLine> gridLine;
};