#include "LightSourceBillboard.h"

std::shared_ptr<ShaderProgram>	LightSourceBillboard::shader;
std::optional<VertexArray> 		LightSourceBillboard::quadVao;
std::optional<IndexBuffer> 		LightSourceBillboard::quadIbo;
std::optional<Texture2D>		LightSourceBillboard::texture;

LightSourceBillboard::LightSourceBillboard() {
	Init();
}

void LightSourceBillboard::Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
	constexpr int textureSlot = 0;

	shader->Bind();
	shader->SetUniform("Projection", projection);
	shader->SetUniform("Model", model);
	shader->SetUniform("View", view);
	shader->SetUniform("u_Texture", textureSlot);

	texture->Bind(textureSlot);

	quadVao->Bind();
	OpenGL::DrawElementsWith(*quadIbo);
}

void LightSourceBillboard::Init() {
	if (quadVao.has_value()) {
		return;
	}

	shader = ShaderRegistry::Get().GetShader(ShaderResource::LightSource);
	auto format = VertexFormat<2>()
		  .Push<float>(3, 0)
		  .Push<float>(2, 3 * sizeof(float));

	VertexBuffer vbo = {
		  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		  0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	quadVao.emplace();
	quadVao->AttachVertexBuffer(vbo, format);

	quadIbo.emplace(std::array<unsigned int, 6>{0, 3, 2, 2, 1, 0});

	texture.emplace("./res/textures/light-source.png");
}
