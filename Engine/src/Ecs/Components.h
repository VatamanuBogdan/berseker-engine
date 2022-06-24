#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>

namespace Ecs {

static constexpr uint32_t MaxComponentsNum = 64;

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

class ComponentStorage {
public:
	ComponentStorage();
	explicit ComponentStorage(size_t stride, size_t size);
	ComponentStorage(ComponentStorage &&storage) noexcept;

	ComponentStorage& operator=(ComponentStorage &&storage) noexcept;

	template <typename ComponentType, typename... Args>
	ComponentType& ConstructAt(size_t index, Args&&... args) {
		new (memory.data() + index * stride)ComponentType(std::forward<Args>(args)...);
		return this->template operator[]<ComponentType>(index);
	}

	template <typename ComponentType>
	void DestructAt(size_t index) {
		this->template operator[]<ComponentType>(index).~ComponentType();
	}

	template <typename ComponentType>
	ComponentType& operator[](size_t id) {
		return *reinterpret_cast<ComponentType*>(memory.data() + id * stride);
	}

	void Resize(size_t size);
	[[nodiscard]] bool IsValid() const;

private:
	size_t				stride;
	std::vector<uint8_t> 		memory;
};

}
