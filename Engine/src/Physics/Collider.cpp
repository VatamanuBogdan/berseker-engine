#include "Collider.h"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"

#include <stdexcept>
#include <iostream>

void ConvexGeometry::Reserve(size_t size) {
	convexGeometry.reserve(size);
}

void ConvexGeometry::PushBack(const glm::vec3 &vertex) {
	convexGeometry.push_back(vertex);
}

const glm::vec3& ConvexGeometry::operator[](size_t i) const {
	return convexGeometry[i];
}

size_t ConvexGeometry::GetSize() const {
	return convexGeometry.size();
}

glm::vec3 ConvexGeometry::SupportPointFor(const glm::vec3 &direction, const glm::mat4 &model) const {
	glm::vec3 transformedVertex = model * glm::vec4(convexGeometry[0], 1.0f);
	glm::vec3 furthestVertex = transformedVertex;
	float furthestDistance = glm::dot(furthestVertex, direction);

	for (auto& vertex : convexGeometry) {
		transformedVertex = model * glm::vec4(vertex, 1.0f);

		auto distance = glm::dot(transformedVertex, direction);
		if (distance > furthestDistance) {
			furthestDistance = distance;
			furthestVertex = transformedVertex;
		}
	}

	return furthestVertex;
}


Collider::Collider(std::vector<ConvexGeometry> &&convexGeometries, const Transform &transform)
	  : convexGeometries(std::move(convexGeometries)), transform(transform) {
}

Collider::Collider(Collider &&collider)
	: convexGeometries(std::move(collider.convexGeometries)), transform(collider.transform) {
}

const Transform &Collider::GetTransform() const { return transform; }

void Collider::SetTransform(const Transform &transform) { this->transform = transform; }

const std::vector<ConvexGeometry> &Collider::GetGeometry() const { return convexGeometries; }




void GJKCollisionStrategy::Init() {
}

void GJKCollisionStrategy::Deinit() {
}

bool GJKCollisionStrategy::AreColliding(const Collider &collider1, const Collider &collider2) {
	auto &geometry1 = collider1.GetGeometry();
	auto &geometry2 = collider2.GetGeometry();

	geometry1Model = collider1.GetTransform().ComputeTransformMatrix();
	geometry2Model = collider2.GetTransform().ComputeTransformMatrix();

	for (auto i = 0; i < geometry1.size(); i++) {
		convexGeometry1 = &geometry1[i];
		for (auto j = 0; j < geometry2.size(); j++) {
			convexGeometry2 = &geometry2[j];
			if (AreConvexGeometriesColliding()) {
				return true;
			}
		}
	}

	return false;
}

glm::vec3 GJKCollisionStrategy::SupportVertexForDirection(const glm::vec3 &direction) {
	auto vert1 = convexGeometry1->SupportPointFor(direction, geometry1Model);
	auto vert2 = convexGeometry2->SupportPointFor(-direction, geometry2Model);

	return vert1 - vert2;
}

bool GJKCollisionStrategy::SameDirection(const glm::vec3 &vec1, const glm::vec3 &vec2) {
	return glm::dot(vec1, vec2) > 0;
}

void GJKCollisionStrategy::EvolveLineSimplex(GJKCollisionStrategy::Simplex &simplex, glm::vec3 &direction) {
	glm::vec3 ab = simplex.B - simplex.A;
	glm::vec3 ao = -simplex.A;

	if (SameDirection(ab, ao)) {
		direction = glm::cross(glm::cross(ab, ao), ab);
		return;
	}
	simplex.Set(simplex.A);
	direction = ao;
}

void GJKCollisionStrategy::EvolveTriangleSimplex(GJKCollisionStrategy::Simplex &simplex, glm::vec3 &direction) {
	glm::vec3 a = simplex.A;
	glm::vec3 b = simplex.B;
	glm::vec3 c = simplex.C;

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao =   - a;

	glm::vec3 abc = glm::cross(ab, ac);

	if (SameDirection(glm::cross(abc, ac), ao)) {
		if (SameDirection(ac, ao)) {
			simplex.Set(a, c);
			direction = glm::cross(glm::cross(ac, ao), ac);
		} else {
			simplex.Set(a, b);
			return EvolveLineSimplex(simplex, direction);
		}
	} else {
		if (SameDirection(glm::cross(ab, abc), ao)) {
			simplex.Set(a, b);
			return EvolveLineSimplex(simplex, direction);
		} else {
			if (SameDirection(abc, ao)) {
				direction = abc;
			} else {
				simplex.Set(a, c, b);
				direction = -abc;
			}
		}
	}
}

bool GJKCollisionStrategy::EvolveTetrahedronSimplex(GJKCollisionStrategy::Simplex &simplex, glm::vec3 &direction) {
	glm::vec3 ab = simplex.B - simplex.A;
	glm::vec3 ac = simplex.C - simplex.A;
	glm::vec3 ad = simplex.D - simplex.A;
	glm::vec3 ao = -simplex.A;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (SameDirection(abc, ao)) {
		simplex.Set(simplex.A, simplex.B, simplex.C);
		EvolveTriangleSimplex(simplex, direction);
		return false;
	}

	if (SameDirection(acd, ao)) {
		simplex.Set(simplex.A, simplex.C, simplex.D);
		EvolveTriangleSimplex(simplex, direction);
		return false;
	}

	if (SameDirection(adb, ao)) {
		simplex.Set(simplex.A, simplex.D, simplex.B);
		EvolveTriangleSimplex(simplex, direction);
		return false;
	}

	return true;
}
/*
std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
	os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
	return os;
}
 */

bool GJKCollisionStrategy::AreConvexGeometriesColliding() {
	glm::vec3 direction(1, 1, 1);
	Simplex simplex;

	glm::vec3 support = SupportVertexForDirection(direction);
	simplex.Insert(support);
	direction = -1.0f * support;

	while (true) {
		support = SupportVertexForDirection(direction);
		if (glm::dot(support, direction) <= 0) {
			return false;
		}
/*
		std::cout << "Simplex" << std::endl;
		if (simplex.VerticesNum >= 1) {
			std::cout << "A:" << simplex.A << std::endl;
		}
		if (simplex.VerticesNum >= 2) {
			std::cout << "B:" << simplex.B << std::endl;
		}
		if (simplex.VerticesNum >= 3) {
			std::cout << "C:" << simplex.C << std::endl;
		}
		if (simplex.VerticesNum >= 4) {
			std::cout << "D:" << simplex.D << std::endl;
		}
		std::cout << std::endl << std::endl;
*/

		simplex.Insert(support);
		if (simplex.VerticesNum == 2) {
			EvolveLineSimplex(simplex, direction);
		} else if (simplex.VerticesNum == 3) {
			EvolveTriangleSimplex(simplex, direction);
		} else if (simplex.VerticesNum == 4) {
			if (EvolveTetrahedronSimplex(simplex, direction)) {
				return true;
			}
		} else {
			throw std::logic_error("GJK: Logic Error");
		}
	}
}
