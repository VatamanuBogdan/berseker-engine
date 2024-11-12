#pragma once
#include "Entity.h"
#include "Component.h"
#include "ComponentSet.h"

#include <vector>
#include <any>

namespace Ecs {

class Registry {
public:
	~Registry();

	Entity CreateEntity();

	template <typename ComponentType, typename... Args>
	ComponentType& AddComponentTo(Entity &entity, Args&&... args) {
		VerifyComponentSetCreationFor<ComponentType>();

		size_t entityId = entity.GetId();
		return GetComponentSetFor<ComponentType>().template Add(entityId, std::forward<Args>(args)...);
	}

	template<typename ComponentType>
	ComponentType* GetComponentFrom(const Entity &entity) {
		if (!HasComponentSetFor<ComponentType>()) {
			return nullptr;
		}

		ComponentSet<ComponentType> &componentSet = GetComponentSetFor<ComponentType>();
		size_t entityId = entity.GetId();
		if (!componentSet.Contains(entityId)) {
			return nullptr;
		}
		return &componentSet.GetComponentFor(entityId);
	}

	template<typename ComponentType>
	void RemoveComponentFrom(Entity &entity) {
		if (!HasComponentSetFor<ComponentType>()) {
			return;
		}

		ComponentSet<ComponentType> &componentSet = GetComponentSetFor<ComponentType>();
		size_t entityId = entity.GetId();
		componentSet.Remove(entityId);
	}

private:
	template <typename ComponentType>
	bool HasComponentSetFor() {
		size_t componentId = Component::Id<ComponentType>();
		if (componentSets.size() <= componentId) {
			return false;
		}
		return componentSets[componentId] != nullptr;
	}

	template<typename ComponentType>
	void VerifyComponentSetCreationFor() {
		size_t componentId = Component::Id<ComponentType>();
		if (componentSets.size() <= componentId) {
			componentSets.resize(componentId + 1, nullptr);
			componentSetDeleters.resize(componentId + 1, nullptr);
		}
		if (!componentSets[componentId]) {
			componentSets[componentId] = new ComponentSet<ComponentType>(1);
			componentSetDeleters[componentId] = ComponentSetDeleter<ComponentType>;
		}
	}

	template <typename ComponentType>
	ComponentSet<ComponentType>& GetComponentSetFor() {
		size_t componentId = Component::Id<ComponentType>();
		return *reinterpret_cast<ComponentSet<ComponentType>*>(componentSets[componentId]);
	}

	template<typename ComponentType>
	static void ComponentSetDeleter(void *componentSet) {
		delete reinterpret_cast<ComponentSet<ComponentType>*>(componentSet);
	}

private:
	std::vector<void*>		componentSets;
	std::vector<void (*)(void*)>	componentSetDeleters;
	Entity::IdType			entitiesIdCounter = 0;
};

}
