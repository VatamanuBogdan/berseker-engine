#pragma once
#include "Components.h"

#include <bitset>

namespace Ecs {

class Registry;

class Entity {
public:
	using IdType = uint32_t;

	Entity(const Entity&) = delete;
	Entity(Entity &&entity) noexcept;

	[[nodiscard]] IdType GetId() const { return id; }

private:
	explicit Entity(IdType id)
		  : id(id) {
	}

	IdType				id;
	std::bitset<MaxComponentsNum>	mask{};
	friend class Registry;
};

}
