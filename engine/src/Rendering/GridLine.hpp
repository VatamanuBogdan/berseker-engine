#pragma once
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/VertexBuffer.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "ResourceLoaders/ShaderRegistry.h"

class GridLine {
public:
	GridLine()
		: shader(ShaderRegistry::Get().GetShader(ShaderResource::GridLine)) {
	}

	void Draw(const glm::mat4 &view, const glm::mat4 &projection) {
		shader->Bind();
		shader->SetUniform("Projection", projection);
		shader->SetUniform("View", view);
		vao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	VertexArray vao;
	std::shared_ptr<ShaderProgram> shader;
};