#pragma once
#include "Physics/Boundings/BVolumes.h"
#include "Ecs/Entity.h"

#include <vector>
#include <utility>

class BVH {
public:
	enum Axis {
		OX = 0, OY = 1, OZ = 2
	};

	using BoundedEntity = std::pair<Ecs::Entity, AABB>;

	struct BVHNode {
		const bool	IsLeaf;
		AABB		Bounding;
		union {
			struct {
				Axis		 SplitAxis;
				BVHNode	*LeftChildren;
				BVHNode	*RightChildren;
			} Internal;
			struct {
				size_t	FirstId;
				size_t	LastId;
			} Leaf;
		};

		BVHNode(Axis splitAxis, BVHNode *leftChildren, BVHNode *rightChildren);
		BVHNode(AABB &bounds, size_t firstIdentifier, size_t lastIdentifier);
	};

	void BuildFrom(std::vector<BoundedEntity> &&entities);

	BVHNode* GetRoot();
	AABB& GetBoundingUsingId(size_t id);
	Ecs::Entity GetEntityUsingId(size_t id);
	size_t GetDepth() const { return bvhDepth; }

private:
	BVHNode* BuildTree(size_t startId, size_t endId, size_t &depth);

private:
	AABB ComputeBoundingForRange(size_t startId, size_t endId);
	static Axis FindBestAxisFor(const AABB &bounding);
	size_t PartitionEqually(size_t startId, size_t endId, Axis splitAxis);

private:
	BVHNode				*bvhRoot = nullptr;
	std::vector<BVHNode>		 nodesPool;
	std::vector<size_t>		 ids;
	std::vector<BoundedEntity>	 boundedEntities;
	size_t 				 bvhDepth;

};

