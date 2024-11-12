#include "Model.h"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "ResourceLoaders/ShaderRegistry.h"

#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static auto vertexFormat = Vertex::GetFormat();

void ModelLoader::LoadNode(const aiNode &node) {
	for (unsigned int i = 0; i < node.mNumMeshes; i++) {
		aiMesh &mesh = *scene->mMeshes[node.mMeshes[i]];
		vertices.clear();
		for (unsigned int j = 0; j < mesh.mNumVertices; j++) {
			glm::vec3 position(mesh.mVertices[j].x, mesh.mVertices[j].y, mesh.mVertices[j].z);
			glm::vec3 normal(mesh.mNormals[j].x, mesh.mNormals[j].y, mesh.mNormals[j].z);

			minPoint = glm::min(minPoint, position);
			maxPoint = glm::max(maxPoint, position);

			vertices.emplace_back(position, normal);
		}

		indices.clear();
		for (unsigned int j = 0; j < mesh.mNumFaces; j++) {
			aiFace &face = mesh.mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {;
				indices.push_back(face.mIndices[k]);
			}
		}

		meshes.emplace_back(
			  vertexFormat,
			  VertexBuffer(vertices),
			  IndexBuffer(indices)
		);

		materialLink.emplace_back(mesh.mMaterialIndex);

		OnDebug(meshes.back().SetName(mesh.mName.C_Str()));
	}

	for (unsigned int i = 0; i < node.mNumChildren; i++) {
		LoadNode(*node.mChildren[i]);
	}
}

void ModelLoader::InitForLoading() {
	meshes.clear();
	materials.clear();
	materialLink.clear();

	minPoint = glm::vec3(std::numeric_limits<float>::max());
	maxPoint = glm::vec3(std::numeric_limits<float>::min());
}

void ModelLoader::LoadMaterials() {
	materials.reserve(scene->mNumMaterials);
	for (auto i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];
		aiColor3D ambient, diffuse, specular;
		float shininess;
		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material->Get(AI_MATKEY_SHININESS, shininess);

		materials.emplace_back(
			  material->GetName().C_Str(),
			  glm::vec3 { ambient.r, ambient.g, ambient.b },
			  glm::vec3 { diffuse.r, diffuse.g, diffuse.b },
			  glm::vec3 { specular.r, specular.g, specular.b },
			  shininess,
			  defaultShader
		);

		materials.back().SetLighted(true);
	}
}

Model ModelLoader::LoadModel(const char *modelPath, const std::shared_ptr<ShaderProgram> &defaultShader) {
	try {
		this->defaultShader = defaultShader;

		Assimp::Importer importer;
		scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		InitForLoading();
		LoadNode(*scene->mRootNode);
		LoadMaterials();
		return Model(
			  std::move(meshes),
			  std::move(materials),
			  std::move(materialLink),
			  AABB((maxPoint + minPoint) / 2.0f, maxPoint - minPoint)
	  	);
	} catch (std::exception &e) {
		throw e;
	}
}


