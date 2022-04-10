#include "BoundingVolumes.h"


glm::vec3 BoundingVolumes::AABB::ComputeClosesPointFor(const glm::vec3 &point) const {
	glm::vec3 min = position - halfWidths;
	glm::vec3 max = position + halfWidths;

	return {
		glm::min(glm::max(min.x, point.x), max.x),
		glm::min(glm::max(min.y, point.y), max.y),
		glm::min(glm::max(min.z, point.z), max.z)
	};
}

float BoundingVolumes::AABB::ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const {
	glm::vec3 min = position - halfWidths;
	glm::vec3 max = position + halfWidths;

	float xDistance = point.x < min.x ? (min.x - point.x) * (min.x - point.x) : 0
		  		+ point.x > max.x ? (point.x - max.x) * (point.x - max.x) : 0;

	float yDistance = point.y < min.y ? (min.y - point.y) * (min.y - point.y) : 0
		  		+ point.y > max.y ? (point.y - max.y) * (point.y - max.y) : 0;

	float zDistance = point.z < min.z ? (min.z - point.z) * (min.z - point.z) : 0
		  		+ point.z > max.z ? (point.z - max.z) * (point.z - max.z) : 0;

	return xDistance + yDistance + zDistance;
}

bool BoundingVolumes::CollisionDetector::AreColliding(const AABB &aabb1, const AABB &aabb2) {
	if (glm::abs(aabb1.position.x - aabb2.position.x) > (aabb1.halfWidths.x + aabb2.halfWidths.x)
		|| glm::abs(aabb1.position.y - aabb2.position.y) > (aabb1.halfWidths.y + aabb2.halfWidths.y)
		|| glm::abs(aabb1.position.z - aabb2.position.z) > (aabb1.halfWidths.z + aabb2.halfWidths.z)) {
		return false;
	}
	return true;
}

bool BoundingVolumes::CollisionDetector::AreColliding(const AABB &aabb, const Sphere &sphere) {
	return aabb.ComputeSquaredDistanceToClosesPointFor(sphere.position) <= sphere.radius * sphere.radius;
}

bool BoundingVolumes::CollisionDetector::AreColliding(const Sphere &sphere, const AABB &aabb) {
	return AreColliding(aabb, sphere);
}
