#pragma once
#include "Rendering/Mesh.h"

#include <assimp/scene.h>

#include <vector>
#include <cstddef>

class Model {
public:
	explicit Model(std::vector<Mesh> &&meshes)
		: meshes(std::move(meshes)) {
	}

	Model(Model &&model) noexcept {
		this->meshes = std::move(model.meshes);
	}

	const std::vector<Mesh>& GetMeshes() { return meshes; }

private:
	std::vector<Mesh> meshes;
	friend class ModelLoader;
};

struct Vertex {
	glm::vec3 	Position;
	glm::vec3	Normal;

	Vertex(const glm::vec3 &position, const glm::vec3 normal)
		: Position(position), Normal(normal) {
	}

	static VertexFormat<3> GetFormat() noexcept {
		return VertexFormat<3>().
		        Push<float>(3, offsetof(Vertex, Position)).
			  Push<float>(3, offsetof(Vertex, Normal));
	}
};

class ModelLoader {
public:
	Model LoadModel(const char *modelPath);

private:
	void LoadNode(const aiNode &node);

private:
	const aiScene			*scene;
	std::vector<Mesh>			meshes;
	std::vector<Vertex> 		vertices;
	std::vector<unsigned int> 	indices;
};