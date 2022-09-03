#pragma once
#include <cstdint>

namespace Ecs {

class Registry;

class Entity {
public:
	Entity(const Entity& entity) = default;
	Entity(Entity &&entity) noexcept;

	bool operator==(const Entity &entity) const {
		return id == entity.id;
	}

private:
	using IdType = uint32_t;

	explicit Entity(IdType id);
	[[nodiscard]] IdType GetId() const { return id; }

private:
	IdType id;
	friend class Registry;
};

}
