#pragma once
#include "Physics/Boundings/BVolumes.h"
#include "Scene/Components/Transform.h"

#include <memory>

struct CollisionInfo {
	bool CollisionFlag = false;
};

class ColliderComponent {
public:
	explicit ColliderComponent(const Transform &transform, BVolume::Type type) {
		InitBVolume(transform, type);
	}

	explicit ColliderComponent(const AABB &aabb)
		: bVolume(std::make_shared<AABB>(aabb)) {
	}

	void InitBVolume(const Transform &transform, BVolume::Type type) {
		if (type == BVolume::AABB) {
			bVolume = std::make_shared<AABB>(transform.Position, transform.Scale);
		} else if (type == BVolume::Sphere) {
			bVolume = std::make_shared<Sphere>(transform.Position, transform.Scale.x);
		} else if (type == BVolume::OBB) {
			bVolume = std::make_shared<OBB>(transform.Position, transform.Scale);
			((OBB*) bVolume.get())->SetRotation(transform.Rotation);
		}
	}

	void SetTransform(const Transform &transform) {
		BVolume::Type tag = bVolume->TypeTag;
		if (tag == BVolume::AABB) {
			auto &aabb = *((AABB*) (bVolume.get()));
			aabb.SetPosition(transform.Position);
			aabb.SetWidths(transform.Scale);
		} else if (tag == BVolume::Sphere) {
			auto &sphere = *((Sphere*) (bVolume.get()));
			sphere.SetPosition(transform.Position);
			sphere.SetRadius(transform.Scale.x);
		} else if (tag == BVolume::OBB) {
			auto &obb = *((OBB*) (bVolume.get()));
			obb.SetPosition(transform.Position);
			obb.SetRotation(transform.Rotation);
			obb.SetWidths(transform.Scale);
		}
	}

	const BVolume& GetBoundingVolume() {
		return *bVolume;
	}

private:
	std::shared_ptr<BVolume> bVolume;
};