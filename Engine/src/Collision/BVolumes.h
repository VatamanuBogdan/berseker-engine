#pragma once
#include "ECS/Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <array>


class CollisionDetector;

class BVolume {
public:
	enum Type : unsigned short {
		AABB = 0,
		Sphere,
		OBB,
		LENGTH
	};
	const Type TypeTag;

protected:
	explicit BVolume(Type type)
		: TypeTag(type) {
	}
};


class AABB : public BVolume {
public:
	AABB(const glm::vec3 &position, glm::vec3 widths)
		: BVolume(Type::AABB), position(position), halfWidths(glm::abs(widths) / 2.0f) {
	}

	void Apply(const Transform &transform);

	[[nodiscard]] glm::vec3 ComputeClosesPointFor(const glm::vec3 &point) const;
	[[nodiscard]] float ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const;

	[[nodiscard]] const glm::vec3& GetPosition() const { return position; }
	[[nodiscard]] glm::vec3 GetWidths() const { return 2.0f * halfWidths; }

	void SetPosition(const glm::vec3 &position) { this->position = position; }
	void SetWidths(const glm::vec3 &widths) { this->halfWidths = glm::abs(widths) / 2.0f; }

	void Union(const AABB &aabb);

	AABB& operator=(const AABB &aabb);

private:
	glm::vec3 position;
	glm::vec3 halfWidths;
	friend class CollisionDetector;
};


class Sphere : public BVolume {
public:
	Sphere(const glm::vec3 &position, float radius)
		  : BVolume(Type::Sphere), position(position), radius(glm::abs(radius)) {
	}

	[[nodiscard]] const glm::vec3& GetPosition() const { return position; }
	[[nodiscard]] float GetRadius() const { return radius; }

	void SetPosition(const glm::vec3 &position) { this->position = position; }
	void SetRadius(float radius) { this->radius = glm::abs(radius); }

private:
	glm::vec3 	position;
	float 	radius;
	friend class CollisionDetector;
};


class OBB : public BVolume {
private:
	struct Direction {
		glm::vec3 XAxis = glm::vec3(1, 0, 0);
		glm::vec3 YAxis = glm::vec3(0, 1, 0);
		glm::vec3 ZAxis = glm::vec3(0, 0, 1);
	};

public:
	OBB(const glm::vec3 &position, const glm::vec3 &widths, const glm::vec3 rotation = glm::vec3(0))
		: BVolume(Type::OBB), position(position), halfWidths(glm::abs(widths) / 2.0f), rotation(rotation) {
		SetRotation(rotation);
	}

	[[nodiscard]] glm::vec3 ComputeClosesPointFor(const glm::vec3 &point) const;

	[[nodiscard]] const glm::vec3& GetPosition() const { return position; }
	[[nodiscard]] glm::vec3 GetWidths() const { return 2.0f * halfWidths; }
	[[nodiscard]] const glm::vec3& GetRotation() const { return rotation; }

	void SetPosition(const glm::vec3 &position) { this->position = position; }
	void SetWidths(const glm::vec3 &widths) { this->halfWidths = glm::abs(widths) / 2.0f; }

	void SetRotation(const glm::vec3 &rotation) {
		this->rotation = rotation;

		glm::mat4 transform(1);
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1.0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1.0, 0));
		transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0, 0));

		Direction.YAxis = glm::normalize(glm::vec3(transform * glm::vec4(0, 1, 0, 1)));
		Direction.ZAxis  = glm::normalize(glm::vec3(transform * glm::vec4(0, 0, 1, 1)));
		Direction.XAxis = glm::normalize(glm::cross(Direction.YAxis, Direction.ZAxis));
	}

private:
	glm::vec3 position;
	glm::vec3 halfWidths;
	glm::vec3 rotation;
	Direction Direction;
	friend class CollisionDetector;
};


class CollisionDetector {
public:
	static bool AreColliding(const AABB &aabb1, const AABB &aabb2);
	static bool AreColliding(const AABB &aabb, const Sphere &sphere);
	static bool AreColliding(const AABB &aabb, const OBB &obb);

	static bool AreColliding(const Sphere &sphere1, const Sphere &sphere2);
	static bool AreColliding(const Sphere &sphere, const AABB &aabb);
	static bool AreColliding(const Sphere &sphere, const OBB &obb);

	static bool AreColliding(const OBB &obb1, const OBB &obb2);
	static bool AreColliding(const OBB &obb, const AABB &aabb);
	static bool AreColliding(const OBB &obb, const Sphere &sphere);

	static bool AreColliding(const BVolume &bvolume1, const BVolume &bvolume2);

private:
	class Dispatcher {
	public:
		Dispatcher() = delete;

		using TableFunctionType = bool (*)(const BVolume&, const BVolume&);

		template <typename BVolumeType1, typename BVolumeType2>
		static inline bool Dispatch(const BVolume &bVolume1, const BVolume &bVolume2) {
			return CollisionDetector::AreColliding(
				  reinterpret_cast<const BVolumeType1&>(bVolume1),
				  reinterpret_cast<const BVolumeType2&>(bVolume2)
			);
		}

		static inline bool CallAreCollidingDispatched(const BVolume &bVolume1,
									    const BVolume &bVolume2) {
			return DispatchingTable[TableKeyFor(bVolume1.TypeTag, bVolume2.TypeTag)](bVolume1, bVolume2);
		}

	private:
		static constexpr auto TableKeyFor = [](const BVolume::Type &tag1, const BVolume::Type &tag2) {
			auto length = static_cast<std::underlying_type<BVolume::Type>::type>(BVolume::Type::LENGTH);
			auto value1 = static_cast<std::underlying_type<BVolume::Type>::type>(tag1);
			auto value2 = static_cast<std::underlying_type<BVolume::Type>::type>(tag2);
			return length * value1 + value2;
		};

		template<BVolume::Type Type1, BVolume::Type Type2>
		static constexpr auto TableKeyForTypes = []() constexpr {
			return TableKeyFor(Type1, Type2);
		};

		static constexpr size_t TableMaxKey = []() constexpr {
			auto length = static_cast<std::underlying_type<BVolume::Type>::type>(BVolume::Type::LENGTH);
			return length * length;
		}();

	private:
		static constexpr auto DispatchingTable = []() constexpr {
			std::array<TableFunctionType, TableMaxKey> table{};
			table[TableKeyForTypes<BVolume::Type::AABB, BVolume::Type::AABB>()] = Dispatch<AABB, AABB>;
			table[TableKeyForTypes<BVolume::Type::AABB, BVolume::Type::Sphere>()] = Dispatch<AABB, Sphere>;
			table[TableKeyForTypes<BVolume::Type::AABB, BVolume::Type::OBB>()] = Dispatch<AABB, OBB>;
			table[TableKeyForTypes<BVolume::Type::Sphere, BVolume::Type::AABB>()] = Dispatch<Sphere, AABB>;
			table[TableKeyForTypes<BVolume::Type::Sphere, BVolume::Type::Sphere>()] = Dispatch<Sphere, Sphere>;
			table[TableKeyForTypes<BVolume::Type::Sphere, BVolume::Type::OBB>()] = Dispatch<Sphere, OBB>;
			table[TableKeyForTypes<BVolume::Type::OBB, BVolume::Type::AABB>()] = Dispatch<OBB, AABB>;
			table[TableKeyForTypes<BVolume::Type::OBB, BVolume::Type::Sphere>()] = Dispatch<OBB, Sphere>;
			table[TableKeyForTypes<BVolume::Type::OBB, BVolume::Type::OBB>()] = Dispatch<OBB, OBB>;

			return table;
		}();
	};

private:
	CollisionDetector() = default;
};
