#pragma once
#include "Graphics/OpenGL/ShaderProgram.h"
#include "Graphics/OpenGL/VertexArray.hpp"
#include "Graphics/OpenGL/VertexBuffer.hpp"
#include "Graphics/OpenGL/IndexBuffer.hpp"

class GridLine {
public:
	GridLine()
		: shader(LoadShader()) {
	}

	void Draw(const glm::mat4 &view, const glm::mat4 &projection) {
		shader.Bind();
		shader.SetUniform("Projection", projection);
		shader.SetUniform("View", view);
		vao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	static ShaderProgram LoadShader() {
		return ShaderProgram::LoadFrom("res/shaders/GridLine.vert.glsl", "res/shaders/GridLine.frag.glsl");
	}

private:
	VertexArray vao;
	ShaderProgram shader;
};