#pragma once

#include <Window/Window.h>
#include <Rendering/GraphicsAPI/ShaderProgram.h>
#include <Rendering/Primitives/Primitives.h>
#include <Rendering/GridLine.hpp>
#include <Rendering/Model.h>
#include <Collision/BVolumes.h>
#include <ECS/ECS.h>
#include <ECS/Components/Components.h>
#include <Scene.h>

#include <vector>
#include <memory>

class BVolumesScene final : public Scene {
public:
	explicit BVolumesScene(Application *application)
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
	std::shared_ptr<GridLine> 			gridLine;
	std::shared_ptr<ShaderProgram>		lightedModelShader;
	std::shared_ptr<ShaderProgram>		lightShader;
};