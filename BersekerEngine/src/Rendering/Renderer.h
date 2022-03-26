#pragma once
#include "Rendering/GraphicsAPI/VertexArray.hpp"
#include "Rendering/GraphicsAPI/IndexBuffer.hpp"
#include "Rendering/GraphicsAPI/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Rendering/Camera.hpp"

#include <vector>

class Renderer {
public:
	static void Init();
	static void Deinit();
	static void SetCamera(std::shared_ptr<Camera> &camera);
	static void SubmitForRendering(const VertexArray 			*vao,
						 const IndexBuffer<unsigned int> 	*ibo,
						 ShaderProgram 				*shader,
						 const glm::mat4 				&model);
	static void RenderClearColor(float red, float green, float blue, float alpha);
	static void Render();

private:
	struct RenderingEntity {
		const VertexArray				*vao;
		const IndexBuffer<unsigned int> 	*ibo;
		ShaderProgram				*shaderProgram;
		const glm::mat4				 model;

		RenderingEntity(const VertexArray 			*vao,
				    const IndexBuffer<unsigned int> *ibo,
				    ShaderProgram 			*shaderProgram,
				    const glm::mat4	 		&model)
				    : vao(vao), ibo(ibo), shaderProgram(shaderProgram), model(model) {
		}
	};

	Renderer() = default;

private:
	static std::shared_ptr<Camera> 	camera;
	static std::vector<RenderingEntity>	renderingQueue;
};

