#pragma once
#include "Collision/BVolumes.h"
#include "ECS/Components/Transform.h"

#include <variant>

struct CollisionInfo {
	bool CollisionFlag = false;
};

class ColliderComponent {
public:
	explicit ColliderComponent(const Transform &transform, BVolumes::BVolume::Type type)
		: bVolume(nullptr) {
		InitBVolume(transform, type);
	}


	void InitBVolume(const Transform &transform, BVolumes::BVolume::Type type) {
		if (type == BVolumes::BVolume::AABB) {
			bVolumeRef = &bVolume.template emplace<BVolumes::AABB>(transform.Position, transform.Scale);
		} else if (type == BVolumes::BVolume::Sphere) {
			bVolumeRef = &bVolume.template emplace<BVolumes::Sphere>(transform.Position, transform.Scale.x);
		} else if (type == BVolumes::BVolume::OBB) {
			auto &obb = bVolume.template emplace<BVolumes::OBB>(transform.Position, transform.Scale);
			obb.SetRotation(transform.Rotation);
			bVolumeRef = &obb;
		}
	}

	void SetTransform(const Transform &transform) {
		if (std::holds_alternative<BVolumes::AABB>(bVolume)) {
			auto &aabb = std::get<BVolumes::AABB>(bVolume);
			aabb.SetPosition(transform.Position);
			aabb.SetWidths(transform.Scale);
		} else if (std::holds_alternative<BVolumes::Sphere>(bVolume)) {
			auto &sphere = std::get<BVolumes::Sphere>(bVolume);
			sphere.SetPosition(transform.Position);
			sphere.SetRadius(transform.Scale.x);
		} else if (std::holds_alternative<BVolumes::OBB>(bVolume)) {
			auto &obb = std::get<BVolumes::OBB>(bVolume);
			obb.SetPosition(transform.Position);
			obb.SetRotation(transform.Rotation);
			obb.SetWidths(transform.Scale);
		}
	}

	const BVolumes::BVolume& GetBoundingVolume() {
		return *bVolumeRef;
	}

private:
	BVolumes::BVolume *bVolumeRef;
	std::variant<BVolumes::AABB, BVolumes::Sphere, BVolumes::OBB, void*> bVolume;
};