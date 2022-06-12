#pragma once
#include "Rendering/GraphicsAPI/CubeMapTexture.hpp"
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Rendering/GraphicsAPI/ShaderProgram.h"

#include "../Utils.h"

#include <glm/glm.hpp>

#include <memory>

class Skybox {
public:
	explicit Skybox(const CubeMapResource &cubeMapResource);

	void Draw(const glm::mat4 &projection, const glm::mat4 &view);

private:
	static void Init();

private:
	CubeMapTexture 					cubeMapTexture;
	static bool 					initialised;
	static Lateinit<VertexArray> 			skyboxVao;
	static std::shared_ptr<ShaderProgram> 	skyboxShader;
};
