#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Rendering/Primitives/Primitives.h"
#include "Scene/Camera.hpp"
#include "Rendering/Mesh.h"

#include <glm/glm.hpp>

#include <memory>
#include <bitset>
#include <array>

class PrimitivesRenderer;

class PrimitivesStorage {
private:
	enum PrimitiveGeometry {
		SHAPED_CUBE_GEOMETRY = 0,
		FILLED_CUBE_GEOMETRY,
		SHAPED_SPHERE_GEOMETRY,
		GEOMETRIES_COUNT
	};
	static constexpr size_t StorageSize = GEOMETRIES_COUNT * sizeof(Mesh);

private:
	PrimitivesStorage() = default;
	~PrimitivesStorage();

	void SetGeometry(PrimitiveGeometry geometry, Mesh &&mesh) {
		auto targetMesh = reinterpret_cast<Mesh*>(storage.data() + geometry * sizeof(Mesh));
		if (flags[geometry]) {
			targetMesh->~Mesh();
		}
		new (targetMesh) Mesh(std::move(mesh));
		flags[geometry] = true;
	}

	Mesh& GetGeometry(PrimitiveGeometry geometry) {
		auto targetMesh = reinterpret_cast<Mesh*>(storage.data() + geometry * sizeof(Mesh));
		return *targetMesh;
	}

private:
	std::array<uint8_t, StorageSize>	storage{};
	std::bitset<GEOMETRIES_COUNT>		flags{};
	friend class PrimitivesRenderer;
};

class PrimitivesRenderer {
public:
	void Init();
	void Deinit();

	void AttachCamera(std::shared_ptr<Camera> &camera);
	void RenderCube(const CubeProps &props);
	void RenderSphere(const SphereProps &props);

private:
	void InitCubeGeometry();
	void InitSphereGeometry();

private:
	PrimitivesStorage		storage;
	std::shared_ptr<Camera> camera = nullptr;
};

