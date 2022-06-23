#pragma once
#include "Collision/BVolumes.h"
#include "Scene/Components/Transform.h"

#include <variant>

struct CollisionInfo {
	bool CollisionFlag = false;
};

class ColliderComponent {
public:
	explicit ColliderComponent(const Transform &transform, BVolume::Type type)
		: bVolume(nullptr) {
		InitBVolume(transform, type);
	}


	void InitBVolume(const Transform &transform, BVolume::Type type) {
		if (type == BVolume::AABB) {
			bVolumeRef = &bVolume.template emplace<AABB>(transform.Position, transform.Scale);
		} else if (type == BVolume::Sphere) {
			bVolumeRef = &bVolume.template emplace<Sphere>(transform.Position, transform.Scale.x);
		} else if (type == BVolume::OBB) {
			auto &obb = bVolume.template emplace<OBB>(transform.Position, transform.Scale);
			obb.SetRotation(transform.Rotation);
			bVolumeRef = &obb;
		}
	}

	void SetTransform(const Transform &transform) {
		if (std::holds_alternative<AABB>(bVolume)) {
			auto &aabb = std::get<AABB>(bVolume);
			aabb.SetPosition(transform.Position);
			aabb.SetWidths(transform.Scale);
		} else if (std::holds_alternative<Sphere>(bVolume)) {
			auto &sphere = std::get<Sphere>(bVolume);
			sphere.SetPosition(transform.Position);
			sphere.SetRadius(transform.Scale.x);
		} else if (std::holds_alternative<OBB>(bVolume)) {
			auto &obb = std::get<OBB>(bVolume);
			obb.SetPosition(transform.Position);
			obb.SetRotation(transform.Rotation);
			obb.SetWidths(transform.Scale);
		}
	}

	const BVolume& GetBoundingVolume() {
		return *bVolumeRef;
	}

private:
	BVolume *bVolumeRef;
	std::variant<AABB, Sphere, OBB, void*> bVolume;
};