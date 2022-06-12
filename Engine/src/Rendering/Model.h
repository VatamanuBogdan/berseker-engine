#pragma once
#include "Rendering/Mesh.h"
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include <assimp/scene.h>

#include <vector>
#include <cstddef>

#include "Rendering/Material.hpp"

class Model {
public:
	explicit Model(std::vector<Mesh> &&meshes, std::vector<Material> &&materials, std::vector<int> &&meshMaterialLink)
		: meshes(std::move(meshes)),
		  materials(std::move(materials)),
		  meshMaterialLink(std::move(meshMaterialLink)) {
	}

	Model(Model &&model) noexcept {
		this->meshes = std::move(model.meshes);
		this->materials = std::move(model.materials);
		this->meshMaterialLink = std::move(model.meshMaterialLink);
	}

	[[nodiscard]] const std::vector<Mesh>& GetMeshes() const { return meshes; }
	[[nodiscard]] const std::vector<Material>& GetMaterials() const { return materials; }
	[[nodiscard]] std::vector<Material>& GetMaterials() { return materials; }
	[[nodiscard]] const Material& GetMaterialFor(int index) const { return materials[meshMaterialLink[index]]; }

private:
	std::vector<Mesh>		meshes;
	std::vector<Material>	materials;
	std::vector<int>		meshMaterialLink;
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
	Model LoadModel(const char *modelPath, const std::shared_ptr<ShaderProgram> &defaultShader);

private:
	void InitForLoading();
	void LoadNode(const aiNode &node);
	void LoadMaterials();

private:
	const aiScene			*scene;
	std::vector<Mesh>			 meshes;
	std::vector<Material>		 materials;
	std::vector<int>			 materialLink;
	std::vector<Vertex> 		 vertices;
	std::vector<unsigned int> 	 indices;
	std::shared_ptr<ShaderProgram> defaultShader;
};