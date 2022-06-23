#include "BVH.h"

#include <algorithm>

BVH::BVHNode::BVHNode(BVH::Axis splitAxis, BVH::BVHNode *leftChildren, BVH::BVHNode *rightChildren)
	: IsLeaf(false), Bounds(leftChildren->Bounds) {
	Bounds.Union(rightChildren->Bounds);
	Internal.SplitAxis = splitAxis;
	Internal.LeftChildren = leftChildren;
	Internal.RightChildren = rightChildren;
}

BVH::BVHNode::BVHNode(AABB &bounds, size_t firstPrimitiveIndex, size_t primitivesNum)
	: IsLeaf(true), Bounds(bounds) {
	Leaf.FirstPrimitiveIndex = firstPrimitiveIndex;
	Leaf.PrimitivesNum = primitivesNum;
}

void BVH::BuildFrom(const std::vector<AABB> &boundingVolumes) {
	boundingVolumesRef = &boundingVolumes;
	nodes.clear();
	identifiers.resize(boundingVolumes.size());
	for (size_t i = 0; i < boundingVolumes.size(); i++) {
		identifiers[i] = i;
	}
	// TODO Pre alloc nodes to avoid reallocation, because you kep a ptr reference this will be a problem
	nodes.clear();
	nodes.reserve(3 * boundingVolumes.size());
	root = BuildTree(0, boundingVolumes.size());
}

AABB BVH::ComputeBoundsFor(size_t start, size_t end) {
	AABB result = (*boundingVolumesRef)[start];
	for (size_t i = start + 1; i < end; i++) {
		result.Union((*boundingVolumesRef)[i]);
	}
	return result;
}

BVH::Axis BVH::ChooseAxisFor(const AABB &bounding) {
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
	auto xAxisComparator = [=](const size_t identifier1, const size_t identifier2) {
		return (*boundingVolumesRef)[identifier1].GetPosition().x
		        < (*boundingVolumesRef)[identifier2].GetPosition().x;
	};
	auto yAxisComparator = [=](const size_t identifier1, const size_t identifier2) {
		return (*boundingVolumesRef)[identifier1].GetPosition().y
		        < (*boundingVolumesRef)[identifier2].GetPosition().y;
	};
	auto zAxisComparator = [=](const size_t identifier1, const size_t identifier2) {
		return (*boundingVolumesRef)[identifier1].GetPosition().z
		        < (*boundingVolumesRef)[identifier2].GetPosition().z;
	};

	switch (splitAxis) {
	case OX:
		std::nth_element(&identifiers[start], &identifiers[mid], &identifiers[end], xAxisComparator);
		break;
	case OY:
		std::nth_element(&identifiers[start], &identifiers[mid], &identifiers[end], yAxisComparator);
		break;
	case OZ:
		std::nth_element(&identifiers[start], &identifiers[mid], &identifiers[end], zAxisComparator);
		break;
	}
	return mid;
}

BVH::BVHNode* BVH::BuildTree(size_t start, size_t end) {
	AABB bounding = ComputeBoundsFor(start, end);
	size_t primitivesNum = end - start;

	float width;
	Axis splitAxis = ChooseAxisFor(bounding);
	switch (splitAxis) {
	case OX:
		width = bounding.GetWidths().x;
		break;
	case OY:
		width = bounding.GetWidths().y;
		break;
	case OZ:
		width = bounding.GetWidths().z;
		break;
	}

	if (primitivesNum == 1 || width == 0) {
		nodes.emplace_back(bounding, nodes.size(), primitivesNum);
		return &nodes.back();
	}

	size_t mid = PartitionEqually(start, end, splitAxis);
	nodes.emplace_back(splitAxis, BuildTree(start, mid), BuildTree(mid, end));
	return &nodes.back();
}

