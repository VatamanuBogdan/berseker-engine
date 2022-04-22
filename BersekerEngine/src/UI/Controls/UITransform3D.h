#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>

class UITransform3D {
public:
	class Adapter {
	public:
		virtual void SetPosition(const glm::vec3 &position) = 0;
		virtual void SetRotation(const glm::vec3 &rotation) = 0;
		virtual void SetScale(const glm::vec3 &scale) = 0;

		virtual const glm::vec3& GetPosition() = 0;
		virtual const glm::vec3& GetRotation() = 0;
		virtual const glm::vec3& GetScale() = 0;

		virtual const char* GetID() = 0;
	};

public:
	template<typename ForwardIterType>
	void SetAdapters(ForwardIterType begin, ForwardIterType end) {
		for (auto iter = begin; iter != end; iter++) {
			this->adaptersContainer.push_back(*iter);
		}
	}
	void Draw();

private:
	static bool ItemGetterWrapper(void* data, int index, const char** outText);
	const char* ItemGetter(int index);

private:
	using AdaptersContainerType = std::vector<std::shared_ptr<Adapter>>;

	int 				adapterIndex = -1;
	AdaptersContainerType 	adaptersContainer;
};
