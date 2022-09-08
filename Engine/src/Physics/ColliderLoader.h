#pragma once
#include "Collider.h"
#include "Rendering/GraphicsAPI/VertexArray.hpp"

#include <utility>

struct RenderableCollider {
	VertexArray vao;
	size_t	size;

	RenderableCollider() = default;
	RenderableCollider(RenderableCollider &&renderableCollider) noexcept;

	std::vector<std::pair<size_t, size_t>> componentRanges;
};


class ColliderLoader {
public:
	std::pair<Collider, RenderableCollider> LoadCollider(const char *colliderPath);
};