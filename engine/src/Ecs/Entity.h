#pragma once
#include <cstdint>

namespace Ecs {

class Registry;

class Entity {
public:
	using IdType = uint32_t;

	Entity(const Entity& entity) = default;
	Entity(Entity &&entity) noexcept;

	bool operator==(const Entity &entity) const {
		return id == entity.id;
	}

	[[nodiscard]] IdType GetId() const { return id; }

private:
	explicit Entity(IdType id);

private:
	IdType id;
	friend class Registry;
};

}
