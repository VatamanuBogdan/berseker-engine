#pragma once
#include <cstdint>

namespace Ecs {

class Registry;

class Entity {
public:
	Entity(const Entity&) = delete;
	Entity(Entity &&entity) noexcept;

private:
	using IdType = uint32_t;

	explicit Entity(IdType id);
	[[nodiscard]] IdType GetId() const { return id; }

private:
	IdType id;
	friend class Registry;
};

}
