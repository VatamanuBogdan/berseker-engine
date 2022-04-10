#pragma once
#include <glm/glm.hpp>


namespace BoundingVolumes {
	class CollisionDetector;


	class BoundingVolume {
	};


	class AABB : public BoundingVolume {
	public:
		AABB(const glm::vec3 &position, float xWidth, float yWidth, float zWidth)
			  : position(position), halfWidths(xWidth / 2, yWidth / 2, zWidth / 2) {
		}

		void MoveTo(const glm::vec3 &position) { this->position = position; }
		[[nodiscard]] const glm::vec3& GetPosition() const { return this->position; }

		[[nodiscard]] glm::vec3 ComputeClosesPointFor(const glm::vec3 &point) const;
		[[nodiscard]] float ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const;

	private:
		glm::vec3 position;
		glm::vec3 halfWidths;

		friend class CollisionDetector;
	};


	class Sphere : public BoundingVolume {
	public:
		Sphere(const glm::vec3 &position, float radius)
			  : position(position), radius(radius) {
		}

		void MoveTo(const glm::vec3 &position) { this->position = position; }
		[[nodiscard]] const glm::vec3& GetPosition() const { return this->position; }

	private:
		glm::vec3 position;
		float radius;

		friend class CollisionDetector;
	};


	class CollisionDetector {
	public:
		static bool AreColliding(const AABB &aabb1, const AABB &aabb2);

		static bool AreColliding(const AABB &aabb, const Sphere &sphere);
		static bool AreColliding(const Sphere &sphere, const AABB &aabb);

	private:
		CollisionDetector() = default;
	};

}