#pragma once


#include <glm/glm.hpp>

#include <vector>
#include <utility>


class ConvexGeometry {
public:
	void Reserve(size_t size);
	void PushBack(const glm::vec3 &vertex);

	[[nodiscard]] const glm::vec3& operator[](size_t i) const;
	[[nodiscard]] size_t GetSize() const;

	[[nodiscard]] glm::vec3 FindFurthestFor(const glm::vec3 &direction, const glm::mat4 &model) const;

private:
	std::vector<glm::vec3> convexGeometry;
};


class Collider {
public:
	explicit Collider(std::vector<ConvexGeometry> &&convexGeometries, const glm::mat4 &model = glm::mat4(1));

	[[nodiscard]] const glm::mat4& GetModel() const;
	[[nodiscard]] const std::vector<ConvexGeometry>& GetGeometry() const;

	void SetModel(const glm::mat4 &model);

private:
	std::vector<ConvexGeometry> 	convexGeometries;
	glm::mat4 				model;
};


class CollisionDetectionStrategy {
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual bool AreColliding(const Collider &collider1, const Collider &collider2) = 0;
};


class GJKCollisionStrategy : public CollisionDetectionStrategy {
public:
	void Init() override;
	void Deinit() override;

	bool AreColliding(const Collider &collider1, const Collider &collider2) override;

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
	const glm::mat4		*geometry1Model = nullptr;
	const glm::mat4		*geometry2Model = nullptr;
	const ConvexGeometry 	*convexGeometry1 = nullptr;
	const ConvexGeometry 	*convexGeometry2 = nullptr;
};