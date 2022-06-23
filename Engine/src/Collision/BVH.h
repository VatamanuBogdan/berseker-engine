#pragma once
#include "BVolumes.h"

#include <vector>

class BVH {
public:
	enum Axis {
		OX = 0, OY = 1, OZ = 2
	};

	using PrimitiveIdentifier = size_t;

	struct BVHNode {
		const bool		IsLeaf;
		BVolumes::AABB	Bounds;
		union {
			struct {
				Axis		 SplitAxis;
				BVHNode	*LeftChildren;
				BVHNode	*RightChildren;
			} Internal;
			struct {
				size_t	FirstPrimitiveIndex;
				size_t	PrimitivesNum;
			} Leaf;
		};

		BVHNode(Axis splitAxis, BVHNode *leftChildren, BVHNode *rightChildren);
		BVHNode(BVolumes::AABB &bounds, size_t firstPrimitiveIndex, size_t primitivesNum);
	};

	void BuildFrom(const std::vector<BVolumes::AABB> &boundingVolumes);

private:
	BVolumes::AABB ComputeBoundsFor(size_t start, size_t end);
	Axis ChooseAxisFor(const BVolumes::AABB &bounding);
	size_t PartitionEqually(size_t start, size_t end, Axis splitAxis);

	BVHNode* BuildTree(size_t start, size_t end);

private:
	BVHNode					*root;
	std::vector<BVHNode>			 nodes;
	std::vector<PrimitiveIdentifier> 	 identifiers;
	const std::vector<BVolumes::AABB>	*boundingVolumesRef;
};

