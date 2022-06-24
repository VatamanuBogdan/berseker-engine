#pragma once
#include "Entity.h"

#include <cstdint>


namespace Ecs {

class Registry {
public:
	static constexpr uint32_t MaxEntities = 1024;

	Entity CreateEntity();

	template <typename ComponentType, typename... Args>
	ComponentType& AddComponentTo(Entity &entity, Args&&... args) {
		auto componentId = Component::Id<ComponentType>();

		entity.mask.set(componentId);
		if (componentsStorages.size() <= componentId) {
			componentsStorages.resize(componentId + 1);
		}

		auto &storage = componentsStorages[componentId];
		if (storage.IsValid()) {
			storage = std::move(ComponentStorage(sizeof(ComponentType), MaxEntities));
		}
		return storage.template ConstructAt<ComponentType>(entity.GetId(), std::forward<Args>(args)...);
	}

	template<typename ComponentType>
	ComponentType* GetComponentFrom(const Entity &entity) {
		auto componentId = Component::Id<ComponentType>();

		if (!entity.mask[Component::Id<ComponentType>()]) {
			return nullptr;
		}
		return &componentsStorages[componentId].template operator[]<ComponentType>(entity.GetId());
	}

	template<typename ComponentType>
	void RemoveComponentFrom(Entity &entity) {
		auto componentId = Component::Id<ComponentType>();

		if (!entity.mask[componentId]) {
			return;
		}

		auto &storage = componentsStorages[componentId];
		storage.template DestructAt<ComponentType>(entity.GetId());
		entity.mask.set(componentId, false);
	}

private:
	std::vector<ComponentStorage>		componentsStorages;
	Entity::IdType				entitiesIdCounter = 0;
};

}
