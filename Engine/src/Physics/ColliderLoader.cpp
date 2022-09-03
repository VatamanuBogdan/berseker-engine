#include "ColliderLoader.h"

#include <fstream>
#include <stdexcept>

using namespace std::string_literals;

RenderableCollider::RenderableCollider(RenderableCollider &&renderableCollider) noexcept
	: vao(std::move(renderableCollider.vao)), size(renderableCollider.size) {
}


std::pair<Collider, RenderableCollider> ColliderLoader::LoadCollider(const char *colliderPath) {
	std::ifstream inStream(colliderPath);
	if (!inStream.is_open()) {
		throw std::runtime_error("Failed on opening "s + colliderPath + " file");
	}

	std::vector<ConvexGeometry> colliderConvexComponents;

	size_t convexComponentsNum, verticesNum;
	inStream >> convexComponentsNum;
	colliderConvexComponents.reserve(convexComponentsNum);


	struct RenderableColliderPoint {
		glm::vec3 	Coords;
		int 		ComponentIndex;

		RenderableColliderPoint(const glm::vec3 coords, int componentIndex)
			  : Coords(coords), ComponentIndex(componentIndex) {
		}

	};
	std::vector<RenderableColliderPoint> renderableColliderPoints;

	char type;
	float x, y, z;
	for (size_t i = 0; i <  convexComponentsNum; i++) {
		inStream >> type >> verticesNum;
		if (type != 'o') {
			inStream.close();
			throw std::runtime_error("Collider wrong Format");
		}

		ConvexGeometry convexGeometry;	convexGeometry.Reserve(verticesNum);
		for (size_t j = 0; j < verticesNum; j++) {
			inStream >> x >> y >> z;
			convexGeometry.PushBack(glm::vec3(x, y, z));
			renderableColliderPoints.emplace_back(glm::vec3(x, y, z), i);
		}

		colliderConvexComponents.push_back(std::move(convexGeometry));
	}

	inStream.close();

	VertexFormat<2> vertexFormat;
	vertexFormat.Push<float>(3, offsetof(RenderableColliderPoint, Coords));
	vertexFormat.Push<int>(1, offsetof(RenderableColliderPoint, ComponentIndex));

	RenderableCollider renderableComponent;
	renderableComponent.vao.AttachVertexBuffer(VertexBuffer(renderableColliderPoints), vertexFormat);
	renderableComponent.size = renderableColliderPoints.size();

	return { Collider(std::move(colliderConvexComponents), Transform()), std::move(renderableComponent) };
}

