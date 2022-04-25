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
	virtual ~PrimitiveProps() = default;

	PrimitiveBody BodyType;

	explicit PrimitiveProps(const PrimitiveBody &body)
		: BodyType(body) {
	}

};

struct Primitive3DProps : public PrimitiveProps {
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	explicit Primitive3DProps(const glm::vec3 &position,
					  const glm::vec3 &rotation,
					  const glm::vec3 &scale,
					  const PrimitiveBody &body)
		  : Position(position), Rotation(rotation), Scale(scale), PrimitiveProps(body) {
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