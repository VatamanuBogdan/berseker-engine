#pragma once


#include <glm/glm.hpp>

#include <vector>
#include <utility>
#include "Scene/Components/Transform.h"


class ConvexGeometry {
public:
	void Reserve(size_t size);
	void PushBack(const glm::vec3 &vertex);

	[[nodiscard]] const glm::vec3& operator[](size_t i) const;
	[[nodiscard]] size_t GetSize() const;

	[[nodiscard]] glm::vec3 SupportPointFor(const glm::vec3 &direction, const glm::mat4 &model) const;

private:
	std::vector<glm::vec3> convexGeometry;
};


class Collider {
public:
	explicit Collider(std::vector<ConvexGeometry> &&convexGeometries, const Transform &transform);
	Collider(Collider &&collider);

	[[nodiscard]] const Transform& GetTransform() const;
	[[nodiscard]] const std::vector<ConvexGeometry>& GetGeometry() const;

	void SetTransform(const Transform &transform);

private:
	std::vector<ConvexGeometry> 	convexGeometries;
	Transform				transform;
};


struct CollisionData {
	bool valid;
	size_t component1Index;
	size_t component2Index;
};

class CollisionDetectionStrategy {
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual CollisionData AreColliding(const Collider &collider1, const Collider &collider2) = 0;
};


class GJKCollisionStrategy : public CollisionDetectionStrategy {
public:
	void Init() override;
	void Deinit() override;

	CollisionData AreColliding(const Collider &collider1, const Collider &collider2) override;

private:
	struct Simplex {
		void Set(const glm::vec3 &a) {
			VerticesNum = 1;
			A = a;
		}

		void Set(const glm::vec3 &a, const glm::vec3 &b) {
			VerticesNum = 2;
			A = a;	B = b;
		}

		void Set(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
			VerticesNum = 3;
			A = a;	B = b;	C = c;
		}

		void Insert(const glm::vec3 &point) {
			D = C; C = B; B = A; A = point;
			VerticesNum = glm::min(VerticesNum + 1, 4);
		}

		int VerticesNum = 0;
		glm::vec3 A, B, C, D;
	};

private:
	glm::vec3 SupportVertexForDirection(const glm::vec3& direction);
	bool SameDirection(const glm::vec3 &vec1, const glm::vec3 &vec2);

	void EvolveLineSimplex(Simplex &simplex, glm::vec3 &direction);
	void EvolveTriangleSimplex(Simplex &simplex, glm::vec3 &direction);
	bool EvolveTetrahedronSimplex(Simplex &simplex, glm::vec3 &direction);
	bool AreConvexGeometriesColliding();

private:
	glm::mat4		geometry1Model;
	glm::mat4		geometry2Model;
	const ConvexGeometry 	*convexGeometry1 = nullptr;
	const ConvexGeometry 	*convexGeometry2 = nullptr;
};