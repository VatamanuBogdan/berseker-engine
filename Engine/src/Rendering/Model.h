#pragma once
#include "Rendering/Mesh.h"
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "Collision/BVolumes.h"

#include <assimp/scene.h>

#include <vector>
#include <cstddef>

#include "Rendering/Material.hpp"

class Model {
public:
	explicit Model(std::vector<Mesh> &&meshes,
			   std::vector<Material> &&materials,
			   std::vector<int> &&meshMaterialLink,
			   BVolumes::AABB aabb)
		: meshes(std::move(meshes)),
		  materials(std::move(materials)),
		  meshMaterialLink(std::move(meshMaterialLink)),
		  aabb(aabb) {
	}

	Model(Model &&model) noexcept
		: meshes(std::move(model.meshes)),
		  materials(std::move(model.materials)),
		  meshMaterialLink(std::move(model.meshMaterialLink)),
		  aabb(model.aabb) {
	}

	[[nodiscard]] const std::vector<Mesh>& GetMeshes() const { return meshes; }
	[[nodiscard]] const std::vector<Material>& GetMaterials() const { return materials; }
	[[nodiscard]] std::vector<Material>& GetMaterials() { return materials; }
	[[nodiscard]] const Material& GetMaterialFor(int index) const { return materials[meshMaterialLink[index]]; }

	const BVolumes::AABB& GetAABB() { return aabb; }

private:
	std::vector<Mesh>		meshes;
	std::vector<Material>	materials;
	std::vector<int>		meshMaterialLink;
	BVolumes::AABB		aabb;
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
	glm::vec3 				 minPoint;
	glm::vec3 				 maxPoint;
	std::vector<Vertex> 		 vertices;
	std::vector<unsigned int> 	 indices;
	std::shared_ptr<ShaderProgram> defaultShader;
};