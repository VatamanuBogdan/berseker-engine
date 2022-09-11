#pragma once
#include "Scene/Scene.h"
#include "Rendering/GridLine.hpp"
#include "Physics/Collider.h"
#include "Physics/Partitioning/BVH.h"

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
	void RenderBVH(BVH::BVHNode *node, int depth);

private:
	BVH bvh;
	bool displayEntireBvh = false;
	bool displayDecomposition = false;
	bool displayDecompositionOnlyCollision = false;
	int bvhDepthToDisplay = 0;
	std::shared_ptr<GridLine> gridLine;
	GJKCollisionStrategy collisionStrategy;
};

