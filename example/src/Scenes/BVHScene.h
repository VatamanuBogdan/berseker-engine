#pragma once
#include "Scene/Scene.h"
#include "Application.h"
#include "Rendering/GridLine.hpp"
#include "Physics/Partitioning/BVH.h"
#include "CollisionScene.h"

enum class CullingResult {
	Inside, Outside, Intersecting
};


class BVHScene: public CollisionScene {
public:
	explicit BVHScene(Application *application)
	: CollisionScene(application) {
	}

	void Init() override;
	void Deinit() override;

	void OnPreUpdate() override;
	void OnUpdate() override;
	void OnPostUpdate() override;

	void OnPreRendering() override;
	void OnPostRendering() override;

private:
	void NormalRendering();
	void CullingRendering();
	void BVHCullingRendering();
	void BVHCullingRenderingHelper1(BVH::BVHNode *node);
	void BVHCullingRenderingHelper2(BVH::BVHNode *node);
	static CullingResult AABBvsFrustum(const glm::mat4& mvp, const AABB& aabb);

private:
	void InitEntities();
	void RenderBVH(BVH::BVHNode *root, int &depth);

private:
	glm::mat4 frustumVP;
	BVH bvh;
	std::shared_ptr<GridLine> gridLine;
	int bvhDepthToDisplay = 0;
};
