#pragma once
#include <cstdint>
#include <bitset>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_set>


class ECS {
public:
	ECS() = delete;

	using ComponentIDType = uint32_t;
	using EntityIDType = uint32_t;

	static constexpr uint32_t MaxComponents = 64;
	static constexpr uint32_t MaxEntities = 1024;

	template<typename ComponentType>
	static ComponentIDType ComponentID() {
		static const ComponentIDType id = ComponentsIDCounter++;
		return id;
	}

	class Registry;

	class Entity {
	public:
		[[nodiscard]] EntityIDType GetID() const { return id; }

		Entity(const Entity&) = delete;

		Entity(Entity &&entity) noexcept
			  : id(entity.id), mask(entity.mask) {
			entity.id = -1;
			entity.mask.reset();
		}

	private:
		 explicit Entity(EntityIDType id)
			: id(id) {
		}

		EntityIDType 			id;
		std::bitset<MaxComponents>	mask{};
		friend class Registry;
	};

private:
	class ComponentStorage {
	public:
		ComponentStorage()
			: stride(0) {
		}

		explicit ComponentStorage(size_t stride, size_t size)
			: stride(stride) {
			memory.resize(stride * size);
		}

		ComponentStorage(ComponentStorage &&storage) noexcept
			  : stride(storage.stride), memory(std::move(storage.memory)) {
		}

		ComponentStorage& operator=(ComponentStorage &&storage) noexcept {
			stride = storage.stride;
			memory = std::move(storage.memory);
			return *this;
		}

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
		ComponentType& operator[](EntityIDType entityID) {
			return *reinterpret_cast<ComponentType*>(memory.data() + entityID * stride);
		}

		void Resize(size_t size) {
			memory.resize(size);
		}

		[[nodiscard]] bool IsValid() const { return memory.empty(); }

	private:
		size_t				stride;
		std::vector<uint8_t> 		memory;
	};

public:
	class Registry {
	public:
		static Entity CreateEntity() {
			Entity entity(EntitiesIDCounter++);
			return std::move(entity);
		}

		template <typename ComponentType, typename... Args>
		ComponentType& AddComponentTo(Entity &entity, Args&&... args) {
			auto componentId = ComponentID<ComponentType>();

			entity.mask.set(componentId);
			if (componentsStorages.size() <= componentId) {
				componentsStorages.resize(componentId + 1);
			}

			auto &storage = componentsStorages[componentId];
			if (storage.IsValid()) {
				storage = std::move(ComponentStorage(sizeof(ComponentType), MaxEntities));
			}
			return storage.template ConstructAt<ComponentType>(entity.GetID(), std::forward<Args>(args)...);
		}

		template<typename ComponentType>
		ComponentType* GetComponentFrom(const Entity &entity) {
			auto componentId = ComponentID<ComponentType>();

			if (!entity.mask[ComponentID<ComponentType>()]) {
				return nullptr;
			}
			return &componentsStorages[componentId].template operator[]<ComponentType>(entity.GetID());
		}

		template<typename ComponentType>
		void RemoveComponentFrom(Entity &entity) {
			auto componentId = ComponentID<ComponentType>();

			if (!entity.mask[componentId]) {
				return;
			}

			auto &storage = componentsStorages[componentId];
			storage.template DestructAt<ComponentType>(entity.GetID());
			entity.mask.set(componentId, false);
		}

	private:
		std::vector<ComponentStorage>		componentsStorages;
	};

private:
	static EntityIDType 	EntitiesIDCounter;
	static ComponentIDType	ComponentsIDCounter;
};

