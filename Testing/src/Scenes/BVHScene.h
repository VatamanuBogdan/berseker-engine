#pragma once
#include "Scene/Scene.h"
#include "Application.h"
#include "Rendering/GridLine.hpp"
#include "Physics/Partitioning/BVH.h"

class BVHScene: public Scene{
public:
	explicit BVHScene(Application *application)
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
	void InitEntities();
	void RenderBVH(BVH::BVHNode *root, int depth = 0);

private:
	BVH bvh;
	std::shared_ptr<GridLine> gridLine;
	int bvhDepthToDisplay = 0;
};
