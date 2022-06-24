#include "Components.h"

namespace Ecs {

Component::IdType Component::IdCounter = 0;

ComponentStorage::ComponentStorage()
	: stride(0) {
}

ComponentStorage::ComponentStorage(size_t stride, size_t size)
	: stride(stride) {
	memory.resize(stride * size);
}

ComponentStorage::ComponentStorage(ComponentStorage &&storage) noexcept
	  : stride(storage.stride), memory(std::move(storage.memory)) {
}

ComponentStorage &ComponentStorage::operator=(ComponentStorage &&storage) noexcept {
	stride = storage.stride;
	memory = std::move(storage.memory);
	return *this;
}

void ComponentStorage::Resize(size_t size) {
	memory.resize(size);
}

bool ComponentStorage::IsValid() const { return memory.empty(); }

}