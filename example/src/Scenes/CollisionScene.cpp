#include "CollisionScene.h"
#include "Physics/Boundings/BVolumes.h"


CollisionScene::CollisionScene(Application *application)
	: Scene(application), testedBVolume(glm::vec3(0), glm::vec3(1)) {
}

void CollisionScene::CollisionComputingComputingUsingBruteForce() {
	for (int i = 0; i < entities.size(); i++) {
		AABB *aabb1 = registry.GetComponentFrom<AABB>(entities[i]);
		if (!aabb1) {
			continue;
		}

		for (int j = i + 1; j < entities.size(); j++) {
			AABB *aabb2 = registry.GetComponentFrom<AABB>(entities[j]);
			if (!aabb2) {
				continue;
			}
			CollisionDetector::AreColliding(*aabb1, *aabb2);
		}
	}
}

void CollisionScene::CollisionComputingUsingBVH(BVH &bvh) {
	this->bvh = &bvh;
	for (int i = 0; i < entities.size(); i++) {
		AABB *aabb = registry.GetComponentFrom<AABB>(entities[i]);
		if (!aabb) {
			continue;
		}
		testedBVolume = *aabb;
		CollisionOverBVH(bvh.GetRoot());
	}
}

void CollisionScene::CollisionOverBVH(BVH::BVHNode *node) {
	if (!node) {
		return;
	}

	if (!CollisionDetector::AreColliding(testedBVolume, node->Bounding)) {
		return;
	}

	if (node->IsLeaf) {
		auto &leaf = node->Leaf;
		for (size_t i = leaf.FirstId; i <= leaf.LastId; i++) {
			// CollisionDetector::AreColliding(testedBVolume, bvh->GetBoundingUsingId(i));
		}
		return;
	}

	CollisionOverBVH(node->Internal.LeftChildren);
	CollisionOverBVH(node->Internal.RightChildren);
}
