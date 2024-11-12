#pragma once
#include "Scene/Scene.h"
#include "Physics/Partitioning/BVH.h"

class CollisionScene: public Scene {
public:
	explicit CollisionScene(Application *application);

	void CollisionComputingComputingUsingBruteForce();
	void CollisionComputingUsingBVH(BVH &bvh);

protected:
	void CollisionOverBVH(BVH::BVHNode *node);

protected:
	AABB testedBVolume;
	BVH *bvh;
};


