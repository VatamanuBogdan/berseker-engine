#include "Skybox.h"
#include "ResourceLoaders/ShaderRegistry.h"

bool						Skybox::initialised = false;
Lateinit<VertexArray>			Skybox::skyboxVao;
std::shared_ptr<ShaderProgram> 	Skybox::skyboxShader;

Skybox::Skybox(const CubeMapResource &cubeMapResource)
	: cubeMapTexture(cubeMapResource) {
	Init();
}

void Skybox::Draw(const glm::mat4 &projection, const glm::mat4 &view) {
	glDepthMask(GL_FALSE);
	skyboxShader->Bind();
	skyboxShader->SetUniform("Projection", projection);
	skyboxShader->SetUniform("View", view);

	cubeMapTexture.Bind();

	OpenGL::DrawArrays(*skyboxVao, OpenGL::TRIANGLES, 0, 35);
	glDepthMask(GL_TRUE);
}

void Skybox::Init() {
	if (Skybox::initialised) {
		return;
	}

	skyboxShader = ShaderRegistry::Get().GetShader(ShaderResource::Skybox);
	auto format = VertexFormat<1>()
		  .Push<float>(3, 0);

	VertexBuffer vbo = {
		  -1.0f,  1.0f, -1.0f,
		  -1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f,  1.0f, -1.0f,
		  -1.0f,  1.0f, -1.0f,

		  -1.0f, -1.0f,  1.0f,
		  -1.0f, -1.0f, -1.0f,
		  -1.0f,  1.0f, -1.0f,
		  -1.0f,  1.0f, -1.0f,
		  -1.0f,  1.0f,  1.0f,
		  -1.0f, -1.0f,  1.0f,

		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,

		  -1.0f, -1.0f,  1.0f,
		  -1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f, -1.0f,  1.0f,
		  -1.0f, -1.0f,  1.0f,

		  -1.0f,  1.0f, -1.0f,
		  1.0f,  1.0f, -1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  -1.0f,  1.0f,  1.0f,
		  -1.0f,  1.0f, -1.0f,

		  -1.0f, -1.0f, -1.0f,
		  -1.0f, -1.0f,  1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  -1.0f, -1.0f,  1.0f,
		  1.0f, -1.0f,  1.0f
	};
	skyboxVao.Init();
	skyboxVao->AttachVertexBuffer(vbo, format);

	initialised = true;
}
