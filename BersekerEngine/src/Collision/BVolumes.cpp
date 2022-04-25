#include "BVolumes.h"

#include <stdexcept>
#include <glm/gtc/constants.hpp>


namespace BVolumes {

	glm::vec3 AABB::ComputeClosesPointFor(const glm::vec3 &point) const {
		glm::vec3 min = Position - HalfWidths;
		glm::vec3 max = Position + HalfWidths;

		return {
			  glm::min(glm::max(min.x, point.x), max.x),
			  glm::min(glm::max(min.y, point.y), max.y),
			  glm::min(glm::max(min.z, point.z), max.z)
		};
	}

	float AABB::ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const {
		glm::vec3 min = Position - HalfWidths;
		glm::vec3 max = Position + HalfWidths;

		float xDistance = point.x < min.x ? (min.x - point.x) * (min.x - point.x) : 0 + point.x > max.x ?
														    (point.x - max.x) *
														    (point.x - max.x) : 0;

		float yDistance = point.y < min.y ? (min.y - point.y) * (min.y - point.y) : 0 + point.y > max.y ?
														    (point.y - max.y) *
														    (point.y - max.y) : 0;

		float zDistance = point.z < min.z ? (min.z - point.z) * (min.z - point.z) : 0 + point.z > max.z ?
														    (point.z - max.z) *
														    (point.z - max.z) : 0;

		return xDistance + yDistance + zDistance;
	}

	bool CollisionDetector::AreColliding(const AABB &aabb1, const AABB &aabb2) {
		if (glm::abs(aabb1.Position.x - aabb2.Position.x) > (aabb1.HalfWidths.x + aabb2.HalfWidths.x)
		    || glm::abs(aabb1.Position.y - aabb2.Position.y) > (aabb1.HalfWidths.y + aabb2.HalfWidths.y)
		    || glm::abs(aabb1.Position.z - aabb2.Position.z) > (aabb1.HalfWidths.z + aabb2.HalfWidths.z)) {
			return false;
		}
		return true;
	}

	bool CollisionDetector::AreColliding(const AABB &aabb, const Sphere &sphere) {
		return aabb.ComputeSquaredDistanceToClosesPointFor(sphere.Position) <= sphere.Radius * sphere.Radius;
	}

	bool CollisionDetector::AreColliding(const BVolumes::AABB &aabb,
							 const BVolumes::OBB &obb) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	bool CollisionDetector::AreColliding(const BVolumes::Sphere &sphere1,
							 const BVolumes::Sphere &sphere2) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	bool CollisionDetector::AreColliding(const Sphere &sphere,
							 const AABB &aabb) {
		return AreColliding(aabb, sphere);
	}

	bool CollisionDetector::AreColliding(const BVolumes::Sphere &sphere,
							 const BVolumes::OBB &obb) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	bool CollisionDetector::AreColliding(const BVolumes::OBB &obb,
							 const BVolumes::AABB &aabb) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	bool CollisionDetector::AreColliding(const BVolumes::OBB &obb,
							 const BVolumes::Sphere &sphere) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	bool CollisionDetector::AreColliding(const BVolumes::OBB &obb1,
							 const BVolumes::OBB &obb2) {
		auto &dir1 = obb1.Directions;
		auto &dir2 = obb2.Directions;

		auto &hw1 = obb1.HalfWidths;
		auto &hw2 = obb2.HalfWidths;

		float proj1;
		float proj2;

		glm::mat3 rotation = {
			  glm::dot(dir1.XAxis, dir2.XAxis), glm::dot(dir1.XAxis, dir2.YAxis),
			  glm::dot(dir1.XAxis, dir2.ZAxis),
			  glm::dot(dir1.YAxis, dir2.XAxis), glm::dot(dir1.YAxis, dir2.YAxis),
			  glm::dot(dir1.YAxis, dir2.ZAxis),
			  glm::dot(dir1.ZAxis, dir2.XAxis), glm::dot(dir1.ZAxis, dir2.YAxis),
			  glm::dot(dir1.ZAxis, dir2.ZAxis),
		};
		glm::mat3 absRotation;
		for (auto i = 0; i < 3; i++) {
			for (auto j = 0; j < 3; j++) {
				absRotation[i][j] = glm::abs(rotation[i][j]) + glm::epsilon<float>();
			}
		}

		glm::vec3 trans = obb2.Position - obb1.Position;
		trans = glm::vec3{glm::dot(trans, dir1.XAxis), glm::dot(trans, dir1.YAxis), glm::dot(trans, dir1.ZAxis)};

		// OBB1 Axis
		for (auto i = 0; i < 3; i++) {
			proj1 = obb1.HalfWidths[i];
			proj2 = glm::dot(obb2.HalfWidths, glm::vec3(absRotation[i][0], absRotation[i][1], absRotation[i][2]));
			if (glm::abs(trans[i]) > proj1 + proj2) {
				return false;
			}
		}

		// OBB2 Axis
		for (auto i = 0; i < 3; i++) {
			proj1 = glm::dot(obb1.HalfWidths, glm::vec3(absRotation[0][i], absRotation[1][i], absRotation[2][i]));
			proj2 = obb2.HalfWidths[i];
			if (glm::abs(glm::dot(trans, glm::vec3(rotation[0][i], rotation[1][i], rotation[2][i]))) >
			    proj1 + proj2) {
				return false;
			}
		}

		proj1 = hw1[1] * absRotation[2][0] + hw1[2] * absRotation[1][0];
		proj2 = hw2[1] * absRotation[0][2] + hw1[2] * absRotation[0][1];
		if (glm::abs(trans[2] * rotation[1][0] - trans[1] * rotation[2][0]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[1] * absRotation[2][1] + hw1[2] * absRotation[1][1];
		proj2 = hw2[0] * absRotation[0][2] + hw2[2] * absRotation[0][0];
		if (glm::abs(trans[2] * rotation[1][1] - trans[1] * rotation[2][1]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[1] * absRotation[2][2] + hw1[2] * absRotation[1][2];
		proj2 = hw2[0] * absRotation[0][1] + hw2[1] * absRotation[0][0];
		if (glm::abs(trans[2] * rotation[1][2] - trans[1] * rotation[2][2]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[2][0] + hw1[2] * absRotation[0][0];
		proj2 = hw2[1] * absRotation[1][2] + hw2[2] * absRotation[1][1];
		if (glm::abs(trans[0] * rotation[2][0] - trans[2] * rotation[0][0]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[2][1] + hw1[2] * absRotation[0][1];
		proj2 = hw2[0] * absRotation[1][2] + hw2[2] * absRotation[1][0];
		if (glm::abs(trans[0] * rotation[2][1] - trans[2] * rotation[0][1]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[2][2] + hw1[2] * absRotation[0][2];
		proj2 = hw2[0] * absRotation[1][1] + hw2[1] * absRotation[1][0];
		if (glm::abs(trans[0] * rotation[2][2] - trans[2] * rotation[0][2]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[1][0] + hw1[1] * absRotation[0][0];
		proj2 = hw2[1] * absRotation[2][2] + hw2[2] * absRotation[2][1];
		if (glm::abs(trans[1] * rotation[0][0] - trans[0] * rotation[1][0]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[1][1] + hw1[1] * absRotation[0][1];
		proj2 = hw2[0] * absRotation[2][2] + hw2[2] * absRotation[2][0];
		if (glm::abs(trans[1] * rotation[0][1] - trans[0] * rotation[1][1]) > proj1 + proj2) {
			return false;
		}

		proj1 = hw1[0] * absRotation[1][2] + hw1[1] * absRotation[0][2];
		proj2 = hw2[0] * absRotation[2][1] + hw2[1] * absRotation[2][0];
		if (glm::abs(trans[1] * rotation[0][2] - trans[0] * rotation[1][2]) > proj1 + proj2) {
			return false;
		}

		return true;
	}

	bool CollisionDetector::AreCollidingDispatch(const BoundingVolume &bvolume1,
								   const BoundingVolume &bvolume2) {
		return Dispatcher::CallAreCollidingDispatched(bvolume1, bvolume2);
	}

}