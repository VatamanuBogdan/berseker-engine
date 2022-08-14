#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>

namespace Ecs {

class Component {
public:
	using IdType = uint32_t;

	Component() = delete;

	template <typename ComponentType>
	static IdType Id() {
		static const IdType id = IdCounter++;
		return id;
	}

private:
	static IdType IdCounter;
};

}
