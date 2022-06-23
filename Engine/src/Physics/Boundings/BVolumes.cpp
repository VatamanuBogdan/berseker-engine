#include "BVolumes.h"


void AABB::Apply(const Transform &transform) {
	glm::mat4 transformMatrix = transform.ComputeTransformMatrix();
	glm::vec3 minPoint = position - halfWidths;
	glm::vec3 maxPoint = position + halfWidths;

	glm::vec4 points[] = {
		  {maxPoint, 1},
		  {maxPoint.x, maxPoint.y, minPoint.z, 1},
		  {minPoint.x, maxPoint.y, maxPoint.z, 1},
		  {minPoint.x, maxPoint.y, minPoint.z, 1},
		  {minPoint, 1},
		  {maxPoint.x, minPoint.y, minPoint.z, 1},
		  {minPoint.x, minPoint.y, maxPoint.z, 1},
		  {maxPoint.x, minPoint.y, maxPoint.z, 1},
	};

	minPoint = maxPoint = transformMatrix * points[0];
	for (auto &point : points) {
		glm::vec3 transformedPoint = transformMatrix * point;
		minPoint = glm::min(minPoint, transformedPoint);
		maxPoint = glm::max(maxPoint, transformedPoint);
	}

	position = (minPoint + maxPoint) / 2.0f;
	halfWidths = (maxPoint - minPoint) / 2.0f;
}

glm::vec3 AABB::ComputeClosesPointFor(const glm::vec3 &point) const {
	glm::vec3 min = position - halfWidths;
	glm::vec3 max = position + halfWidths;

	return {
		  glm::min(glm::max(min.x, point.x), max.x),
		  glm::min(glm::max(min.y, point.y), max.y),
		  glm::min(glm::max(min.z, point.z), max.z)
	};
}

float AABB::ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const {
	glm::vec3 min = position - halfWidths;
	glm::vec3 max = position + halfWidths;

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

void AABB::Union(const AABB &aabb) {
	glm::vec3 minPoint = glm::min(position - halfWidths, aabb.position - aabb.halfWidths);
	glm::vec3 maxPoint = glm::min(position + halfWidths, aabb.position + aabb.halfWidths);

	position = (minPoint + maxPoint) / 2.0f;
	halfWidths = (maxPoint - minPoint) / 2.0f;
}

AABB& AABB::operator=(const AABB &aabb) {
	position = aabb.position;
	halfWidths = aabb.halfWidths;
	return *this;
}

glm::vec3 OBB::ComputeClosesPointFor(const glm::vec3 &point) const {
	const glm::vec3 distVec = point - position;

	glm::vec3 result = position;

	float dist = glm::dot(distVec, Direction.XAxis);
	if (dist > halfWidths.x) {
		dist = halfWidths.x;
	}
	if (dist < -halfWidths.x) {
		dist = -halfWidths.x;
	}
	result += dist * Direction.XAxis;

	dist = glm::dot(distVec, Direction.YAxis);
	if (dist > halfWidths.y) {
		dist = halfWidths.y;
	}
	if (dist < -halfWidths.y) {
		dist = -halfWidths.y;
	}
	result += dist * Direction.YAxis;

	dist = glm::dot(distVec, Direction.ZAxis);
	if (dist > halfWidths.z) {
		dist = halfWidths.z;
	}
	if (dist < -halfWidths.z) {
		dist = -halfWidths.z;
	}
	result += dist * Direction.ZAxis;

	return result;
}

bool CollisionDetector::AreColliding(const AABB &aabb1, const AABB &aabb2) {
	if (glm::abs(aabb1.position.x - aabb2.position.x) > (aabb1.halfWidths.x + aabb2.halfWidths.x)
	    || glm::abs(aabb1.position.y - aabb2.position.y) > (aabb1.halfWidths.y + aabb2.halfWidths.y)
	    || glm::abs(aabb1.position.z - aabb2.position.z) > (aabb1.halfWidths.z + aabb2.halfWidths.z)) {
		return false;
	}
	return true;
}

bool CollisionDetector::AreColliding(const AABB &aabb, const Sphere &sphere) {
	return aabb.ComputeSquaredDistanceToClosesPointFor(sphere.position) <= sphere.radius * sphere.radius;
}

bool CollisionDetector::AreColliding(const AABB &aabb,
						 const OBB &obb) {
	const OBB tempOBB(aabb.position, glm::vec3(2) * aabb.halfWidths);
	return AreColliding(tempOBB, obb);
}

bool CollisionDetector::AreColliding(const Sphere &sphere1,
						 const Sphere &sphere2) {
	return glm::length(sphere1.position - sphere2.position) <= sphere1.radius + sphere2.radius;
}

bool CollisionDetector::AreColliding(const Sphere &sphere,
						 const AABB &aabb) {
	return AreColliding(aabb, sphere);
}

bool CollisionDetector::AreColliding(const Sphere &sphere,
						 const OBB &obb) {
	glm::vec3 distVec = obb.ComputeClosesPointFor(sphere.position) - sphere.position;
	return glm::dot(distVec, distVec) <= sphere.radius * sphere.radius;
}

bool CollisionDetector::AreColliding(const OBB &obb,
						 const AABB &aabb) {
	return AreColliding(aabb, obb);
}

bool CollisionDetector::AreColliding(const OBB &obb,
						 const Sphere &sphere) {
	return AreColliding(sphere, obb);
}

bool CollisionDetector::AreColliding(const OBB &obb1,
						 const OBB &obb2) {
	auto &dir1 = obb1.Direction;
	auto &dir2 = obb2.Direction;

	auto &hw1 = obb1.halfWidths;
	auto &hw2 = obb2.halfWidths;

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

	glm::vec3 trans = obb2.position - obb1.position;
	trans = glm::vec3{glm::dot(trans, dir1.XAxis), glm::dot(trans, dir1.YAxis), glm::dot(trans, dir1.ZAxis)};

	// OBB1 Axis
	for (auto i = 0; i < 3; i++) {
		proj1 = obb1.halfWidths[i];
		proj2 = glm::dot(obb2.halfWidths, glm::vec3(absRotation[i][0], absRotation[i][1], absRotation[i][2]));
		if (glm::abs(trans[i]) > proj1 + proj2) {
			return false;
		}
	}

	// OBB2 Axis
	for (auto i = 0; i < 3; i++) {
		proj1 = glm::dot(obb1.halfWidths, glm::vec3(absRotation[0][i], absRotation[1][i], absRotation[2][i]));
		proj2 = obb2.halfWidths[i];
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

bool CollisionDetector::AreColliding(const BVolume &bvolume1,
						 const BVolume &bvolume2) {
	return Dispatcher::CallAreCollidingDispatched(bvolume1, bvolume2);
}
