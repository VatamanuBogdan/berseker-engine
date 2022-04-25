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
		virtual void SetPosition(const glm::vec3 &position, int index) = 0;
		virtual void SetRotation(const glm::vec3 &rotation, int index) = 0;
		virtual void SetScale(const glm::vec3 &scale, int index) = 0;

		virtual const glm::vec3& GetPosition(int index) = 0;
		virtual const glm::vec3& GetRotation(int index) = 0;
		virtual const glm::vec3& GetScale(int index) = 0;

		virtual int GetSize() = 0;

		virtual const char* GetID(int index) = 0;
	};

public:
	void Draw(Adapter &adapter);

private:
	static bool IDGetterWrapper(void* data, int index, const char** outText);

private:
	int adapterIndex = -1;
};
