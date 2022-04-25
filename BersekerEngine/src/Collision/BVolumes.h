#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace BVolumes {
	class CollisionDetector;

	class BoundingVolume {
	public:
		enum class TypeTag : unsigned short {
			AABB = 0,
			Sphere,
			OBB,
			LENGTH
		};
		const TypeTag Type;

	protected:
		explicit BoundingVolume(TypeTag type)
			: Type(type) {
		}
	};


	class AABB : public BoundingVolume {
	public:
		AABB(const glm::vec3 &position, glm::vec3 widths)
			: BoundingVolume(TypeTag::AABB), Position(position), HalfWidths(widths / glm::vec3(2)) {
		}

		[[nodiscard]] glm::vec3 ComputeClosesPointFor(const glm::vec3 &point) const;
		[[nodiscard]] float ComputeSquaredDistanceToClosesPointFor(const glm::vec3 &point) const;

	public:
		glm::vec3 Position;
		glm::vec3 HalfWidths;
	};


	class Sphere : public BoundingVolume {
	public:
		Sphere(const glm::vec3 &position, float radius)
			  : BoundingVolume(TypeTag::Sphere), Position(position), Radius(radius) {
		}

	public:
		glm::vec3 	Position;
		float 	Radius;
	};


	class OBB : public BoundingVolume {
	public:
		struct Direction {
			glm::vec3 XAxis;
			glm::vec3 YAxis;
			glm::vec3 ZAxis;

			Direction()
				: XAxis(glm::vec3(1, 0, 0)),
				  YAxis(glm::vec3(0, 1, 0)),
				  ZAxis(glm::vec3(0, 0, 1)) {
			}

			void SetRotation(const glm::vec3 &rotation) {
				glm::mat4 transform(1);
				transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1.0));
				transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1.0, 0));
				transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0, 0));

				YAxis = glm::normalize(glm::vec3(transform * glm::vec4(0, 1, 0, 1)));
				ZAxis  = glm::normalize(glm::vec3(transform * glm::vec4(0, 0, 1, 1)));
				XAxis = glm::normalize(glm::cross(YAxis, ZAxis));
			}
		};

	public:
		OBB(const glm::vec3 &position, const glm::vec3 &widths, const glm::vec3 rotation = glm::vec3(0))
			: BoundingVolume(TypeTag::OBB), Position(position), HalfWidths(widths / glm::vec3(2)), Directions() {
			Directions.SetRotation(rotation);
		}

		glm::vec3 Position;
		glm::vec3 HalfWidths;
		Direction Directions;
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

		static bool AreCollidingDispatch(const BoundingVolume &bvolume1, const BoundingVolume &bvolume2);

	private:
		class Dispatcher {
		public:
			Dispatcher() = delete;

			using Tag = BoundingVolume::TypeTag;
			using TableFunctionType = bool (*)(const BoundingVolume&, const BoundingVolume&);

			template <typename BVolumeType1, typename BVolumeType2>
			static inline bool Dispatch(const BoundingVolume &bVolume1, const BoundingVolume &bVolume2) {
				return CollisionDetector::AreColliding(
					  reinterpret_cast<const BVolumeType1&>(bVolume1),
					  reinterpret_cast<const BVolumeType2&>(bVolume2)
			    	);
			}

			static inline bool CallAreCollidingDispatched(const BoundingVolume &bVolume1,
										    const BoundingVolume &bVolume2) {
				DispatchingTable[TableKeyFor(bVolume1.Type, bVolume2.Type)](bVolume1, bVolume2);
			}

		private:
			static constexpr auto TableKeyFor = [](const Tag &tag1, const Tag &tag2) {
				auto length = static_cast<std::underlying_type<Tag>::type>(Tag::LENGTH);
				auto value1 = static_cast<std::underlying_type<Tag>::type>(tag1);
				auto value2 = static_cast<std::underlying_type<Tag>::type>(tag2);
				return length * value1 + value2;
			};

			template<Tag Type1, Tag Type2>
			static constexpr auto TableKeyForTypes = []() constexpr {
				return TableKeyFor(Type1, Type2);
			};

			static constexpr size_t TableMaxKey = []() constexpr {
				auto length = static_cast<std::underlying_type<Tag>::type>(Tag::LENGTH);
				return length * length;
			}();

		private:
			static constexpr auto DispatchingTable = []() constexpr {
				std::array<TableFunctionType, TableMaxKey> table{};
				table[TableKeyForTypes<Tag::AABB, Tag::AABB>()] = Dispatch<AABB, AABB>;
				table[TableKeyForTypes<Tag::AABB, Tag::Sphere>()] = Dispatch<AABB, Sphere>;
				table[TableKeyForTypes<Tag::AABB, Tag::OBB>()] = Dispatch<AABB, OBB>;
				table[TableKeyForTypes<Tag::Sphere, Tag::AABB>()] = Dispatch<Sphere, AABB>;
				table[TableKeyForTypes<Tag::Sphere, Tag::Sphere>()] = Dispatch<Sphere, Sphere>;
				table[TableKeyForTypes<Tag::Sphere, Tag::OBB>()] = Dispatch<Sphere, OBB>;
				table[TableKeyForTypes<Tag::OBB, Tag::AABB>()] = Dispatch<OBB, AABB>;
				table[TableKeyForTypes<Tag::OBB, Tag::Sphere>()] = Dispatch<OBB, Sphere>;
				table[TableKeyForTypes<Tag::OBB, Tag::OBB>()] = Dispatch<OBB, OBB>;

				return table;
			}();
		};

	private:
		CollisionDetector() = default;
	};

}