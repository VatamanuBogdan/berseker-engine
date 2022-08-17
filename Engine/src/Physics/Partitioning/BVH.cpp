#include "BVH.h"

#include <algorithm>

BVH::BVHNode::BVHNode(BVH::Axis splitAxis, BVH::BVHNode *leftChildren, BVH::BVHNode *rightChildren)
	: IsLeaf(false), Bounding(leftChildren->Bounding) {
	Bounding.Union(rightChildren->Bounding);
	Internal.SplitAxis = splitAxis;
	Internal.LeftChildren = leftChildren;
	Internal.RightChildren = rightChildren;
}

BVH::BVHNode::BVHNode(AABB &bounds, size_t firstIdentifier, size_t lastIdentifier)
	: IsLeaf(true), Bounding(bounds) {
	Leaf.FirstId = firstIdentifier;
	Leaf.LastId = lastIdentifier;
}

void BVH::BuildFrom(std::vector<BoundedEntity> &&entities) {
	nodesPool.clear();
	nodesPool.reserve(3 * entities.size());
	boundedEntities = std::move(entities);

	ids.resize(boundedEntities.size());
	for (size_t i = 0; i < boundedEntities.size(); i++) {
		ids[i] = i;
	}

	bvhRoot = BuildTree(0, this->boundedEntities.size());
}

BVH::BVHNode *BVH::GetRoot() {
	return bvhRoot;
}

BVH::BVHNode* BVH::BuildTree(size_t startId, size_t endId) {
	AABB groupBounding = ComputeBoundingForRange(startId, endId);
	size_t entitiesNum = endId - startId;

	float width;
	Axis splitAxis = FindBestAxisFor(groupBounding);
	switch (splitAxis) {
		case OX:
			width = groupBounding.GetWidths().x;
			break;
		case OY:
			width = groupBounding.GetWidths().y;
			break;
		case OZ:
			width = groupBounding.GetWidths().z;
			break;
	}

	if (entitiesNum == 1 || width == 0) {
		nodesPool.emplace_back(groupBounding, nodesPool.size(), nodesPool.size() + entitiesNum);
		return &nodesPool.back();
	}

	size_t partitionId = PartitionEqually(startId, endId, splitAxis);
	nodesPool.emplace_back(splitAxis, BuildTree(startId, partitionId), BuildTree(partitionId, endId));
	return &nodesPool.back();
}

AABB& BVH::GetBoundingUsingId(size_t id) {
	return boundedEntities[id].second;
}

AABB BVH::ComputeBoundingForRange(size_t startId, size_t endId) {
	AABB result = GetBoundingUsingId(startId);
	for (size_t i = startId + 1; i < endId; i++) {
		result.Union(GetBoundingUsingId(i));
	}
	return result;
}

BVH::Axis BVH::FindBestAxisFor(const AABB &bounding) {
	auto widths = bounding.GetWidths();
	if (widths.x > widths.y && widths.x > widths.z) {
		return Axis::OX;
	}
	if (widths.y > widths.z) {
		return Axis::OY;
	}
	return BVH::Axis::OZ;
}

size_t BVH::PartitionEqually(size_t start, size_t end, Axis splitAxis) {
	size_t mid = (start + end) / 2;
	auto xAxisComparator = [this](const size_t id1, const size_t id2) {
		return GetBoundingUsingId(id1).GetPosition().x
			 < GetBoundingUsingId(id2).GetPosition().x;
	};
	auto yAxisComparator = [=](const size_t id1, const size_t id2) {
		return GetBoundingUsingId(id1).GetPosition().y
			 < GetBoundingUsingId(id2).GetPosition().y;
	};
	auto zAxisComparator = [=](const size_t id1, const size_t id2) {
		return GetBoundingUsingId(id1).GetPosition().z
			 < GetBoundingUsingId(id2).GetPosition().z;
	};

	switch (splitAxis) {
	case OX:
		std::nth_element(&ids[start], &ids[mid], &ids[end], xAxisComparator);
		break;
	case OY:
		std::nth_element(&ids[start], &ids[mid], &ids[end], yAxisComparator);
		break;
	case OZ:
		std::nth_element(&ids[start], &ids[mid], &ids[end], zAxisComparator);
		break;
	}
	return mid;
}
