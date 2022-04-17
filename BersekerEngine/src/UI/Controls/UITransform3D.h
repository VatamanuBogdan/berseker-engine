#pragma once
#include <glm/glm.hpp>


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
	};

public:
	void Draw(UITransform3D::Adapter &adapter);
};
