#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>


template <typename ComponentType>
class ComponentSet {
public:
	explicit ComponentSet(size_t maxElementId) {
		Resize(maxElementId);
	}

	template <typename... Args>
	ComponentType& Add(size_t elementId, Args&&... args) {
		if (Contains(elementId)) {
			return componentsArray[sparseArray[elementId]];
		}

		if (sparseArray.size() <= elementId) {
			Resize(elementId);
		}

		sparseArray[elementId] = packedArray.size();
		packedArray.push_back(elementId);
		componentsArray.template emplace_back(std::forward<Args>(args)...);

		return componentsArray.back();
	}

	ComponentType& GetComponentFor(size_t elementId) {
		return componentsArray[sparseArray[elementId]];
	}

	void Remove(size_t elementId) {
		size_t targetPackedId;
		if (!Contains(elementId, targetPackedId)) {
			return;
		}

		size_t lastPackedId = packedArray.size() - 1;
		sparseArray[elementId] = -1;
		sparseArray[packedArray[lastPackedId]] = targetPackedId;

		if (targetPackedId < lastPackedId) {
			packedArray[targetPackedId] = packedArray[lastPackedId];
			std::swap(componentsArray[targetPackedId], componentsArray[lastPackedId]);
		}

		componentsArray.pop_back();
		packedArray.pop_back();
	}

	bool Contains(size_t elementId) {
		size_t packedId;
		if (!Contains(elementId, packedId)) {
			return false;
		}
		return packedArray[packedId] == elementId;
	}

	void Resize(size_t maxElementId) {
		sparseArray.resize(maxElementId + 1, -1);
	}

public:
	using iterator = typename std::vector<ComponentType>::iterator;
	using const_iterator = typename std::vector<ComponentType>::const_iterator;

	iterator begin() { return componentsArray.begin(); }
	iterator end() { return  componentsArray.end(); }

	[[nodiscard]] const_iterator begin() const { return componentsArray.begin(); }
	[[nodiscard]] const_iterator end() const { return componentsArray.end(); }

	[[nodiscard]] const_iterator cbegin() const { return componentsArray.cbegin(); }
	[[nodiscard]] const_iterator cend() const { return componentsArray.cbegin();  }

private:
	bool Contains(size_t elementId, size_t &packedId) {
		if (sparseArray.size() <= elementId) {
			return false;
		}

		packedId = sparseArray[elementId];
		if (packedArray.size() <= packedId) {
			return false;
		}

		return packedArray[packedId] == elementId;
	}

private:
	std::vector<size_t>		sparseArray;
	std::vector<size_t>		packedArray;
	std::vector<ComponentType>	componentsArray;
};

