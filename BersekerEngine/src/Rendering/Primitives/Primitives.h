#pragma once
#include "Rendering/Utils.h"

#include <glm/glm.hpp>

struct PrimitiveBody {
	enum Type { FILLED, SHAPED };

	explicit PrimitiveBody(Type type, const Color &color, float edgeWidth = 1.0f)
		: Type(type), BodyColor(color), EdgeWidth(edgeWidth) {
	}

	Type	Type;
	Color	BodyColor;
	float EdgeWidth;
};

struct PrimitiveProps {
	PrimitiveBody BodyType;

	explicit PrimitiveProps(const PrimitiveBody &body)
		: BodyType(body) {
	}

};

struct Primitive3DProps : public PrimitiveProps {
	glm::vec3 Position;
	glm::vec3 Rotation;

	explicit Primitive3DProps(const glm::vec3 &position,
					  const glm::vec3 &rotation,
					  const PrimitiveBody &body)
		  : Position(position), Rotation(rotation), PrimitiveProps(body) {
	}
};

struct CubeProps : public Primitive3DProps {
	float Width;

	explicit CubeProps(float width, const Primitive3DProps &props)
		  : Width(width), Primitive3DProps(props) {
	}
};

struct SphereProps : public Primitive3DProps {
	float Radius;

	explicit SphereProps(float radius, const Primitive3DProps &props)
		  : Radius(radius), Primitive3DProps(props) {
	}
};