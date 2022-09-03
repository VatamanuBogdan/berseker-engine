#pragma once
#include "Scene/Scene.h"
#include "Rendering/GridLine.hpp"
#include "Physics/Collider.h"

class GJKScene final : public Scene {
public:
	explicit GJKScene(Application *application);

	void Init() override;
	void Deinit() override;

	void OnPreUpdate() override;
	void OnUpdate() override;
	void OnPostUpdate() override;

	void OnPreRendering() override;
	void OnPostRendering() override;

private:
	void InitEntities();

private:
	std::shared_ptr<GridLine> gridLine;
	GJKCollisionStrategy collisionStrategy;
};

