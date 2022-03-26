#include "Model.h"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"

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
			auto &position = mesh.mVertices[j];
			auto &normal = mesh.mNormals[j];

			vertices.emplace_back(
				  glm::vec3 (position.x, position.y, position.z),
				  glm::vec3 (normal.x, normal.y, normal.z)
			);
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
			  IndexBuffer<unsigned int>(indices)
		);
		OnDebug(meshes.back().SetName(mesh.mName.C_Str()));
	}

	for (unsigned int i = 0; i < node.mNumChildren; i++) {
		LoadNode(*node.mChildren[i]);
	}
}

Model ModelLoader::LoadModel(const char *modelPath) {
	try {
		Assimp::Importer importer;
		scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		LoadNode(*scene->mRootNode);
		return Model(std::move(meshes));
	} catch (std::exception &e) {
		throw e;
	}
}
